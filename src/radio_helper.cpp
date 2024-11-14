//
// Helper functions for managing the internal radio and Wi-Fi/networking
//

#include "config.h"
#include "utils.h"
#include "radio_helper.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <NetworkUdp.h>
#include <ArduinoOTA.h>
#include <HTTPClient.h>

bool ota_helper_is_initialized = false;
bool ota_helper_is_running = false;

void radio_helper_init(const char* ssid, const char* key) {
    digitalWrite(USER_LED_PIN, HIGH);

    // Select the radio antenna
    pinMode(RADIO_SELECT_PIN, OUTPUT);
    digitalWrite(RADIO_SELECT_PIN, RADIO_USE_CHIP_ANTENNA ? LOW : HIGH);

    // Enable the radio system
    pinMode(RADIO_ENABLE_PIN, OUTPUT);
    digitalWrite(RADIO_ENABLE_PIN, HIGH);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, key);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }
}

void radio_helper_ota_init(const std::string& dnsName) {
    // Port defaults to 3232
    // ArduinoOTA.setPort(3232);

    // Hostname defaults to esp3232-[MAC]
    ArduinoOTA.setHostname(dnsName.c_str());

    // No authentication by default
    // ArduinoOTA.setPassword("admin");

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    ArduinoOTA
            .onStart([]() {
                String type;
                if (ArduinoOTA.getCommand() == U_FLASH) {
                    type = "sketch";
                } else {  // U_SPIFFS
                    type = "filesystem";
                }

                // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                SERIAL_DEBUG.println("Start updating " + type);
            })
            .onEnd([]() {
                SERIAL_DEBUG.println("\nEnd");
            })
            .onProgress([](unsigned int progress, unsigned int total) {
                SERIAL_DEBUG.printf("Progress: %u%%\r", (progress / (total / 100)));
            })
            .onError([](ota_error_t error) {
                SERIAL_DEBUG.printf("Error[%u]: ", error);
                if (error == OTA_AUTH_ERROR) {
                    SERIAL_DEBUG.println("Auth Failed");
                } else if (error == OTA_BEGIN_ERROR) {
                    SERIAL_DEBUG.println("Begin Failed");
                } else if (error == OTA_CONNECT_ERROR) {
                    SERIAL_DEBUG.println("Connect Failed");
                } else if (error == OTA_RECEIVE_ERROR) {
                    SERIAL_DEBUG.println("Receive Failed");
                } else if (error == OTA_END_ERROR) {
                    SERIAL_DEBUG.println("End Failed");
                }
            });

    ota_helper_is_initialized = true;
    SERIAL_DEBUG.println("OTA initialized");
}

void radio_helper_ota_begin() {
    if(!ota_helper_is_initialized) {
        SERIAL_DEBUG.println("WARNING: OTA was not initialized before calling radio_helper_ota_begin()");
        radio_helper_ota_init(OTA_DNS_NAME);
    }
    if (ota_helper_is_running) {
        SERIAL_DEBUG.println("OTA is already listening");
    } else {
        SERIAL_DEBUG.println("Starting OTA...");
        ArduinoOTA.begin();
        ota_helper_is_running = true;
        SERIAL_DEBUG.println("OTA listening on IP ");
        SERIAL_DEBUG.println(WiFi.localIP());
        digitalWrite(USER_LED_PIN, LOW);
    }
}

void radio_helper_ota_end() {
    if (ota_helper_is_running) {
        SERIAL_DEBUG.println("Stopping OTA...");
        ArduinoOTA.end();
        ota_helper_is_running = false;
        SERIAL_DEBUG.println("OTA stopped");
        digitalWrite(USER_LED_PIN, HIGH);
    } else {
        SERIAL_DEBUG.println("OTA is already stopped");
    }
}

void radio_helper_ota_loop() {
    if (ota_helper_is_running) {
        ArduinoOTA.handle();
    }
}

int radio_helper_http_get(const std::string& serverUrl, const std::map<std::string, std::string>& params) {
    HTTPClient http;
    std::string serverPath = serverUrl;
    for (const auto& itr: params) {
        serverPath += (itr == *params.begin() ? std::string("?") : std::string("&"));
        serverPath += itr.first + "=" + itr.second;
    }
    http.begin(serverPath.c_str());
    return http.GET();
}