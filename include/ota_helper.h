//
// Created by Ellie Hussey on 11/11/24.
//

#ifndef OTA_HELPER_H
#define OTA_HELPER_H

// By default, use the onboard chip antenna instead of the antenna connector
#ifndef OTA_HELPER_USE_CHIP_ANTENNA
    #define OTA_HELPER_USE_CHIP_ANTENNA true
#endif

// By default, use radio enable/select pins for Seeed Studio XIAO ESP32C6
#ifndef OTA_HELPER_RADIO_ENABLE_PIN
    #define OTA_HELPER_RADIO_ENABLE_PIN GPIO_NUM_3
#endif

#ifndef OTA_HELPER_RADIO_SELECT_PIN
    #define OTA_HELPER_RADIO_SELECT_PIN GPIO_NUM_14
#endif

void ota_helper_init(const char* ssid, const char* key);
void ota_helper_begin();
void ota_helper_end();
void ota_helper_loop();

#endif //OTA_HELPER_H
