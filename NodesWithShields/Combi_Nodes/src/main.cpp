/*
This code is a piece of testcode for the mesh network
*/

#include "painlessMesh.h"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

const int red = D6;
const int green = D5;
const int activeButton = D3;
const int inactiveButton = D2;
const int reset = D4;

bool redFlag = false;
bool greenFlag = false;
bool activeButtonFlag = true;
bool inactiveButtonFlag = true;

Scheduler userScheduler; // to control your personal task
painlessMesh mesh;

uint32_t address[64];
int size = 0;

// Needed for painless library
void receivedCallback( uint32_t from, String &msg )
{
    Serial.printf( "startHere: Received from %u msg=%s\n", from, msg.c_str() );
    digitalWrite(green, HIGH);
    /* Json format
    { "AddressIn": 123412, "AddressOut": 123412}
    */
    Serial.println("=======================JSON=========================");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(msg.c_str());

    if (root.success())
    {
        Serial.println("======================SUCCES========================");
        int n = 0;
        //while loop might be dangerous because of skedular
        if (address[n] != root["AddressOut"])
        {
            while (address[n] != 0)
            {
                n++;
            }
        }

        //check the value of n
        Serial.print("n = ");
        Serial.println(n);

        //bind button one to the 2 leds
        address[n] = root["AddressOut"];
        size = n+1;
    }
    else
    {
        //give an error if the root didn't parse
        Serial.println("Root error, no json detected!");
    }

    Serial.println("======================Toggle=======================");
    //toggle led 1
    if (strcmp(msg.c_str(), "RedToggle") == 0)
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

    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(activeButton, INPUT_PULLUP);
    pinMode(inactiveButton, INPUT_PULLUP);

    for (uint32_t i = 0; i < 64; i++)
    {
        address[i] = 0;
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

    for (int i = 0; i <= size; i++)
    {
        //read button 0 and turn on/off led 0
        if (digitalRead(activeButton) == LOW && activeButtonFlag)
        {
            Serial.println("ButtonPressed");

            String msg = "RedToggle";
            mesh.sendSingle(address[i], msg);

            Serial.println(msg);

            activeButtonFlag = !activeButtonFlag;
        }
        else if (digitalRead(activeButton) == HIGH && !activeButtonFlag)
        {
            Serial.println("ButtonReleased");

            activeButtonFlag = !activeButtonFlag;
        }
    }
}
