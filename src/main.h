//
// Created by Ellie Hussey on 11/11/24.
//

#ifndef DEEPSLEEPOTA_MAIN_H
#define DEEPSLEEPOTA_MAIN_H

#define SERIAL_DEBUG true

#define OTA_WIFI_SSID "AllMyApesGone"
#define OTA_WIFI_PASS "Pr4hr4nda"

#define POWER_SAVER_WAKE_PIN D1
#define OTA_CANCEL_PIN D1
#define OTA_ENABLE_PIN D2

void setup();
void do_stuff();
void loop();

#endif //DEEPSLEEPOTA_MAIN_H
