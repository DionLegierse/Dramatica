/*
This code is a piece of testcode for the mesh network
*/

#include "painlessMesh.h"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

const int LED = D2;
const int BUT = D3;

bool allowButton = true;
bool ledOn = false;

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

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

  if (strcmp(msg.c_str(), "E") == 0)
  {
    if (ledOn)
      ledOn = !ledOn;
    else if (!ledOn)
      ledOn = !ledOn;

    if (ledOn)
      digitalWrite(LED, HIGH);
    else if (!ledOn)
      digitalWrite(LED, LOW);
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
  pinMode(LED, OUTPUT);
  pinMode(BUT, INPUT_PULLUP);

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

  if (digitalRead(BUT) == LOW && allowButton)
  {
    String msg = "E";
    mesh.sendBroadcast(msg);
    allowButton = !allowButton;
  }
  else if (digitalRead(BUT) == HIGH && !allowButton)
  {
    allowButton = !allowButton;
  }
}
