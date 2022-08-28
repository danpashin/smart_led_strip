//
// Created by danpashin on 8/26/22.
//

#include<Wire.h>

#include "color.h"
#include "helper.h"
#include "message.h"

const int kArduinoI2CAddress = 0x8;

const int kRedLedPin = 3;
const int kGreenLedPin = 5;
const int kBlueLedPin = 6;
const int kWhiteLedPin = 9;

static Message request {};
static Message response {};

void setup() {
  Serial.begin(9600);
  Serial.println("setup called");

  Wire.begin(kArduinoI2CAddress);
  Wire.onReceive(I2CReceiveCallback);
  Wire.onRequest(I2CRequestCallback);

  pinMode(kRedLedPin, OUTPUT);
  pinMode(kGreenLedPin, OUTPUT);
  pinMode(kBlueLedPin, OUTPUT);
  pinMode(kWhiteLedPin, OUTPUT);
}

void loop() { }

static void I2CReceiveCallback(int bytesReceived) {
  SerialPrintf("bytes_received: %i", bytesReceived);

  uint8_t *bufferPtr = (uint8_t *)&request;
  int bytesRead = 0;
  
  while (Wire.available() && (++bytesRead <= bytesReceived)) {
    *bufferPtr++ = Wire.read();
  }

  ProcessMessage(request);
}

static void I2CRequestCallback() {
  Wire.write((uint8_t *)&response, Message::HeaderLength() + response.len);
}

static void ProcessMessage(Message message) {
  switch (message.command) {
    case COMMAND_HELLO:
      response.len = 0;
      response.command = COMMAND_SUCCESS;
      break;
    case COMMAND_SET_COLOR:
      SetColor((Color *)message.payload);
      response.len = 0;
      response.command = COMMAND_SUCCESS;
      break;
    default:
      SerialPrintf("Got undefined command: %i", message.command);
      break;
  }
}

static void SetColor(Color *color) {
  analogWrite(kRedLedPin, color->rgb.red);
  analogWrite(kGreenLedPin, color->rgb.green);
  analogWrite(kBlueLedPin, color->rgb.blue);
  analogWrite(kWhiteLedPin, color->white);

  SerialPrintf("Set color with red = %i, green = %i, blue = %i, white = %i",
               color->rgb.red, color->rgb.green, color->rgb.blue, color->white);
}
