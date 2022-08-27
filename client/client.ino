#include<Wire.h>

const int kArduinoI2CAddress = 0x8;

#define SERIAL_PRINTF_MAX_BUFF      256
void serialPrintf(const char *fmt, ...);

const int kRedLedPin = 3;
const int kGreenLedPin = 5;
const int kBlueLedPin = 6;
const int kWhiteLedPin = 9;


typedef enum {
  COMMAND_UNDEFINED,
  COMMAND_HELLO = 2,
  COMMAND_SUCCESS,
  COMMAND_SET_COLOR,
} Command;

typedef struct {
  uint8_t len;
  Command command: 8;
  uint8_t payload[30];
} Message;

typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} RGBColor;

typedef struct {
  RGBColor rgb;
  uint8_t white;
} Color;


static Message response {};

void setup() {
  Serial.begin(9600);
  Serial.println("setup called");

  Wire.begin(kArduinoI2CAddress);
  Wire.onReceive(i2c_receive_callback);
  Wire.onRequest(i2c_request_callback);

  pinMode(kRedLedPin, OUTPUT);
  pinMode(kGreenLedPin, OUTPUT);
  pinMode(kBlueLedPin, OUTPUT);
  pinMode(kWhiteLedPin, OUTPUT);
}

void loop() {
}

static void i2c_receive_callback(int bytes_received) {
  serialPrintf("bytes_received: %i", bytes_received);

  Message *msg = (Message *)calloc(1, bytes_received);

  uint8_t *buffer_ptr = (uint8_t *)msg;
  int bytes_read = 0;
  while (Wire.available() && (++bytes_read <= bytes_received)) {
    *buffer_ptr++ = Wire.read();
    serialPrintf("Read byte %i", bytes_read);
  }

  serialPrintf("Payload len is %i", msg->len);

  process_message(msg);
  free(msg);
}

static void i2c_request_callback() {
  Wire.write((byte *)&response, sizeof(uint8_t) + 1 + response.len);
}

static void process_message(Message *message) {
  switch (message->command) {
    case COMMAND_HELLO:
      response.len = 0;
      response.command = COMMAND_SUCCESS;
      break;
    case COMMAND_SET_COLOR:
      set_color((Color *)message->payload);
      break;
    default:
      serialPrintf("Got undefined command: %i", message->command);
      break;
  }
}

static void set_color(Color *color) {
  serialPrintf("Set color with red = %i, green = %i, blue = %i, white = %i",
               color->rgb.red, color->rgb.green, color->rgb.blue, color->white);

  analogWrite(kRedLedPin, color->rgb.red);
  analogWrite(kGreenLedPin, color->rgb.green);
  analogWrite(kBlueLedPin, color->rgb.blue);
  analogWrite(kWhiteLedPin, color->white);
}

void serialPrintf(const char *fmt, ...) {
  char buff[SERIAL_PRINTF_MAX_BUFF];

  va_list pargs;
  va_start(pargs, fmt);
  vsnprintf(buff, SERIAL_PRINTF_MAX_BUFF, fmt, pargs);
  va_end(pargs);

  Serial.println(buff);
}
