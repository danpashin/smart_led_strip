//
// Created by danpashin on 8/27/22.
//

#include <iostream>
#include <vector>
#include <thread>

#include "arduino.h"
#include "oping.h"

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

static void CheckDevices(Arduino &arduino) {
    pingobj_t *pinger = ping_construct();

    const std::vector<const char *> hosts{
            "192.168.31.135", "192.168.31.136", "192.168.31.137"
    };

    do {
        bool success = false;

        for (auto host: hosts) {
            if (ping_host_add(pinger, host) != 0) {
                continue;
            }

            const int pings = ping_send(pinger);
            ping_host_remove(pinger, host);

            success = pings > 0;
            std::cout << host << " is " << (success ? "online" : "offline") << std::endl;
            if (success) {
                break;
            }
        }
        arduino.SetAnyoneAtHome(success);

        sleep(60);
    } while (true);
}

int main() {
    Arduino arduino = Arduino(0x8);

    std::thread pingThread(CheckDevices, std::ref(arduino));
    pingThread.detach();

    performOperations(arduino);

    return EXIT_SUCCESS;
}
