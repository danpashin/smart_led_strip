//
// Created by danpashin on 8/27/22.
//

#ifndef SMART_LED_COLOR_H
#define SMART_LED_COLOR_H

#include <cstdint>
#include <optional>
#include "day_part.h"

namespace smart_led {
    /*!
     * @brief Color containing three components - red, green and blue
     */
    struct RGBColor {
        /// Red part of the color. Can be from 0 to 255
        uint8_t red;
        /// Green part of the color. Can be from 0 to 255
        uint8_t green;
        /// Blue part of the color. Can be from 0 to 255
        uint8_t blue;
    };

    /*!
     * @brief Color containing four components - RGB and white (I have both LED strips)
     */
    struct Color {
        /// RGB component of the color
        std::optional<RGBColor> rgb = std::nullopt;

        /// White component of the color
        std::optional<uint8_t> white = std::nullopt;

        /// Color is being used at morning
        static Color morning();

        /// Color is being used at day
        static Color daylight();

        /// Color is being used at evening
        static Color evening();

        /// Color is being used at night. Currently is black - has no color
        static Color night();

        /// Just black color
        static Color vantaBlack();
    };
}

#endif //SMART_LED_COLOR_H
