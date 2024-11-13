//
// Helper functions for managing the internal radio and Wi-Fi/networking
//
#pragma once

#ifndef RadioHelper_h

/** HEADER SETUP */
#define RadioHelper_h
#include <map>
#include <string>
/** END HEADER SETUP */

void radio_helper_init(const char* ssid, const char* key);
void radio_helper_http_get(const std::string& serverUrl, const std::map<std::string, std::string>& params = {});

void radio_helper_ota_init(const std::string& dnsName);
void radio_helper_ota_begin();
void radio_helper_ota_end();
void radio_helper_ota_loop();

#endif //RadioHelper_h