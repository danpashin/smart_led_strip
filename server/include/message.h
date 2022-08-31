//
// Created by danpashin on 8/27/22.
//

#ifndef SMART_LED_MESSAGE_H
#define SMART_LED_MESSAGE_H

#include <cstdint>
#include <optional>

namespace smart_led {
    enum Command : uint8_t {
        UNDEFINED = 0,
        HELLO_QUERY = 2,
        SUCCESS,
        SET_COLOR,
    };

    struct Message {
        /// Size of message in bytes. Default is the size of header.
        uint8_t length = sizeof(uint8_t) * 2;
        /// Byte indicating which command is in the message. Has the size of 1 byte
        Command command = UNDEFINED;
        /// Payload with size of 30 bytes 'cause Arduino can't receive more
        [[maybe_unused]] uint8_t payload[30]{};
    };
}

#endif //SMART_LED_MESSAGE_H
