#pragma once

#ifdef UNIT_TEST
    #include "mock_arduino.h"
    #include "mock_pn532.h"  // Mock PN532 for unit testing
#else
    #include <Arduino.h>
    #include <Adafruit_PN532.h>
#endif

class RFIDController {
public:
    // ESP32-C3 SPI SS pin
    RFIDController(uint8_t ss_pin = 5);
    bool begin();
    bool readCard(uint8_t* uid, uint8_t* uidLength);
    bool validateUID(const uint8_t* uid, uint8_t uidLength);
    void addUID4B(const uint8_t* uid);
    void addUID7B(const uint8_t* uid);
    uint32_t getFirmwareVersion();
    void printFirmwareVersion();
    void initializeDefaultUIDs();

 private:
    static constexpr uint8_t MAX_4B_UIDS = 1;
    static constexpr uint8_t MAX_7B_UIDS = 2;
    uint8_t _ss_pin;
    Adafruit_PN532* nfc;  // Using SPI interface
    
    uint8_t uids4B[MAX_4B_UIDS][4];
    uint8_t uids7B[MAX_7B_UIDS][7];
    uint8_t num4BUIDs;
    uint8_t num7BUIDs;

    bool compare4BUID(const uint8_t* uid1, const uint8_t* uid2);
    bool compare7BUID(const uint8_t* uid1, const uint8_t* uid2);
};