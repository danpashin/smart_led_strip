//
// Created by danpashin on 8/28/22.
//

#ifndef SMART_LED_COLOR_H
#define SMART_LED_COLOR_H

struct Color {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t white;
};

bool operator == (const Color& lhs, const Color& rhs) {
    return lhs.red == rhs.red && lhs.green == rhs.green && lhs.blue == rhs.blue && lhs.white == rhs.white;
}

bool operator != (const Color& lhs, const Color& rhs) {
    return !(lhs == rhs);
}

#endif //SMART_LED_COLOR_H
