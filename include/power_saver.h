//
// Created by Ellie Hussey on 11/11/24.
//

#ifndef POWER_SAVER_H
#define POWER_SAVER_H

// By default, use battery read pin for Seeed Studio XIAO ESP32C6
#define POWER_SAVER_BATT_ADC_PIN A0

// Define a specific wake pin to monitor for exiting deep sleep
#ifndef POWER_SAVER_WAKE_PIN
    #define POWER_SAVER_WAKE_PIN GPIO_NUM_1
#endif

// Define a specific wake pin bitmask
#define POWER_SAVER_WAKE_PIN_MASK (1ULL << POWER_SAVER_WAKE_PIN)

void power_saver_init();
double get_battery_voltage();
void enter_deep_sleep();
void print_wakeup_reason();

#endif //POWER_SAVER_H
