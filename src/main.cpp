#include <SPI.h>
#include "mcp_can.h"

// Define MCP2515 SPI CS pin
const int CS_PIN = 5; // Adjust if using a different pin

// Initialize MCP_CAN object
MCP_CAN CAN(CS_PIN);

// CAN ID to send and receive messages
const long CAN_ID = 0x18DB33F1;

void setup() {
  Serial.begin(115200);

  // Initialize CAN communication at 500 kbps (adjust as needed)
  if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("CAN Initialized Successfully!");
  } else {
    Serial.println("CAN Initialization Failed.");
    while (1);
  }

  // Set the MCP2515 in normal mode to communicate
  CAN.setMode(MCP_NORMAL);
}

void sendCANMessage() {
  byte data[] = {0x02, 0x10, 0x03}; // Example data for PID request (0x010C = Engine RPM)
  byte len = sizeof(data);

  // Send message to the specific CAN ID
  if (CAN.sendMsgBuf(CAN_ID, 0, len, data) == CAN_OK) {
    Serial.println("Message Sent Successfully");
  } else {
    Serial.println("Error Sending Message");
  }
}

void receiveCANMessage() {
  long unsigned int rxId;
  unsigned char len = 0;
  unsigned char rxBuf[8];

  // Check if data is received
  if (CAN.checkReceive() == CAN_MSGAVAIL) {
    // Read the message
    CAN.readMsgBuf(&rxId, &len, rxBuf);

    // Check if the response is from the expected CAN ID
    
      Serial.print("Received response with CAN ID: 0x");
      Serial.println(rxId, HEX);

      Serial.print("Data: ");
      for (int i = 0; i < len; i++) {
        Serial.print(rxBuf[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
    
  }
}


void loop() {
  sendCANMessage();   // Send the message
  delay(5000);        // Wait for the response
  receiveCANMessage(); // Check for a response
}
