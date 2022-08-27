//
// Created by danpashin on 8/27/22.
//

#include "arduino.h"
#include <cstring>

using namespace smart_led;

Arduino::Arduino(uint8_t address)
        : bus(Pi2c(static_cast<int>(address), false)),
          address(address) {
}

bool Arduino::RequestIsOk() noexcept(false) {
    Message response{};
    const int bytes_count = bus.i2cRead(reinterpret_cast<char *>(&response), sizeof(response));
    if (bytes_count != sizeof(response)) {
        throw std::runtime_error("Bus read error");
    }

    return response.command == SUCCESS;
}

bool Arduino::CheckConnect() noexcept(false) {
    Message msg{};
    msg.command = HELLO_QUERY;

    const int bytes_count = bus.i2cWrite(reinterpret_cast<char *>(&msg), msg.length);
    if (bytes_count != msg.length) {
        throw std::runtime_error("Bus write error");
    }

    return this->RequestIsOk();
}

bool Arduino::SetColor(Color &color) {
    Message msg{};
    msg.command = SET_COLOR;

    if (color.rgb) {
        msg.length += 3;
        memcpy(msg.payload, &*color.rgb, sizeof(RGBColor));
    }

    if (color.white) {
        msg.length += 1;
        msg.payload[3] = *color.white;
    }

    const int bytes_count = bus.i2cWrite(reinterpret_cast<char *>(&msg), msg.length);
    if (bytes_count != msg.length) {
        throw std::runtime_error("Bus write error");
    }

    return this->RequestIsOk();
}
