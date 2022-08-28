//
// Created by danpashin on 8/27/22.
//
#include "color.h"

using namespace smart_led;

Color Color::morning() {
    return Color{
            .rgb = RGBColor{
                    .red = 150,
                    .green = 35,
                    .blue = 2
            },
            .white = 0,
    };
}

Color Color::daylight() {
    return Color{
            .rgb = RGBColor{
                    .red = 255,
                    .green = 80,
                    .blue = 7
            },
            .white = 255,
    };
}

Color Color::evening() {
    return Color{
            .rgb = RGBColor{
                    .red = 60,
                    .green = 20,
                    .blue = 1
            },
            .white = 0,
    };
}

Color Color::night() {
    return Color{
            .rgb = RGBColor{
                    .red = 0,
                    .green = 0,
                    .blue = 0
            },
            .white = 0,
    };
}

