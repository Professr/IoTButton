//
// Helper for power-related functionality (deep sleep, battery voltage, etc)
//

#include "power_helper.h"
#include "config.h"
#include "utils.h"

#include <Arduino.h>


/*
Hardware Connections
======================
Push Button to GPIO 0 pulled down with a 10K Ohm
resistor

NOTE:
======
Bit mask of GPIO numbers which will cause wakeup. Only GPIOs
which have RTC functionality can be used in this bit map.
For different SoCs, the related GPIOs are:
- ESP32: 0, 2, 4, 12-15, 25-27, 32-39
- ESP32-S2: 0-21
- ESP32-S3: 0-21
- ESP32-C6: 0-7
- ESP32-H2: 7-14
*/

RTC_DATA_ATTR int bootCount = 0;

void power_helper_init() {
    /** Handle wake-up from Deep Sleep **/
    // Increment boot number every reboot
    ++bootCount;

    // Print the wakeup reason for ESP32
    Serial.println("Boot number: " + String(bootCount));
    print_wakeup_reason();

    #ifdef BATT_ADC_PIN
        /** Setup for reading battery voltage **/
        pinMode(BATT_ADC_PIN, INPUT);
    #endif
}

double get_battery_voltage() {
    #ifdef BATT_ADC_PIN
        uint32_t v_batt = 0;
        for(int i = 0; i < 16; i++) {
            v_batt += analogReadMilliVolts(BATT_ADC_PIN);    // Read and accumulate ADC voltage
        }
        return 2.0 * v_batt / 16 / 1000.0;   // Adjust for 1:2 divider and convert to volts
    #else
        return 0.f;
    #endif
}

void enter_deep_sleep_wake_on_pin(uint8_t wake_pin) {
    enter_deep_sleep_wake_on_mask((1ULL << wake_pin));
}

void enter_deep_sleep_wake_on_mask(uint64_t wake_pin_mask) {
    esp_sleep_enable_ext1_wakeup(wake_pin_mask, ESP_EXT1_WAKEUP_ANY_HIGH);

    //Go to sleep now
    SERIAL_DEBUG.println("Going to sleep now");
    esp_deep_sleep_start();
    SERIAL_DEBUG.println("This will never be printed");
}

void print_wakeup_reason(){
    const esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

    switch(wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_EXT0 : SERIAL_DEBUG.println("Wakeup caused by external signal using RTC_IO"); break;
        case ESP_SLEEP_WAKEUP_EXT1 : SERIAL_DEBUG.println("Wakeup caused by external signal using RTC_CNTL"); break;
        case ESP_SLEEP_WAKEUP_TIMER : SERIAL_DEBUG.println("Wakeup caused by timer"); break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD : SERIAL_DEBUG.println("Wakeup caused by touchpad"); break;
        case ESP_SLEEP_WAKEUP_ULP : SERIAL_DEBUG.println("Wakeup caused by ULP program"); break;
        default : SERIAL_DEBUG.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
    }
}
