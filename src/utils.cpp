//
// Utility functions and reusable hardware abstractions
//
#include "config.h"

#ifndef Arduino_h
    #include <Arduino.h> // For pin constants and types
#endif

void utils_init() {
    pinMode(USER_LED_PIN, OUTPUT);
    digitalWrite(USER_LED_PIN, HIGH); // Turn user LED off
}

void utils_blink_user_led(uint8_t num_blinks, uint16_t blink_delay) {
    for(uint8_t i = 0; i < num_blinks; i++) {
        digitalWrite(USER_LED_PIN, LOW);
        delay(blink_delay);
        digitalWrite(USER_LED_PIN, HIGH);
        delay(blink_delay);
    }
}