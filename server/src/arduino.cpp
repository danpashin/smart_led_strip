//
// Created by danpashin on 8/27/22.
//

#include "arduino.h"
#include <cstring>

using namespace smart_led;

Arduino::Arduino(uint8_t address)
        : bus(Pi2c(static_cast<int>(address), false)),
          _address(address) {
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
        RGBColor rgb = *color.rgb;
        msg.payload[0] = rgb.red;
        msg.payload[1] = rgb.green;
        msg.payload[2] = rgb.blue;
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

bool Arduino::SetDayPartColor(bool force) noexcept(false) {
    DayPart curDayPart = CurrentDayPart();
    if (curDayPart == this->dayPart && !force) {
        return true;
    }
    this->dayPart = curDayPart;

    Color color;
    switch (CurrentDayPart()) {
        case Morning:
            color = Color::morning();
            break;
        case Day:
            color = Color::daylight();
            break;
        case Evening:
            color = Color::evening();
            break;
        case Night:
            color = Color::night();
            break;
    }

    return this->SetColor(color);
}

uint8_t Arduino::address() const {
    return this->_address;
}
