
enum Command : uint8_t {
  COMMAND_UNDEFINED,
  COMMAND_HELLO = 2,
  COMMAND_SUCCESS,
  COMMAND_SET_COLOR,
};

struct Message {
  uint8_t len;
  Command command;
  uint8_t payload[30];

  static uint8_t HeaderLength() {
    return sizeof(uint8_t) + sizeof(Command);
  }
};
