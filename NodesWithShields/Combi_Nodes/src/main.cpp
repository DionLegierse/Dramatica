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
    int button[2][2] = { {0, 0}, {0, 0} };
};

NodeInfo nodeInfo[100];

// Needed for painless library
void receivedCallback( uint32_t from, String &msg )
{
    Serial.printf( "startHere: Received from %u msg=%s\n", from, msg.c_str() );
    /* Json format
    {
    	"AddressIn": 123412,
    	"AddressOut": 123412,
    	"Button": [[0, 0], [0, 0]]
    }
    */
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(msg);
    if (root.success())
    {
        int n = 0;
        while (nodeInfo[n].address != 0)
        {
            n++;
        }

        //bind button one to the 2 leds
        nodeInfo[n].address = root["AddressOut"];
        for (size_t i = 0; i < 2; i++)
        {
            for (size_t k = 0; k < 2; k++)
            {
                nodeInfo[n].button[k][i] = root["Button"][k][i];
            }
        }
    }
    else
    {
        Serial.println("Root error, no json detected!");
    }

  //toggle led 0
  if (strcmp(msg.c_str(), "GreenToggle") == 0)
  {
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
    if (strcmp(msg.c_str(), "RedToggles") == 0)
    {
        if (ledOn[1])
            ledOn[1] = !ledOn[1];
        else if (!ledOn[1])
            ledOn[1] = !ledOn[1];

        if (ledOn[1])
            digitalWrite(led[1], HIGH);
        else if (!ledOn[1])
            digitalWrite(led[1], LOW);
    }
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
            if (nodeInfo[i].button[i][0] == 1)
            {
                String msg = "GreenToggle";
                mesh.sendSingle(nodeInfo[i].address, msg);
            }
            if (nodeInfo[i].button[i][1] == 1)
            {
                String msg = "RedToggle";
                mesh.sendSingle(nodeInfo[i].address, msg);
            }
            allowButton[i] = !allowButton[i];
        }
        else if (digitalRead(button[i]) == HIGH && !allowButton[i])
        {
            allowButton[i] = !allowButton[i];
        }
    }
}
