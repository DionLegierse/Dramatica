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
  int address = 0;
  int led[2] = {0, 0};
  int button[2] = {0, 0};
};

NodeInfo nodeInfo[100];

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain
void sendButton();

void sendMessage()
{
  String msg = "Hello from node ";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg )
{
  Serial.printf( "startHere: Received from %u msg=%s\n", from, msg.c_str() );

  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(msg);

  if (root.succes())
  {
    int n = 0;
    while (nodeInfo[n].address != 0)
    {
      n++;
    }

    nodeInfo[n].address = root["address"];
  }

  //toggle led 0
  if (strcmp(msg.c_str(), "0") == 0)
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
  if (strcmp(msg.c_str(), "1") == 0)
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
  pinMode(led[0], OUTPUT);
  pinMode(led[1], OUTPUT);
  pinMode(button[0], INPUT_PULLUP);
  pinMode(button[1], INPUT_PULLUP);

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

  //read button 0 and turn on/off led 0
  if (digitalRead(button[0]) == LOW && allowButton[0])
  {
    String msg = "0";
    mesh.sendBroadcast(msg);
    allowButton[0] = !allowButton[0];
  }
  else if (digitalRead(button[0]) == HIGH && !allowButton[0])
  {
    allowButton[0] = !allowButton[0];
  }

  //read button 0 and turn on/off led 0
  if (digitalRead(button[1]) == LOW && allowButton[1])
  {
    String msg = "1";
    mesh.sendBroadcast(msg);
    allowButton[1] = !allowButton[1];
  }
  else if (digitalRead(button[1]) == HIGH && !allowButton[1])
  {
    allowButton[1] = !allowButton[1];
  }
}
