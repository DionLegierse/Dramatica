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

#define   MESH_PREFIX     "whateverYouLik"
#define   MESH_PASSWORD   "somethingSneak"
#define   MESH_PORT       5555

char msgSerial[100];
int counter = 0;
bool parseJson = false;

String bridgeJson[64];
uint8_t nodeAmount;
uint8_t nodeCount;
bool getNodeInfo = false;

Scheduler userScheduler; // to control your personal task
painlessMesh mesh;

/**
Recieve callback

This is only used for the GET command. It will check if there is a callback
from a node. It will then add the JSON to an array of strings. and finally
it wil merge those JSONS to create on big JSON for the bridge.
@param  uint32_t from
@param  String &msg
@return none
*/
void receivedCallback( uint32_t from, String &msg )
{
    Serial.printf( "startHere: Received from %u msg=%s\n", from, msg.c_str() );
    Serial.println();
    bridgeJson[nodeCount] = msg;
    nodeCount++;
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


/**
This function will run once on startup

Sets the baudrate and prepares the mesh network. After that it prints out if
the node is a bordernode or a combinode and the address of the
@param  none
@return none
*/
void setup()
{
    Serial.begin(9600);

    //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
    mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

    mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

    Serial.println("Type: BorderNode");
    Serial.printf("Address: ");
    Serial.println( mesh.getNodeId() );
}

/**
This function will loop

It wil first check for a serial input. if there is a serial input then it wil
check if that input is in a JSON format. If it is in a JSON format then it
will parse it and handle the parameters that it got as input. It also runs the
mesh and the scheduler.
@param  none
@return none
*/
void loop()
{
    userScheduler.execute(); // it will run mesh scheduler as well
    mesh.update();

    //get the incomming message ready for Json parsing
    if (Serial.available() > 0 && parseJson == false)
    {
        int msgIn = Serial.read();

        msgSerial[counter] = msgIn;
        if (msgSerial[counter] == '}')
        {
            parseJson = true;
        }
        counter ++;
    }

    if (parseJson)
    {
        Serial.println("========");
        counter = 0;
        Serial.println("JSON RECEIVED");
        String jsonOut(msgSerial);
        String msgOut;
        Serial.println(jsonOut);

        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.parseObject(jsonOut);

        if ( root.success() )
        {
            Serial.println("PARSE SUCCESFULL!");
            uint32_t ADD = root["ADD"];
            String CMD = root["CMD"];
            String ARG = root["ARG"];

            Serial.print("ADD:\t");
            Serial.println(ADD);
            Serial.print("CMD:\t");
            Serial.println(CMD);
            Serial.print("ARG:\t");
            Serial.println(ARG);

            if (strcmp(root["CMD"], "L") == 0)
            {
                mesh.sendSingle(ADD, jsonOut);
            }
            else if (strcmp(root["CMD"], "U") == 0)
            {
                mesh.sendSingle(ADD, jsonOut);
            }
            else if (strcmp(root["CMD"], "T") == 0)
            {
                msgOut = "T";
                mesh.sendSingle(ADD, msgOut);
            }
            else if (strcmp(root["CMD"], "ON") == 0)
            {
                msgOut = "ON";
                mesh.sendSingle(ADD, msgOut);
            }
            else if (strcmp(root["CMD"], "OFF") == 0)
            {
                msgOut = "OFF";
                mesh.sendSingle(ADD, msgOut);
            }
            else if (strcmp(root["CMD"], "G") == 0)
            {
                mesh.sendSingle(ADD, jsonOut);
            }
            else if (strcmp(root["CMD"], "GET") == 0)
            {
                msgOut = "GET";

                std::list<uint32_t> temp = mesh.getNodeList();
                for (uint32_t i : temp)
                {
                    mesh.sendSingle(i, msgOut);
                    Serial.println(i);
                    nodeAmount++;
                }
                getNodeInfo = true;
            }
            else
            {
                Serial.println("Error no command found");
            }
        }
        parseJson = false;

        Serial.println("END OF SENDING...");
        Serial.println("========");
    }

    if (nodeCount == nodeAmount && getNodeInfo)
    {
        String toBridge = "[";

        for (size_t i = 0; i < nodeAmount; i++)
        {
            toBridge += bridgeJson[i] + ", ";
        }

        toBridge.remove(toBridge.length()-2);
        toBridge += "]";


        Serial.println(toBridge);

        nodeCount = 0;
        nodeAmount = 0;

        getNodeInfo = false;
    }
}
