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

#define MESH_PORT       5555

//variables for the eeprom and the network
struct
{
    String name = "node";
    String ssid = "whateverYouLike";
    String password = "somethingSneaky";
    uint8_t group = 0;
    uint32_t address[64];
    uint8_t size = 0;
} data;

//eeprom addresses
unsigned int eepromAddress = 0;

//states for the node
enum STATE {CONNECT, MESH};
STATE state = MESH;

//In and Outputs
const int red = D6;
const int green = D5;
const int activeButton = D3;
const int connectButton = D2;
const int resetButton = D4;

//Flags
bool redFlag = false;
bool greenFlag = false;
bool activeButtonFlag = true;
bool connectButtonFlag = true;
bool setupFlag = true;

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

void setupNodeCallback( uint32_t from, String &msg )
{
    Serial.printf( "newConnectionCallback()" );
}

// Needed for painless library
void normalCallback( uint32_t from, String &msg )
{
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
            unsigned int n = 0;
            //while loop might be dangerous because of skedular
            if (data.address[n] != root["ARG"])
            {
                while (data.address[n] != 0)
                {
                    n++;
                }
                data.size = n + 1;
            }

            data.address[n] = root["ARG"];

            //debugging
            Serial.print("n = ");
            Serial.println(n);
            Serial.print("size = ");
            Serial.println(data.size);
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

    EEPROM.put(eepromAddress, data);
    EEPROM.commit();
    Serial.println("Done receiving...");
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
    EEPROM.begin(512);
    Serial.begin(9600);

    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(activeButton, INPUT_PULLUP);
    pinMode(connectButton, INPUT_PULLUP);

    //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
    mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

    mesh.init( data.ssid, data.password, &userScheduler, MESH_PORT );
    mesh.onReceive(&normalCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
}

void loop()
{

    if (digitalRead(connectButton) == LOW)
    {
        state = CONNECT;
    }

    if (state == CONNECT)
    {
        if (setupFlag)
        {
            setupFlag = !setupFlag;
            mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

            mesh.init( "admin", "admin", &userScheduler, MESH_PORT );
            mesh.onReceive(&setupNodeCallback);
            mesh.onNewConnection(&newConnectionCallback);
            mesh.onChangedConnections(&changedConnectionCallback);
            mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
        }

        userScheduler.execute(); // it will run mesh scheduler as well
        mesh.update();

        state = MESH;
    }


    if (state == MESH)
    {
        userScheduler.execute(); // it will run mesh scheduler as well
        mesh.update();

        for (unsigned int i = 0; i < 64; i++)
        {
            //read button 0 and turn on/off led 0
            if (digitalRead(activeButton) == LOW && activeButtonFlag)
            {
                Serial.println("ButtonPressed");

                /*
                String msg = "RedToggle";
                mesh.sendSingle(data.address[i], msg);

                Serial.println(msg);

                activeButtonFlag = !activeButtonFlag;
                */

                EEPROM.get(eepromAddress, data);
                Serial.printf("Group: ");
                Serial.println(data.group);
                Serial.printf("name: ");
                Serial.println(data.name);
                Serial.printf("ssid: ");
                Serial.println(data.ssid);
                Serial.printf("password: ");
                Serial.println(data.password);

                for (size_t i = 0; i < data.size; i++)
                {
                    Serial.printf("address[%d]: ", i);
                    Serial.println(data.address[i]);
                }
            }
            else if (digitalRead(activeButton) == HIGH && !activeButtonFlag)
            {
                Serial.println("ButtonReleased");

                activeButtonFlag = !activeButtonFlag;
            }
        }
    }

    if (eepromAddress == EEPROM.length())
    {
        eepromAddress = 0;
        printf("%d", EEPROM.length());
    }
}
