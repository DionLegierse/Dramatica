/*
Default Json format
{
    "ADD": <address>,
    "CMD": <command>,
    "ARG": <target>
}
*/

#include "painlessMesh.h"
#include <EEPROM.h>

#define     MESH_PORT       5555
#define     MESH_PREFIX     "whateverYouLike"
#define     MESH_PASSWORD   "somethingSneaky"
#define     EEPROM_ADDRESS  0

//variables for the eeprom and the network
struct
{
    String name = "node";
    uint8_t group = 0;
    uint32_t address[64];
    uint8_t size = 1;

} data;

//In and Outputs
const int red = D6;
const int green = D5;
const int sendButton = D3;
const int onButton = D2;
const int resetButton = D4;

//Flags
bool redFlag = false;
bool greenFlag = false;
bool sendButtonFlag = true;
bool onButtonFlag = true;
bool resetFlag = true;

//schedular and mesh
Scheduler userScheduler;
painlessMesh mesh;

void toggleLed()
{
    //toggle
    if (redFlag)
        redFlag = !redFlag;
    else if (!redFlag)
        redFlag = !redFlag;

    //digitalwrite
    if (redFlag)
        digitalWrite(red, HIGH);
    else if (!redFlag)
        digitalWrite(red, LOW);
}

void showEEPROM()
{
    EEPROM.get(EEPROM_ADDRESS, data);
    Serial.println("========");
    Serial.printf("Group: ");
    Serial.println(data.group);
    Serial.printf("name: ");
    Serial.println(data.name);
    Serial.printf("size: ");
    Serial.println(data.size);

    for (size_t i = 0; i < data.size; i++)
    {
        Serial.printf("address[%d]: ", i);
        Serial.println(data.address[i]);
    }
    Serial.println("========");
}


void setupNodeCallback( uint32_t from, String &msg )
{
    Serial.printf( "newConnectionCallback()" );
}

// Needed for painless library
void normalCallback( uint32_t from, String &msg )
{
    Serial.println("========");
    Serial.println( "normalCallback()" );
    Serial.println( msg.c_str() );
    digitalWrite(green, HIGH);

    Serial.println("Parsing...");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(msg.c_str());

    if (root.success())
    {
        Serial.println("Json Parse succesfull!");

        if (strcmp(root["CMD"], "L") == 0)
        {
            uint n = 0;
            bool go = true;

            while (go)
            {
                if (data.address[n] == root["ARG"])
                {
                    data.address[n] = root["ARG"];
                    go = false;
                }
                else if(data.address[n] == 0)
                {
                    data.address[n] = root["ARG"];
                    go = false;
                }
                else
                {
                    n++;
                }
            }
            data.size = n + 1;
        }
        else if (strcmp(root["CMD"], "G") == 0)
        {
            data.group = root["ARG"];
        }
        else
        {
            Serial.println("Error no command found: yes Json");
        }
    }
    else
    {
        //give an error if the root didn't parse
        Serial.println("No Json to parse");

        if (strcmp(msg.c_str(), "T") == 0)
        {
            toggleLed();
        }
        else if (strcmp(msg.c_str(), "ON") == 0)
        {
            if (!redFlag)
                toggleLed();
        }
        else if (strcmp(msg.c_str(), "OFF") == 0)
        {
            if (redFlag)
                toggleLed();
        }
        else
        {
            Serial.println("Error no command found: no Json");
        }
    }

    EEPROM.put(EEPROM_ADDRESS, data);
    EEPROM.commit();

    showEEPROM();

    Serial.println("Done receiving...");
    Serial.println("========");
}

void newConnectionCallback(uint32_t nodeId)
{
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback()
{
    Serial.printf("Changed connections %s\n", mesh.subConnectionJson().c_str());
}

void nodeTimeAdjustedCallback(int32_t offset)
{
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup()
{
    Serial.begin(9600);
    EEPROM.begin(1024);

    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(onButton, INPUT_PULLUP);
    pinMode(sendButton, INPUT_PULLUP);
    pinMode(resetButton, INPUT_PULLUP);

    //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
    mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

    mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
    mesh.onReceive(&normalCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

    Serial.println("CombiNode");
}

void loop()
{
    userScheduler.execute(); // it will run mesh scheduler as well
    mesh.update();

    //read button 0 and turn on/off led 0
    if (digitalRead(sendButton) == LOW && sendButtonFlag)
    {
        Serial.println("========");
        Serial.println("sendButton");

        sendButtonFlag = !sendButtonFlag;

        for (unsigned int i = 0; i < data.size; i++)
        {
            String msg = "T";
            mesh.sendSingle(data.address[i], msg);

            Serial.println(msg);
        }
    }
    else if (digitalRead(sendButton) == HIGH && !sendButtonFlag)
    {
        Serial.println("nendButton");

        sendButtonFlag = !sendButtonFlag;
        Serial.println("========");
    }

    if (digitalRead(onButton) == LOW && onButtonFlag)
    {
        Serial.println("========");
        onButtonFlag = !onButtonFlag;
        Serial.println("onButton");
        showEEPROM();
    }
    else if (digitalRead(onButton) == HIGH && !onButtonFlag)
    {
        onButtonFlag = !onButtonFlag;
        Serial.println("nonButton");
        Serial.println("========");
    }
}
