//
// Created by danpashin on 8/27/22.
//

#ifndef SMART_LED_DAY_PART_H
#define SMART_LED_DAY_PART_H

namespace smart_led {
    enum DayPart {
        Night,
        Morning,
        Day,
        Evening,
    };

    DayPart CurrentDayPart();
}

#endif //SMART_LED_DAY_PART_H
