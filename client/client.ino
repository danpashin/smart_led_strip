//
// Created by danpashin on 8/26/22.
//

#include<Wire.h>

#include "color.h"
#include "helper.h"
#include "message.h"
#include <FadeLed.h>

const int kArduinoI2CAddress = 0x8;

// Red, green, blue and white
static FadeLed rgbwLeds[4] = {3, 5, 6, 9};

static Color currentColor;

static Message request {};
static Message response {};

void setup() {
  Serial.begin(9600);
  Serial.println("setup called");

  Wire.begin(kArduinoI2CAddress);
  Wire.setClock(400000);
  Wire.onReceive(I2CReceiveCallback);
  Wire.onRequest(I2CRequestCallback);

  FadeLed::setInterval(10);
  
  for (int i; i < sizeof(rgbwLeds) / sizeof(*rgbwLeds); i++) {
    rgbwLeds[i].noGammaTable();
    rgbwLeds[i].setTime(5000, true);
  }
}

void loop() {
  FadeLed::update();
}

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
      Color newColor = *(Color *)message.payload;
      if (newColor != currentColor) {
          currentColor = newColor;
          SetColor(newColor);
      }
      response.len = 0;
      response.command = COMMAND_SUCCESS;
      break;
    default:
      SerialPrintf("Got undefined command: %i", message.command);
      break;
  }
}

static void SetColor(Color color) {
  rgbwLeds[0].set(color.red);
  rgbwLeds[1].set(color.green);
  rgbwLeds[2].set(color.blue);
  rgbwLeds[3].set(color.white);

  SerialPrintf("Set color with red = %i, green = %i, blue = %i, white = %i",
               color.red, color.green, color.blue, color.white);
}
