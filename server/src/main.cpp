//
// Created by danpashin on 8/27/22.
//

#include <iostream>
#include "arduino.h"

using namespace smart_led;

const uint8_t kArduinoAddress = 0x8;

int main() {
    Arduino arduino = Arduino(kArduinoAddress);
    if (!arduino.CheckConnect()) {
        std::cerr << "Arduino did not respond at address 0x"
                  << std::hex << static_cast<int>(arduino.address)
                  << std::endl;
        return EXIT_FAILURE;
    }

    auto color = Color{
            .rgb = RGBColor{
                    .red = 255,
                    .green = 80,
                    .blue = 6
            },
            .white = 255,
    };

    arduino.SetColor(color);

    return EXIT_SUCCESS;
}
