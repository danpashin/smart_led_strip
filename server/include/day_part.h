//
// Created by danpashin on 8/27/22.
//

#ifndef SMART_LED_DAY_PART_H
#define SMART_LED_DAY_PART_H

namespace smart_led {
    enum DayPart {
        /// From 00::00 to 00:07
        Night,
        /// From 07:00 to 09::00
        Morning,
        /// From 09::00 to 22:00
        Day,
        /// From 22::00 to 00:00
        Evening,
    };

    /*!
     * @return Current part of the day
     */
    DayPart CurrentDayPart();
}

#endif //SMART_LED_DAY_PART_H
