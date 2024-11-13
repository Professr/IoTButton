//
// Helper for managing a "menu mode"
//
#pragma once

#ifndef MenuHelper_h

/** HEADER SETUP */
#define MenuHelper_h
#ifndef Arduino_h
    #include <Arduino.h> // For pin constants and types
#endif
#ifndef Button2_h
    #include "Button2.h"
#endif
/** END HEADER SETUP */

void menu_helper_init(uint8_t menuButtonPin, uint8_t backButtonPin);
void menu_helper_loop();

void menu_click_handler(Button2& btn);
void back_click_handler(Button2& btn);

#endif //MenuHelper_h