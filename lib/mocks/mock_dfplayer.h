#pragma once

#include <stdint.h>

// Mock Serial class for DFPlayer
class SoftwareSerial {
private:
    uint8_t _rx_pin;
    uint8_t _tx_pin;
    
public:
    SoftwareSerial(uint8_t rx, uint8_t tx) : _rx_pin(rx), _tx_pin(tx) {}
    
    void begin(long baudrate) {
        // Mock implementation
    }
    
    int available() {
        return 0;
    }
    
    uint8_t read() {
        return 0;
    }
    
    void write(uint8_t data) {
        // Mock implementation
    }
};

// Mock DFPlayerMini_Fast class
class DFPlayerMini_Fast {
private:
    bool _initialized;
    uint8_t _volume;
    uint8_t _lastPlayedTrack;
    
public:
    DFPlayerMini_Fast() : _initialized(false), _volume(0), _lastPlayedTrack(0) {}
    
    bool begin(SoftwareSerial& serial, bool debug = false) {
        _initialized = true;
        return true;
    }
    
    void volume(uint8_t vol) {
        if (_initialized) {
            _volume = vol;
        }
    }
    
    void play(uint8_t track) {
        if (_initialized) {
            _lastPlayedTrack = track;
        }
    }
    
    // Test helper methods
    uint8_t getVolume() const {
        return _volume;
    }
    
    uint8_t getLastPlayedTrack() const {
        return _lastPlayedTrack;
    }
    
    bool isInitialized() const {
        return _initialized;
    }
};