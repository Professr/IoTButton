//
// Example implementation of an IoT button with OTA update support
// Built using the Seeed Studio XIAO ESP32C6 module
//

#include "main.h"

#include "config.h"
#include "utils.h"
#include "power_helper.h"
#include "menu_helper.h"
#include "radio_helper.h"

#include <string>

#include <Arduino.h>

enum OperatingState {
    NORMAL,
    MENU,
};

OperatingState operatingState;
unsigned long bootMillis;

void setup(){
    bootMillis = millis();
    SERIAL_DEBUG.begin(115200);
    delay(1000); //Take some time to open up the Serial Monitor

    // Set up the menu enable pin
    pinMode(MENU_BUTTON_PIN, INPUT_PULLDOWN);

    // Initialize utility functions and blink user LED once
    utils_init();
    utils_blink_user_led(1, 100);

    // Initialize power saver
    power_helper_init();

    // Read and print the battery voltage (to 3 decimal places)
    SERIAL_DEBUG.println(get_battery_voltage(), 3);

    // If the menu button pin is high, start the menu handler
    if (digitalRead(MENU_BUTTON_PIN)) {
        operatingState = OperatingState::MENU;
        menu_init();
    } else {
        // If we're not in menu mode, do normal init
        operatingState = OperatingState::NORMAL;
        normal_init();
    }
}

void menu_init() {
    while(digitalRead(MENU_BUTTON_PIN)) {
        utils_blink_user_led(1, 50);
    }
    menu_helper_init(MENU_BUTTON_PIN, BACK_BUTTON_PIN);
}

void normal_init() {
    // Normal IoT button operation - connect to Wi-Fi, make an HTTP request, go to sleep
    const unsigned long millisecondsSinceWake = millis() - bootMillis;
    pinMode(DEEP_SLEEP_WAKE_PIN, INPUT_PULLDOWN);

    radio_helper_init(WIFI_SSID, WIFI_PASS);
    radio_helper_http_get(IOT_BUTTON_EVENT_URL, {
            {"vBatt", std::to_string(get_battery_voltage())},
            {"eventType", std::to_string(esp_sleep_get_wakeup_cause())},
            {"msSinceEvent", std::to_string(millisecondsSinceWake)},
    });

    utils_blink_user_led(5, 100);

    enter_deep_sleep_wake_on_pin(DEEP_SLEEP_WAKE_PIN);
}

void loop(){
    switch (operatingState) {
        case OperatingState::MENU:
            menu_helper_loop();
            break;
        case OperatingState::NORMAL:
        default:
            break;
    }
}
