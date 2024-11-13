//
// Define hardware pin mappings, connection options, and application-specific settings
//
#pragma once

#ifndef IoTButtonConfig_h

/** HEADER SETUP */
#define IoTButtonConfig_h
#ifndef Arduino_h
    #include <Arduino.h> // For pin constants and types
#endif
/** END HEADER SETUP */

/** CONFIGURATION */
// By default, disable serial debug (if we need OTA, we're probably not hooked up to a cable)
//#define ENABLE_SERIAL_DEBUG true

// By default, use the onboard chip antenna instead of the antenna connector
#define RADIO_USE_CHIP_ANTENNA true
// By default, use radio enable/select pins for Seeed Studio XIAO ESP32C6
#define RADIO_ENABLE_PIN GPIO_NUM_3
#define RADIO_SELECT_PIN GPIO_NUM_14

#define WIFI_SSID XXXXXXXXXXX
#define WIFI_PASS XXXXXXXXXXX
#define OTA_DNS_NAME "esp32ota" // Must match upload-port in platformio.ini
#define IOT_BUTTON_EVENT_URL XXXXXXXXXXX

// Define button pins for "menu mode"
#define BACK_BUTTON_PIN D2
#define MENU_BUTTON_PIN D1

// Define a specific wake pin to monitor for exiting deep sleep
#define DEEP_SLEEP_WAKE_PIN D2

// By default, use user LED pin for Seeed Studio XIAO ESP32C6
#define USER_LED_PIN GPIO_NUM_15

// By default, use battery read pin for Seeed Studio XIAO ESP32C6
#define BATT_ADC_PIN A0
/** END CONFIGURATION */

#endif //IoTButtonConfig_h