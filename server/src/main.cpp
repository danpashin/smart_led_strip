//
// Created by danpashin on 8/27/22.
//

#include <iostream>
#include "arduino.h"

using namespace smart_led;

[[noreturn]] static void performOperations(Arduino &arduino) {
    bool wasOffline = false;
    do {
        try {
            if (!arduino.CheckConnect()) {
                std::cerr << "Arduino did not respond at address 0x"
                          << std::hex << static_cast<int>(arduino.address())
                          << std::endl;
                wasOffline = true;
                sleep(8);
                continue;
            }

            arduino.SetDayPartColor(wasOffline);
            wasOffline = false;
        } catch (std::runtime_error &ex) {
            std::cerr << "got runtime exception, ignoring... " << ex.what() << std::endl;

            wasOffline = true;
            sleep(8);
        } catch (...) {
            std::cerr << "got exception, ignoring..." << std::endl;
        }
        sleep(2);
    } while (true);
}

int main() {
    Arduino arduino = Arduino(0x8);
    performOperations(arduino);

    return EXIT_SUCCESS;
}
