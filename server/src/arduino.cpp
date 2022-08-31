//
// Created by danpashin on 8/27/22.
//

#include "arduino.h"
#include "helper.h"

#include <mutex>
#include <cstring>

using namespace smart_led;

Arduino::Arduino(uint8_t address)
        : _bus(Pi2c(static_cast<int>(address), false)),
          _address(address) {
}

bool Arduino::RequestIsOk() noexcept(false) {
    Message response{};
    const int bytes_count = this->_bus.i2cRead(reinterpret_cast<char *>(&response), sizeof(response));
    if (bytes_count != sizeof(response)) {
        throw std::runtime_error(
                string_format("I2C received %i bytes, expected %i; error = %s", bytes_count, sizeof(response),
                              std::strerror(errno))
        );
    }

    return response.command == SUCCESS;
}

bool Arduino::CheckConnect() noexcept(false) {
    std::lock_guard lock(this->_busWriteMutex);
    Message msg{};
    msg.command = HELLO_QUERY;

    const int bytes_count = this->_bus.i2cWrite(reinterpret_cast<char *>(&msg), msg.length);
    if (bytes_count != msg.length) {
        throw std::runtime_error(
                string_format("I2C send %i bytes, expected %i; error = %s", bytes_count, sizeof(msg.length),
                              std::strerror(errno))
        );
    }

    return this->RequestIsOk();
}

bool Arduino::SetColor(Color color) {
    std::lock_guard lock(this->_busWriteMutex);
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

    const int bytes_count = this->_bus.i2cWrite(reinterpret_cast<char *>(&msg), msg.length);
    if (bytes_count != msg.length) {
        throw std::runtime_error(
                string_format("I2C send %i bytes, expected %i; error = %s", bytes_count, sizeof(msg.length),
                              std::strerror(errno))
        );
    }

    return this->RequestIsOk();
}

bool Arduino::SetDayPartColor(bool force) noexcept(false) {
    static bool cachedAnyoneAtHome = false;
    const bool anyoneAtHome = this->AnyoneAtHome();

    if (!anyoneAtHome) {
        if (anyoneAtHome != cachedAnyoneAtHome) {
            cachedAnyoneAtHome = anyoneAtHome;
            this->SetColor(Color::vantaBlack());
        }

        return true;
    }

    if (anyoneAtHome != cachedAnyoneAtHome) {
        force = true;
        cachedAnyoneAtHome = anyoneAtHome;
    }

    const DayPart curDayPart = CurrentDayPart();
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

void Arduino::SetAnyoneAtHome(bool anyoneAtHome) {
    std::lock_guard lock(this->_anyoneAtHomeMutex);
    this->_anyoneAtHome = anyoneAtHome;
}

bool Arduino::AnyoneAtHome() {
    std::shared_lock lock(this->_anyoneAtHomeMutex);
    return this->_anyoneAtHome;
}
