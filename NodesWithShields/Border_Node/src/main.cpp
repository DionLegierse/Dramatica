/*
This code is a piece of testcode for the mesh network
*/

#include "painlessMesh.h"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

char msgSerial[100];
int counter = 0;
bool parseJson = false;

Scheduler userScheduler; // to control your personal task
painlessMesh mesh;

// Needed for painless library
void receivedCallback( uint32_t from, String &msg )
{
    Serial.printf( "startHere: Received from %u msg=%s\n", from, msg.c_str() );
    /* Json format
    { "AddressIn": 123412, "AddressOut": 123412, "Button": [0, 0, 0, 0, 0, 0] }
    */
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

    if (Serial.available() > 0 && parseJson == false)
    {
        int msgIn = Serial.read();

        msgSerial[counter] = msgIn;
        if (msgSerial[counter] == '}')
        {
            parseJson = true;
        }
        counter ++;
        /* Json format
        { "AddressIn": 123412, "AddressOut": 123412}
        */
    }

    if (parseJson)
    {
        counter = 0;
        Serial.println("=======================JSON=========================");
        String msgOut(msgSerial);
        Serial.println(msgOut);

        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.parseObject(msgOut);

        if (root.success())
        {
            Serial.println("======================SUCCES========================");
            uint32_t addressIn = root["AddressIn"];
            uint32_t addressOut = root["AddressOut"];

            Serial.print("AddressIn:\t");
            Serial.println(addressIn);
            Serial.print("AddressOut:\t");
            Serial.println(addressOut);

            mesh.sendSingle(addressIn, msgOut);
        }
        Serial.println("========================End=========================\n\n\n\n");
        parseJson = false;
    }
}
