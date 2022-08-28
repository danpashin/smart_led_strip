//
// Created by danpashin on 8/28/22.
//

#ifndef SMART_LED_COLOR_H
#define SMART_LED_COLOR_H

struct Color {
  struct RGBColor {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
  } rgb;
  uint8_t white;
};


#endif //SMART_LED_COLOR_H
