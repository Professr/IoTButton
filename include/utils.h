//
// Utility functions and reusable hardware abstractions
//
#pragma once

#ifndef IoTButtonUtils_h

/** HEADER SETUP */
#define IoTButtonUtils_h
#ifndef Arduino_h
    #include <Arduino.h> // For pin constants and types
#endif
/** END HEADER SETUP */

/** SPECIAL MACROS */
#ifdef ENABLE_SERIAL_DEBUG
    #define SERIAL_DEBUG Serial
#else
    // This is a do-nothing replacement for "Serial"
    class VoidSerial {
        public:
            template <typename T>
            void begin(T t...) {}
            template <typename T>
            void println(T t...) {}
            template <typename T>
            void printf(T t...) {}
    };
    extern VoidSerial voidSerial;
    #define SERIAL_DEBUG voidSerial
#endif
/** END SPECIAL MACROS */

void utils_init();
void utils_blink_user_led(uint8_t numBlinks, uint16_t blinkDelay);

#endif //IoTButtonUtils_h