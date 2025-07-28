#include <Arduino.h>
//#include <painlessMesh.h>
#include "NTBpM.h"
#include <NTBoled.h>

// Create a mesh object using your credentials
NTBpM meshNode("yourSSID", "yourPassword", 5555);  // Replace with actual values

NTBoled oled;

unsigned long lastSendTime = 0;
const unsigned long sendInterval = 3000;

// Custom receive handler to display incoming messages
void handleReceivedMessage(uint32_t from, String msg) {
  Serial.printf("📩 Message from %u: %s\n", from, msg.c_str());
}


void setup() {

  if (!oled.begin()) {
    Serial.println(F("Failed to start OLED"));
    while (true);
  }
  // Clear the screen buffer
  oled.clear();
  oled.show();    

  meshNode.begin();
  meshNode.setReceiveCallback(handleReceivedMessage);
  randomSeed(analogRead(A0));  // Optional: improves randomness

}

void loop() {
  meshNode.update();

  if (millis() - lastSendTime >= sendInterval) {
    int randomNumber = random(0, 1000);
    meshNode.sendBroadcast(String(randomNumber));

    //oled.print(meshNode.getNodeId(), 1, 5, 5);
    //oled.show(); // Update the display

    lastSendTime = millis();
  }
}
