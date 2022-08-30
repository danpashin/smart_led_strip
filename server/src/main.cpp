//
// Created by danpashin on 8/27/22.
//

#include <iostream>
#include <vector>
#include <thread>

#include "arduino.h"
#include "arp.h"

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

static void SendARPRequests(Arduino &arduino) {
    std::vector<const char *> hosts{
            "192.168.31.135", "192.168.31.136", "192.168.31.137"
    };
    slice_mut_char_const_ptr hosts_slice{hosts.data(), hosts.size()};

    int failsCount = 0;

    while (true) {
        bool found = arp_ping_multiple(hosts_slice, 10);
        if (found) {
            failsCount = 0;
            arduino.SetAnyoneAtHome(true);
        } else {
            failsCount++;
            
            if (failsCount > 10) {
                failsCount = 0;
                arduino.SetAnyoneAtHome(false);
            }
        }

        sleep(180);
    }
}

int main() {
    auto arduino = Arduino(0x8);

    std::thread pingThread(SendARPRequests, std::ref(arduino));
    pingThread.detach();

    performOperations(arduino);

    return EXIT_SUCCESS;
}
