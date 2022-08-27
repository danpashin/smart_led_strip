//
// Created by danpashin on 8/27/22.
//

#include <ctime>
#include "day_part.h"

using namespace smart_led;

DayPart smart_led::CurrentDayPart() {
    time_t timestamp = time(nullptr);
    tm *local_time = localtime(&timestamp);
    const int hour = local_time->tm_hour;

    if (hour >= 6 && hour <= 9) {
        return DayPart::Morning;
    }

    if (hour >= 10 && hour <= 19) {
        return DayPart::Day;
    }

    if (hour >= 20 && hour <= 21) {
        return DayPart::Evening;
    }

    return DayPart::Night;
}
