//
// Created by Ellie Hussey on 11/11/24.
//

#include "main.h"
#include "../include/power_saver.h"
#include "../include/ota_helper.h"
#include <Arduino.h>

void setup(){
    #if SERIAL_DEBUG
        Serial.begin(115200);
    #endif
    delay(1000); //Take some time to open up the Serial Monitor

    // Set up the user LED
    pinMode(GPIO_NUM_15, OUTPUT);
    digitalWrite(GPIO_NUM_15, HIGH);

    // Initialize power saver
    power_saver_init();
    pinMode(POWER_SAVER_WAKE_PIN, INPUT_PULLDOWN);

    // Read and print the battery voltage (to 3 decimal places)
    #if SERIAL_DEBUG
        Serial.println(get_battery_voltage(), 3);
    #endif

    // Set up OTA enable pin
    pinMode(OTA_ENABLE_PIN, INPUT_PULLDOWN);
    // Set up OTA cancel pin
    pinMode(OTA_CANCEL_PIN, INPUT_PULLDOWN);

    // If the OTA enable pin is high, start listening for OTA updates
    if (digitalRead(OTA_ENABLE_PIN)) {
        // Initialize OTA functionality
        ota_helper_init(OTA_WIFI_SSID, OTA_WIFI_PASS);
        ota_helper_begin();
    } else {
        // If we're not in OTA mode, do stuff
        do_stuff();
    }
}

void do_stuff() {
    enter_deep_sleep();
}

void loop(){
    ota_helper_loop();
    // If the OTA cancel pin goes high while waiting for OTA updates, reboot
    if (digitalRead(OTA_CANCEL_PIN)) {
        ESP.restart();
    }
}
