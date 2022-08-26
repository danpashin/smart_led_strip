#include<Wire.h>

#define SERIAL_PRINTF_MAX_BUFF      256
void serialPrintf(const char *fmt, ...);

const int kRedLedPin = 3;
const int kGreenLedPin = 5;
const int kBlueLedPin = 6;
const int kWhiteLedPin = 9;

typedef enum {
  COMMAND_SET_COLOR = 1,
} Command;

typedef struct {
  Command command: 8;
  uint8_t payload[31];
} Message;

typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t white;
} Color;

void setup() {
  Wire.begin(0x8);
  Wire.onReceive(i2c_receive_callback);

  pinMode(kRedLedPin, OUTPUT);
  pinMode(kGreenLedPin, OUTPUT);
  pinMode(kBlueLedPin, OUTPUT);
  pinMode(kWhiteLedPin, OUTPUT);

  Serial.begin(9600);
  serialPrintf("Accepting messages with size of %i bytes", sizeof(Message));
}

void loop() {
  delay(100);
}

void i2c_receive_callback(int len) {
  if (len != sizeof(Message)) {
    serialPrintf("Can't allocate buffer of size %i as it not equal %i", len, sizeof(Message));
    return;
  }

  Message msg;
  uint8_t *buffer_ptr = (uint8_t *)&msg;
  while (Wire.available()) {
    *buffer_ptr++ = Wire.read();
  }

  process_message(msg);
}

void process_message(Message message) {
  serialPrintf("Got command %i", message.command);
  switch (message.command) {
    case COMMAND_SET_COLOR:
      Color color;
      memcpy(&color, message.payload, sizeof(Color));
      set_color(color);
      break;
    default:
      serialPrintf("Got undefined command");
      break;
  }
}

void set_color(Color color) {
  serialPrintf("Set color with red = %i, green = %i, blue = %i, white = %i", color.red, color.green, color.blue, color.white);

  analogWrite(kRedLedPin, color.red);
  analogWrite(kGreenLedPin, color.green);
  analogWrite(kBlueLedPin, color.blue);
  analogWrite(kWhiteLedPin, color.white);
}

void serialPrintf(const char *fmt, ...) {
  char buff[SERIAL_PRINTF_MAX_BUFF];

  va_list pargs;
  va_start(pargs, fmt);
  vsnprintf(buff, SERIAL_PRINTF_MAX_BUFF, fmt, pargs);
  va_end(pargs);

  Serial.println(buff);
}
