#include "painlessMesh.h"

#define MESH_PREFIX "whateverYouLike"
#define MESH_PASSWORD "somethingSneaky"
#define MESH_PORT 5555

Scheduler userScheduler; // to control your personal task
painlessMesh mesh;

void receivedCallback(uint32_t from, const String &msg) {
  Serial.printf("Received from %u msg=%s\n", from, msg.c_str());

  char command = msg.charAt(0);

  // Process the received command and control the LED
  switch (command) {
    case 'F':
      Serial.println("Move FORWARD");
      blinkLED(1);
      // Add code to move the bot forward
      break;
    case 'B':
      Serial.println("Move BACKWARD");
      blinkLED(2);
      // Add code to move the bot backward
      break;
    case 'R':
      Serial.println("Turn RIGHT");
      blinkLED(3);
      // Add code to turn the bot right
      break;
    case 'L':
      Serial.println("Turn LEFT");
      blinkLED(4);
      // Add code to turn the bot left
      break;
    default:
      Serial.println("Invalid command");
      break;
  }
}

void blinkLED(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
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

  pinMode(LED_BUILTIN, OUTPUT); // Initialize the built-in LED pin as an output
}

void loop() {
  mesh.update();
}
