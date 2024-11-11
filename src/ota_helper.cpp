//
// Created by Ellie Hussey on 11/11/24.
//

#include "ota_helper.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <NetworkUdp.h>
#include <ArduinoOTA.h>

bool ota_helper_is_initialized = false;
bool ota_helper_is_running = false;

void ota_helper_init(const char* ssid, const char* key) {
    // Set up the user LED
    pinMode(GPIO_NUM_15, OUTPUT);
    digitalWrite(GPIO_NUM_15, HIGH);

    // Select the radio antenna
    pinMode(OTA_HELPER_RADIO_SELECT_PIN, OUTPUT);
    digitalWrite(OTA_HELPER_RADIO_SELECT_PIN, OTA_HELPER_USE_CHIP_ANTENNA ? LOW : HIGH);

    // Enable the radio system
    pinMode(OTA_HELPER_RADIO_ENABLE_PIN, OUTPUT);
    digitalWrite(OTA_HELPER_RADIO_ENABLE_PIN, HIGH);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, key);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }

    // Port defaults to 3232
    // ArduinoOTA.setPort(3232);

    // Hostname defaults to esp3232-[MAC]
    ArduinoOTA.setHostname("esp32ota");

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
                Serial.println("Start updating " + type);
            })
            .onEnd([]() {
                Serial.println("\nEnd");
            })
            .onProgress([](unsigned int progress, unsigned int total) {
                Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
            })
            .onError([](ota_error_t error) {
                Serial.printf("Error[%u]: ", error);
                if (error == OTA_AUTH_ERROR) {
                    Serial.println("Auth Failed");
                } else if (error == OTA_BEGIN_ERROR) {
                    Serial.println("Begin Failed");
                } else if (error == OTA_CONNECT_ERROR) {
                    Serial.println("Connect Failed");
                } else if (error == OTA_RECEIVE_ERROR) {
                    Serial.println("Receive Failed");
                } else if (error == OTA_END_ERROR) {
                    Serial.println("End Failed");
                }
            });

    ota_helper_is_initialized = true;
    Serial.println("OTA initialized");
}

void ota_helper_begin() {
    assert(ota_helper_is_initialized);
    if (ota_helper_is_running) {
        Serial.println("OTA is already listening");
    } else {
        Serial.println("Starting OTA...");
        ArduinoOTA.begin();
        ota_helper_is_running = true;
        Serial.println("OTA listening on IP ");
        Serial.println(WiFi.localIP());
        digitalWrite(GPIO_NUM_15, LOW);
    }
}

void ota_helper_end() {
    assert(ota_helper_is_initialized);
    if (ota_helper_is_running) {
        Serial.println("Stopping OTA...");
        ArduinoOTA.end();
        ota_helper_is_running = false;
        Serial.println("OTA stopped");
        digitalWrite(GPIO_NUM_15, HIGH);
    } else {
        Serial.println("OTA is already stopped");
    }
}

void ota_helper_loop() {
    if (ota_helper_is_running) {
        ArduinoOTA.handle();
    }
}