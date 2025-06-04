#pragma once

#include <stdint.h>

#define PN532_MIFARE_ISO14443A (0x00)

// Mock PN532 class
class Adafruit_PN532 {
private:
    uint8_t _ss_pin;
    uint32_t _firmwareVersion;
    bool _initialized;
    
    // Mock card data
    uint8_t* _mockUID;
    uint8_t _mockUIDLength;
    bool _hasCard;
    
public:
    Adafruit_PN532(uint8_t ss) : _ss_pin(ss), _firmwareVersion(0x0153), _initialized(false),
                                _mockUID(nullptr), _mockUIDLength(0), _hasCard(false) {}
    
    void begin() {
        _initialized = true;
    }
    
    uint32_t getFirmwareVersion() {
        return _initialized ? _firmwareVersion : 0;
    }
    
    void SAMConfig() {
        // Mock implementation - does nothing
    }
    
    bool readPassiveTargetID(uint8_t cardbaudrate, uint8_t* uid, uint8_t* uidLength, uint16_t timeout = 0) {
        if (!_initialized || !_hasCard) {
            return false;
        }
        
        if (_mockUID && uid && uidLength) {
            // Simple copy instead of memcpy
            for (uint8_t i = 0; i < _mockUIDLength; i++) {
                uid[i] = _mockUID[i];
            }
            *uidLength = _mockUIDLength;
            return true;
        }
        
        return false;
    }
    
    // Test helper methods
    void setMockCard(uint8_t* uid, uint8_t uidLength) {
        _mockUID = uid;
        _mockUIDLength = uidLength;
        _hasCard = true;
    }
    
    void removeMockCard() {
        _hasCard = false;
    }
    
    void setFirmwareVersion(uint32_t version) {
        _firmwareVersion = version;
    }
};