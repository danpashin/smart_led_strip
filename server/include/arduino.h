//
// Created by danpashin on 8/27/22.
//

#ifndef SMART_LED_ARDUINO_H
#define SMART_LED_ARDUINO_H

#include <cinttypes>
#include <shared_mutex>

#include "pi2c.h"
#include "message.h"
#include "color.h"

namespace smart_led {

    struct Arduino {
    public:
        explicit Arduino(uint8_t address);

        bool CheckConnect() noexcept(false);

        bool SetColor(Color color) noexcept(false);

        bool SetDayPartColor(bool force) noexcept(false);

        [[nodiscard]] uint8_t address() const;

        void SetAnyoneAtHome(bool anyoneAtHome);

        bool AnyoneAtHome();

    protected:
        uint8_t _address;
        Pi2c _bus;
        mutable std::shared_mutex _busWriteMutex;

        mutable DayPart dayPart = DayPart::Night;

        mutable bool _anyoneAtHome = true;
        mutable std::shared_mutex _anyoneAtHomeMutex;

        bool RequestIsOk() noexcept(false);

    };
}

#endif //SMART_LED_ARDUINO_H
