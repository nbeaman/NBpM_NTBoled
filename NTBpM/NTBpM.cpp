#include "NTBpM.h"

// Initialize the static callback holder to null
MeshReceiveCallback NTBpM::userCallback = nullptr;

// Constructor: sets credentials and configures task to send a default message every 10 seconds
NTBpM::NTBpM(const String& ssid, const String& password, uint16_t port)
  : _ssid(ssid), _password(password), _port(port)
  {
    // Optional: setup task later if needed.  You can automatically send message(s) every TASK_SECOND.
	// If you uncomment the taskSendMessage below, you will have to add and enable the periodic task to send messages in the begin() section below (near the end):
	// userScheduler.addTask(taskSendMessage);
	// taskSendMessage.enable();
	
	// taskSendMessage(TASK_SECOND * 10, TASK_FOREVER, std::bind(&NTBpM::sendBroadcast, this, "Hello from NTBpM!")) {}
  }

void NTBpM::begin() {
  Serial.begin(115200);  // Begin serial communication for debugging

  // Optional: enable debug output from painlessMesh
  // mesh.setDebugMsgTypes(ERROR | STARTUP | CONNECTION);

  // Initialize the mesh network with credentials and scheduler
  mesh.init(_ssid, _password, &userScheduler, _port);

  // Attach internal handlers to mesh events
  mesh.onReceive(&NTBpM::receivedCallback);
  mesh.onNewConnection(&NTBpM::newConnectionCallback);
  mesh.onChangedConnections(&NTBpM::changedConnectionCallback);

}

// Add this method at the end (or anywhere in the implementation file)
String NTBpM::getNodeId() {
  // Convert the uint32_t nodeId to String
  return String(mesh.getNodeId());
}

void NTBpM::update() {
  // Continuously called to keep the mesh network alive and responsive
  mesh.update();
}

void NTBpM::sendBroadcast(const String& message) {
  // Sends a message to all nodes in the mesh
  mesh.sendBroadcast(message);
  Serial.println("Broadcasting: " + message);
}

void NTBpM::setReceiveCallback(MeshReceiveCallback cb) {
  // Stores the user’s custom message handler
  userCallback = cb;
}

void NTBpM::receivedCallback(uint32_t from, String &msg) {
  // Displays message info in Serial Monitor
  // Serial.printf("Received from %u: %s\n", from, msg.c_str());

  // If user provided a custom handler, call it
  if (userCallback) {
    userCallback(from, msg);
  }
}

void NTBpM::newConnectionCallback(uint32_t nodeId) {
  // Inform when a new node joins the mesh
  Serial.printf("New connection, nodeId = %u\n", nodeId);
}

void NTBpM::changedConnectionCallback() {
  // Inform when mesh topology (who's connected to whom) changes
  Serial.println("Mesh connections changed");
}
