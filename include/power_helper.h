//
// Helper for power-related functionality (deep sleep, battery voltage, etc)
//
#pragma once

#ifndef PowerHelper_h

/** HEADER SETUP */
#define PowerHelper_h
#ifndef Arduino_h
    #include <Arduino.h> // For pin constants and types
#endif
/** END HEADER SETUP */

void power_helper_init();
double get_battery_voltage();
void enter_deep_sleep_wake_on_pin(uint8_t wakePin);
void enter_deep_sleep_wake_on_mask(uint64_t wakePinMask);
void print_wakeup_reason();

#endif //PowerHelper_h