//
// Created by Ellie Hussey on 11/11/24.
//

#include <Arduino.h>
#include "power_saver.h"

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

void power_saver_init() {
    /** Handle wake-up from Deep Sleep **/
    // Increment boot number every reboot
    ++bootCount;

    // Print the wakeup reason for ESP32
    Serial.println("Boot number: " + String(bootCount));
    print_wakeup_reason();

    #ifdef POWER_SAVER_BATT_ADC_PIN
        /** Setup for reading battery voltage **/
        pinMode(POWER_SAVER_BATT_ADC_PIN, INPUT);
    #endif
}

double get_battery_voltage() {
    #ifdef POWER_SAVER_BATT_ADC_PIN
        uint32_t v_batt = 0;
        for(int i = 0; i < 16; i++) {
            v_batt += analogReadMilliVolts(POWER_SAVER_BATT_ADC_PIN);    // Read and accumulate ADC voltage
        }
        return 2.0 * v_batt / 16 / 1000.0;   // Adjust for 1:2 divider and convert to volts
    #else
        return -1.f;
    #endif
}

void enter_deep_sleep() {
    #ifdef POWER_SAVER_WAKE_PIN_MASK
        esp_sleep_enable_ext1_wakeup(POWER_SAVER_WAKE_PIN_MASK, ESP_EXT1_WAKEUP_ANY_HIGH);
    #endif

    //Go to sleep now
    Serial.println("Going to sleep now");
    esp_deep_sleep_start();
    Serial.println("This will never be printed");
}

void print_wakeup_reason(){
    esp_sleep_wakeup_cause_t wakeup_reason;

    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch(wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
        case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
        case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
        case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
        default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
    }
}
