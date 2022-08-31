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
        /*!
         * @brief Constructs struct instance with specified address
         *
         * @param[in] address Address on which Arduino is connected via I2C bus
         */
        explicit Arduino(uint8_t address);

        /*!
         * @brief Checks connect with the Arduino
         *
         * @throws std::runtime_exception with detailed message
         * if there was an error when reading or writing data
         *
         * @return true if Arduino responded with success. Otherwise, false.
         */
        bool CheckConnect() noexcept(false);

        /*!
         * @brief Sets color to the LED strips
         *
         * @param[in] color Color which will be send to Arduino
         *
         * @throws std::runtime_exception with detailed message
         * if there was an error when reading or writing data
         *
         * @return true if color was set. Otherwise, false
         */
        bool SetColor(Color color) noexcept(false);

        /*!
         * @brief Sets color matching to the current part of the day
         *
         * @details Additionally checks if anyone is at home
         * and powers off all LED strips while there's no one.
         *
         * @param[in] force Flag which indicates if color needs to be set
         * even it is already set
         *
         * @throws std::runtime_exception with detailed message
         * if there was an error when reading or writing data
         *
         * @return true if color was set. Otherwise, false
         */
        bool SetDayPartColor(bool force) noexcept(false);

        /*!
         * @return Address on which Arduino is connected via I2C bus
         */
        uint8_t address() const;

        /*!
         * @param[in] anyoneAtHome Flag indicating if there is anyone at home
         */
        void SetAnyoneAtHome(bool anyoneAtHome);

        /*!
         * @return Flag indicating if there is anyone at home
         */
        bool AnyoneAtHome();

    protected:
        uint8_t _address;
        Pi2c _bus;
        mutable std::shared_mutex _busWriteMutex;

        mutable DayPart dayPart = DayPart::Night;

        mutable bool _anyoneAtHome = true;
        mutable std::shared_mutex _anyoneAtHomeMutex;

        /*!
         * @brief Receives response from Arduino and checks if it is valid
         *
         * @throws std::runtime_exception with detailed message
         * if there was an error when reading or writing data
         *
         * @return true if response was OK. Otherwise, false
         */
        bool RequestIsOk() noexcept(false);
    };
}

#endif //SMART_LED_ARDUINO_H
