#include "painlessMesh.h"

#define MESH_PREFIX "whateverYouLike"
#define MESH_PASSWORD "somethingSneaky"
#define MESH_PORT 5555

Scheduler userScheduler; // to control your personal task
painlessMesh mesh;

const int motor1A = 14;
const int motor1B = 12;
const int motor2A = 13;
const int motor2B = 15;
const int ledPin = LED_BUILTIN;

void receivedCallback(uint32_t from, const String &msg) {
  Serial.printf("Received from %u msg=%s\n", from, msg.c_str());

  char command = msg.charAt(0);

  // Process the received command and control the motors and LED
  switch (command) {
    case 'F':
      Serial.println("Move FORWARD");
      moveForward();
      blinkLED(1);
      break;
    case 'B':
      Serial.println("Move BACKWARD");
      moveBackward();
      blinkLED(2);
      break;
    case 'R':
      Serial.println("Turn RIGHT");
      turnRight();
      blinkLED(3);
      break;
    case 'L':
      Serial.println("Turn LEFT");
      turnLeft();
      blinkLED(4);
      break;
    default:
      Serial.println("Invalid command");
      stopMotors();
      blinkLED(5);
      break;
  }
}

void moveForward() {
  digitalWrite(motor1A, HIGH);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, HIGH);
  digitalWrite(motor2B, LOW);
}

void moveBackward() {
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, HIGH);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, HIGH);
}

void turnRight() {
  digitalWrite(motor1A, HIGH);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, HIGH);
}

void turnLeft() {
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, HIGH);
  digitalWrite(motor2A, HIGH);
  digitalWrite(motor2B, LOW);
}

void stopMotors() {
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, LOW);
}

void blinkLED(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
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

  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
  pinMode(ledPin, OUTPUT);
}
\
void loop() {
  mesh.update();
}
