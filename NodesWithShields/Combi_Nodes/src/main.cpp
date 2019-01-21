/**
@file
@author  Harm Meyer
@version 1.0

@section DESCRIPTION
This is the code of the bordernode.

The default JSON format is:
{
    "ADD": <address>,
    "CMD": <command>,
    "ARG": <target>
}
*/

#include "painlessMesh.h"
#include <EEPROM.h>

#define     MESH_PORT       5555
#define     MESH_PREFIX     "whateverYouLik"
#define     MESH_PASSWORD   "somethingSneak"
#define     EEPROM_ADDRESS  0

struct
{
    uint32_t ID = 123;
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

/**
Toggles Red LED

@param  none
@return none
*/
void toggleRedLed()
{
    //toggle
    if (redFlag)
        redFlag = !redFlag;
    else if (!redFlag)
        redFlag = !redFlag;
}

/**
Toggles Green led

@param  none
@return none
*/
void toggleGreenLed()
{
    //toggle
    if (greenFlag)
        greenFlag = !greenFlag;
    else if (!greenFlag)
        greenFlag = !greenFlag;
}

/**
Updates led status

@param  none
@return none
*/
void updateLeds()
{
    //digitalwrite
    if (redFlag)
        digitalWrite(red, HIGH);
    else if (!redFlag)
        digitalWrite(red, LOW);

    //digitalwrite
    if (greenFlag)
        digitalWrite(green, HIGH);
    else if (!greenFlag)
        digitalWrite(green, LOW);
}

/**
Recieve callback

This function gets a message and wil check if that message is a JSON. When
it isn't in JSON format it will execute the command if that command is a
command it recognizes. If it is a JSON then it will parse it and check the
command. Next it will execute the command.
@param  uint32_t from
@param  String &msg
@return none
*/
void callback(String msg, uint32_t from)
{
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
        else if (strcmp(root["CMD"], "U"))
        {
            uint n = 0;
            bool go = true;

            while (go)
            {
                if (data.address[n] == root["ARG"])
                {
                    data.address[n] = 0;
                    go = false;
                }
                else
                {
                    n++;
                }
            }
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
            toggleRedLed();
        }
        else if (strcmp(msg.c_str(), "ON") == 0)
        {
            if (!redFlag)
                toggleRedLed();
        }
        else if (strcmp(msg.c_str(), "OFF") == 0)
        {
            if (redFlag)
                toggleRedLed();
        }
        else if (strcmp(msg.c_str(), "GET") == 0)
        {
            String jsonMsg;
            jsonMsg = "{ \"ID\": ";
            jsonMsg += data.ID;
            jsonMsg += ", \"Address\": ";
            jsonMsg += mesh.getNodeId();
            jsonMsg += ", \"Group\": ";
            jsonMsg += data.group;
            jsonMsg += " }";
            Serial.println(jsonMsg);
            mesh.sendSingle(from, jsonMsg);
        }
        else
        {
            Serial.println("Error no command found: no Json");
        }
    }
}


/**
Shows the data in the EEPROM

@param  none
@return none
*/
void showEEPROM()
{
    EEPROM.get(EEPROM_ADDRESS, data);
    Serial.println("========");
    Serial.printf("Group: ");
    Serial.println(data.group);
    Serial.printf("ID: ");
    Serial.println(data.ID);
    Serial.printf("size: ");
    Serial.println(data.size);

    for (size_t i = 0; i < data.size; i++)
    {
        Serial.printf("address[%d]: ", i);
        Serial.println(data.address[i]);
    }
    Serial.println("========");
}

/**
Writes to EEPROM

@param  none
@return none
*/
void writeEEPROM()
{
    EEPROM.put(EEPROM_ADDRESS, data);
    EEPROM.commit();
}

/**
Button press chekcer

Checks if the sendbutton is being pressed. It wil send out a msg if it is.
@param  none
@return none
*/
void sendButtonFunction()
{
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
}

/**
Button press chekcer

Checks if the onbutton is being pressed. It wil read out the EEPROM if it is.
@param  none
@return none
*/
void onButtonFunction()
{
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

/**
Button press chekcer

Checks if the resetbutton is being pressed. If it is then it will clear the EEPROM.
@param  none
@return none
*/
void resetbuttonFunction()
{
    if (digitalRead(resetButton) == LOW && resetFlag)
    {
        Serial.println("========");
        Serial.println("resetButton");
        resetFlag = !resetFlag;

        data.ID = 123;
        data.group = 0;
        data.size = 0;
        for (int i = 0 ; i < 64 ; i++)
        {
            data.address[i] = 0;
        }
        writeEEPROM();
    }
    else if (digitalRead(resetButton) == HIGH && !resetFlag)
    {
        resetFlag = !resetFlag;
        Serial.println("nonButton");
        Serial.println("========");
    }
}

/**
normal callback

Is used by painlessmesh to check for a callback from a node. First it will run
the Callback() function. Then it will read out the EEPROM. After that it shows
what's in the EEPROM to validate the data.
@param  none
@return none
*/
void normalCallback( uint32_t from, String &msg )
{
    Serial.println("========");
    Serial.println( "normalCallback()" );
    digitalWrite(green, HIGH);

    callback(msg, from);

    writeEEPROM();

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

/**
This function will run once on startup

Sets the baudrate and prepares the mesh network. After that it prints out if
the node is a bordernode or a combinode and the address the node. This also
preps the EEPROM.
@param  none
@return none
*/
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

    Serial.println("Type: CombiNode");
    Serial.printf("Address: ");
    Serial.println( mesh.getNodeId() );
}

/**
This function will loop

Reads out the button inputs and updates the led. It also runs the mesh and
scheduler.
@param  none
@return none
*/
void loop()
{
    userScheduler.execute();
    mesh.update();

    onButtonFunction();
    sendButtonFunction();
    resetbuttonFunction();

    updateLeds();
}
