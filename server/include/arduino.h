//
// Created by danpashin on 8/27/22.
//

#ifndef SMART_LED_ARDUINO_H
#define SMART_LED_ARDUINO_H

#include <cinttypes>
#include "pi2c.h"
#include "message.h"
#include "color.h"

namespace smart_led {

    struct Arduino {
    public:
        explicit Arduino(uint8_t address);

        bool CheckConnect() noexcept(false);

        bool SetColor(Color &color) noexcept(false);

        bool SetDayPartColor(bool force) noexcept(false);

        uint8_t address() const;

    protected:
        uint8_t _address;
        Pi2c bus;

        DayPart dayPart;

        bool RequestIsOk() noexcept(false);
    };
}

#endif //SMART_LED_ARDUINO_H
