#include "painlessMesh.h"

#define MESH_PREFIX "whateverYouLike"
#define MESH_PASSWORD "somethingSneaky"
#define MESH_PORT 5555

Scheduler userScheduler; // to control your personal task
painlessMesh mesh;

Task taskSendMessage(TASK_SECOND *1, TASK_FOREVER, []() {
  if (Serial.available()) {
    char command = Serial.read();
    mesh.sendBroadcast(&command, sizeof(command));
  }
});

void receivedCallback(uint32_t from, const String &msg) {
  Serial.printf("Received from %u msg=%s\n", from, msg.c_str());

  char command = msg.charAt(0);

  // Process the received command
  switch (command) {
    case 'F':
      Serial.println("Move FORWARD");
      // Add code to move the bot forward
      break;
    case 'B':
      Serial.println("Move BACKWARD");
      // Add code to move the bot backward
      break;
    case 'R':
      Serial.println("Turn RIGHT");
      // Add code to turn the bot right
      break;
    case 'L':
      Serial.println("Turn LEFT");
      // Add code to turn the bot left
      break;
    default:
      Serial.println("Invalid command");
      break;
  }
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.println("Changed connections");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void setup() {
  Serial.begin(115200);

  mesh.setDebugMsgTypes(ERROR | STARTUP);

  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive([](uint32_t from, const String &msg) { receivedCallback(from, msg); });
  mesh.onNewConnection([](uint32_t nodeId) { newConnectionCallback(nodeId); });
  mesh.onChangedConnections([]() { changedConnectionCallback(); });
  mesh.onNodeTimeAdjusted([](int32_t offset) { nodeTimeAdjustedCallback(offset); });

  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();
}

void loop() {
  mesh.update();
}
