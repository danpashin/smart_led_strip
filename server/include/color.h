//
// Created by danpashin on 8/27/22.
//

#ifndef SMART_LED_COLOR_H
#define SMART_LED_COLOR_H

#include <cstdint>
#include <optional>
#include "day_part.h"

namespace smart_led {
    struct RGBColor {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };

    struct Color {
    public:
        std::optional<RGBColor> rgb = std::nullopt;
        std::optional<uint8_t> white = std::nullopt;

        static Color daylight();

        static Color evening();

        static Color night();
    };
}

#endif //SMART_LED_COLOR_H
