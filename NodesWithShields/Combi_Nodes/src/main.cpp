/*
This code is a piece of testcode for the mesh network
*/

#include "painlessMesh.h"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

const int led[2]    = {D5, D6};
const int button[2] = {D3, D2};
const int reset     = D4;

bool allowButton[2] = {true, true};
bool ledOn[2]       = {false, false};

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

//struct to configure inputs/outputs
struct NodeInfo
{
    uint32_t address = 0;
    int button[6] = {0, 0, 0, 0, 0, 0};
};

NodeInfo nodeInfo[64];

// Needed for painless library
void receivedCallback( uint32_t from, String &msg )
{
    Serial.printf( "startHere: Received from %u msg=%s\n", from, msg.c_str() );
    /* Json format
    { "AddressIn": 123412, "AddressOut": 123412, "Button": [0, 0, 0, 0, 0, 0] }
    */
    Serial.println("=======================JSON=========================");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(msg.c_str());
    if (root.success())
    {
        Serial.println("======================SUCCES========================");
        int n = 0;
        //while loop might be dangerous because of skedular
        if (nodeInfo[n].address != root["AddressOut"])
        {
            while (nodeInfo[n].address != 0)
            {

                n++;
            }
        }

        //check the value of n
        Serial.print("n = ");
        Serial.println(n);

        //bind button one to the 2 leds
        nodeInfo[n].address = root["AddressOut"];
        for (size_t i = 0; i < 6; i++)
        {
            nodeInfo[n].button[i] = root["Button"][i];
        }
    }
    else
    {
        //give an error if the root didn't parse
        Serial.println("Root error, no json detected!");
    }

    Serial.println("======================BUTTONs=======================");
    if (strcmp(msg.c_str(), "GreenToggle") == 0)
    {
        //toggle led with index i;
        if (ledOn[0])
            ledOn[0] = !ledOn[0];
        else if (!ledOn[0])
            ledOn[0] = !ledOn[0];

        if (ledOn[0])
            digitalWrite(led[0], HIGH);
        else if (!ledOn[0])
            digitalWrite(led[0], LOW);
    }
    //toggle led 1
    if (strcmp(msg.c_str(), "RedToggle") == 0)
    {
        //toggle led with index i;
        if (ledOn[1])
            ledOn[1] = !ledOn[1];
        else if (!ledOn[1])
            ledOn[1] = !ledOn[1];

        if (ledOn[1])
            digitalWrite(led[1], HIGH);
        else if (!ledOn[1])
            digitalWrite(led[1], LOW);
    }
    Serial.println("=======================DONE=========================");
}

void newConnectionCallback(uint32_t nodeId)
{
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback()
{
    Serial.printf("Changed connections %s\n",mesh.subConnectionJson().c_str());
}

void nodeTimeAdjustedCallback(int32_t offset)
{
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup()
{
    Serial.begin(9600);
    for (int i = 0; i < 2; i++)
    {
    pinMode(led[i], OUTPUT);
    pinMode(button[i], INPUT_PULLUP);
    }

    //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
    mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

    mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
}

void loop()
{
    userScheduler.execute(); // it will run mesh scheduler as well
    mesh.update();

    for (int i = 0; i < 2; i++)
    {
        //read button 0 and turn on/off led 0
        if (digitalRead(button[i]) == LOW && allowButton[i])
        {
            //first button
            if (nodeInfo[0].button[0] == 1)
            {
                if (nodeInfo[0].button[1] == 1)
                {
                    String msg = "GreenToggle";
                    mesh.sendSingle(nodeInfo[0].address, msg);

                    Serial.println(msg);
                }
                if (nodeInfo[0].button[2] == 1)
                {
                    String msg = "RedToggle";
                    mesh.sendSingle(nodeInfo[0].address, msg);

                    Serial.println(msg);
                }
            }
            //second button
            if (nodeInfo[0].button[3] == 1)
            {
                if (nodeInfo[0].button[4] == 1)
                {
                    String msg = "GreenToggle";
                    mesh.sendSingle(nodeInfo[0].address, msg);

                    Serial.println(msg);
                }
                if (nodeInfo[0].button[5] == 1)
                {
                    String msg = "RedToggle";
                    mesh.sendSingle(nodeInfo[0].address, msg);

                    Serial.println(msg);
                }
            }
            allowButton[i] = !allowButton[i];

        }
        else if (digitalRead(button[i]) == HIGH && !allowButton[i])
        {
            allowButton[i] = !allowButton[i];
        }
    }
}
