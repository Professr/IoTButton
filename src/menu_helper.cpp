//
// Helper for managing a "menu mode"
//

#include "config.h"
#include "utils.h"
#include "menu_helper.h"
#include "radio_helper.h"

#include <Arduino.h>
#include "Button2.h"

Button2 btn_menu;
Button2 btn_back;

enum MenuState {
    MAIN,
    OTA,
};

MenuState menuState = MenuState::MAIN;

void menu_helper_init(uint8_t menu_button_pin, uint8_t back_button_pin) {
    // Set up menu button handler
    btn_menu.begin(menu_button_pin, INPUT_PULLDOWN, false);
    btn_menu.setDoubleClickTime(500);
    btn_menu.setClickHandler(menu_click_handler);
    btn_menu.setDoubleClickHandler(menu_click_handler);

    // Set up back button handler
    btn_back.begin(back_button_pin, INPUT_PULLDOWN, false);
    btn_menu.setDoubleClickTime(500);
    btn_back.setClickHandler(back_click_handler);
    btn_back.setDoubleClickHandler(back_click_handler);

    // Set up the user LED and blink it a few times
    utils_init();
    utils_blink_user_led(3, 100);
}

void menu_helper_loop() {
    switch(menuState) {
        case MenuState::OTA:
            radio_helper_ota_loop();
            break;
        default:
            break;
    }
    btn_menu.loop();
    btn_back.loop();
}

void menu_click_handler(Button2& btn) {
    switch(btn.getType()) {
        case single_click:
            switch(menuState) {
                case MenuState::MAIN:
                case MenuState::OTA:
                default:
                    break;
            }
            break;
        case double_click:
            switch (menuState) {
                case MenuState::MAIN:
                    // Double-clicking menu at the root level should start OTA mode
                    menuState = MenuState::OTA;
                    utils_blink_user_led(3, 100);
                    radio_helper_init(WIFI_SSID, WIFI_PASS);
                    radio_helper_ota_init(OTA_DNS_NAME);
                    radio_helper_ota_begin();
                    break;
                case MenuState::OTA:
                default:
                    break;
            }
        default:
            break;
    }
}

void back_click_handler(Button2& btn) {
    switch(btn.getType()) {
        case single_click:
            switch(menuState) {
                case MenuState::MAIN:
                case MenuState::OTA:
                default:
                    break;
            }
            break;
        case double_click:
            // Double click of the back button should reboot
            switch(menuState) {
                case MenuState::MAIN:
                case MenuState::OTA:
                    // Before reboot, cleanly end any OTA that's running
                    radio_helper_ota_end();
                default:
                    ESP.restart();
                    break;
            }
            break;
        default:
            break;
    }
}
