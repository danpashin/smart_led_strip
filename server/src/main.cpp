//
// Created by danpashin on 8/27/22.
//

#include <iostream>
#include "arduino.h"

using namespace smart_led;

const uint8_t kArduinoAddress = 0x8;

[[noreturn]] static void performOperations(Arduino &arduino) {
    bool wasOffline = false;
    do {
        if (!arduino.CheckConnect()) {
            std::cerr << "Arduino did not respond at address 0x"
                      << std::hex << static_cast<int>(arduino.address)
                      << std::endl;
            wasOffline = true;
            sleep(5);
            continue;
        }

        arduino.SetDayPartColor(wasOffline);
        sleep(2);
    } while (true);
}

int main() {
    Arduino arduino = Arduino(kArduinoAddress);
    try {
        performOperations(arduino);
    } catch (...) {
        std::cerr << "got an exception, ignoring..." << std::endl;
    }

    return EXIT_SUCCESS;
}
