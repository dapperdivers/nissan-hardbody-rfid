#include <string.h>
#include "rfid_controller.h"

RFIDController::RFIDController(uint8_t ss_pin)
    : _ss_pin(ss_pin)
    , num4BUIDs(0)
    , num7BUIDs(0)
{
    // Using SPI interface with Adafruit_PN532
    nfc = new Adafruit_PN532(_ss_pin);
}

bool RFIDController::begin() {
    nfc->begin();
    
    uint32_t versiondata = nfc->getFirmwareVersion();
    if (!versiondata) {
        Serial.print(F("Didn't find PN53x board"));
        return false;
    }
    
    // Configure board to read RFID tags
    nfc->SAMConfig();
    return true;
}

bool RFIDController::readCard(uint8_t* uid, uint8_t* uidLength) {
    return nfc->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, uidLength);
}

bool RFIDController::validateUID(const uint8_t* uid, uint8_t uidLength) {
    if (uidLength == 4) {
        for (uint8_t i = 0; i < num4BUIDs; i++) {
            if (compare4BUID(uids4B[i], uid)) {
                return true;
            }
        }
    } else if (uidLength == 7) {
        for (uint8_t i = 0; i < num7BUIDs; i++) {
            if (compare7BUID(uids7B[i], uid)) {
                return true;
            }
        }
    }
    return false;
}

void RFIDController::addUID4B(const uint8_t* uid) {
    if (num4BUIDs < MAX_4B_UIDS) {
        memcpy(uids4B[num4BUIDs], uid, 4);
        num4BUIDs++;
    }
}

void RFIDController::addUID7B(const uint8_t* uid) {
    if (num7BUIDs < MAX_7B_UIDS) {
        memcpy(uids7B[num7BUIDs], uid, 7);
        num7BUIDs++;
    }
}

uint32_t RFIDController::getFirmwareVersion() {
    return nfc->getFirmwareVersion();
}

void RFIDController::printFirmwareVersion() {
    uint32_t versiondata = getFirmwareVersion();
    if (versiondata) {
        Serial.print(F("Found chip PN5")); Serial.println((versiondata>>24) & 0xFF, HEX);
        Serial.print(F("Firmware ver. ")); Serial.print((versiondata>>16) & 0xFF, DEC);
        Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
    }
}

void RFIDController::initializeDefaultUIDs() {
    // Initialize with test UIDs as specified
    uint8_t testUID4B[] = {0xB4, 0x12, 0x34, 0x56};
    uint8_t testUID7B_1[] = {0x04, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC};
    uint8_t testUID7B_2[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD};
    
    addUID4B(testUID4B);
    addUID7B(testUID7B_1);
    addUID7B(testUID7B_2);
}

bool RFIDController::compare4BUID(const uint8_t* uid1, const uint8_t* uid2) {
    return memcmp(uid1, uid2, 4) == 0;
}

bool RFIDController::compare7BUID(const uint8_t* uid1, const uint8_t* uid2) {
    return memcmp(uid1, uid2, 7) == 0;
}