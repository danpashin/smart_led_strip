//
// Created by danpashin on 8/27/22.
//

#include <iostream>
#include <vector>
#include <thread>

#include "arduino.h"
#include "arp.h"

using namespace smart_led;

/*! @brief Main endless loop
 *
 * @details Every 2 seconds pings Arduino and sets appropriate day's part color.
 * If there was connection error it resets current color to the days one.
 *
 * @details Ignores every underlying exceptions and print them to stderr.
 *
 * @param[in] arduino Arduino's instance which will be used to set color and etc
 *
 * @returns Never returns as runs as endless loop
 *
 * */
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


/*!
 * @brief Tries to "ping" devices in endless loop
 *
 * @details Consistently sends ARP requests and tries to get response on them.
 * If no device is responding after 10 attempts (with 10 seconds as ARP response receive timeout)
 * marks them as inactive in Arduino's instance.
 *
 * @details I could make this to use ICMP requests but iPhones
 * don't respond to them while being in sleep state. So the only way is to send ARP request.
 *
 * @param[in] arduino Arduino's instance which will be used to set device state.
 *
 * @returns Never returns as runs as endless loop
 */
[[noreturn]] static void SendARPPings(Arduino &arduino) {
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

        sleep(30);
    }
}

int main() {
    // 0x8 is the I2C address of arduino
    auto arduino = Arduino(0x8);

    std::thread pingThread(SendARPPings, std::ref(arduino));
    pingThread.detach();

    performOperations(arduino);

    // never be executed lol
    return EXIT_SUCCESS;
}
