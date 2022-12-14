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

    if (hour >= 7 && hour <= 8) {
        return DayPart::Morning;
    }

    if (hour >= 9 && hour <= 21) {
        return DayPart::Day;
    }

    if (hour >= 22 && hour <= 23) {
        return DayPart::Evening;
    }

    return DayPart::Night;
}
