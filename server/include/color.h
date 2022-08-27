//
// Created by danpashin on 8/27/22.
//

#ifndef SMART_LED_COLOR_H
#define SMART_LED_COLOR_H

#include <cstdint>
#include <optional>

namespace smart_led {
    struct RGBColor {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };

    struct Color {
        std::optional<RGBColor> rgb = std::nullopt;
        std::optional<uint8_t> white = std::nullopt;
    };
}

#endif //SMART_LED_COLOR_H
