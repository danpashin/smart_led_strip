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
        uint8_t address;

        explicit Arduino(uint8_t address);

        bool CheckConnect() noexcept(false);

        bool SetColor(Color &color) noexcept(false);

    protected:
        Pi2c bus;

        bool RequestIsOk() noexcept(false);
    };
}

#endif //SMART_LED_ARDUINO_H
