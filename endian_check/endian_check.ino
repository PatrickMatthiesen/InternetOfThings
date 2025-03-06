#include "WiFi.h"

void setup() {
    Serial.begin(115200);
    
    uint32_t testValue = 0x12345678;
    uint8_t *bytePtr = (uint8_t*)&testValue;

    Serial.println("Memory representation of 0x12345678:");
    for (int i = 0; i < sizeof(testValue); i++) {
        Serial.printf("Byte %d: 0x%02X\n", i, bytePtr[i]);
    }

    uint8_t mac[6];
    WiFi.macAddress(mac);

    Serial.print("MAC Address (Big Endian): ");
    for (int i = 0; i < 6; i++) {
        Serial.printf("%02X", mac[i]);
        if (i < 5) Serial.print(":");
    }
    Serial.println();

    Serial.print("MAC Address (Little Endian): ");
    for (int i = 5; i >= 0; i--) {  // Reverse order
        Serial.printf("%02X", mac[i]);
        if (i > 0) Serial.print(":");
    }
    Serial.println();
}

void loop() {
}

// little endian:
// Byte 0: 0x78  
// Byte 1: 0x56  
// Byte 2: 0x34  
// Byte 3: 0x12  

// big endian:
// Byte 0: 0x12  
// Byte 1: 0x34  
// Byte 2: 0x56  
// Byte 3: 0x78  

