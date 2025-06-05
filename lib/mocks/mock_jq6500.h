#pragma once

#include <cstdint>
#include "mock_arduino.h"  // Include this to get HardwareSerial definition

// JQ6500 Status constants (matching the real library)
#ifndef MP3_STATUS_STOPPED
#define MP3_STATUS_STOPPED  0
#define MP3_STATUS_PLAYING  1
#define MP3_STATUS_PAUSED   2
#endif

// JQ6500 Source constants
#ifndef MP3_SRC_BUILTIN
#define MP3_SRC_BUILTIN    0
#define MP3_SRC_SDCARD     1
#endif

// Mock JQ6500_Serial class for unit testing
class JQ6500_Serial {
public:
    JQ6500_Serial(HardwareSerial& serial) 
        : serial_(serial)
        , volume_(20)
        , is_playing_(false)
        , last_played_track_(0)
        , status_(MP3_STATUS_STOPPED)
        , source_(MP3_SRC_BUILTIN)
        , position_seconds_(0) {}
    
    void reset() {
        // Mock reset functionality
        is_playing_ = false;
        status_ = MP3_STATUS_STOPPED;
        position_seconds_ = 0;
    }
    
    void setVolume(uint8_t volume) {
        volume_ = volume;
    }
    
    uint8_t getVolume() const {
        return volume_;
    }
    
    void playFileByIndexNumber(uint8_t fileNumber) {
        last_played_track_ = fileNumber;
        is_playing_ = true;
        status_ = MP3_STATUS_PLAYING;
        position_seconds_ = 0;
    }
    
    bool isPlaying() const {
        return is_playing_;
    }
    
    uint8_t getLastPlayedTrack() const {
        return last_played_track_;
    }
    
    // Status monitoring methods
    uint8_t getStatus() const {
        return status_;
    }
    
    uint16_t currentFilePositionInSeconds() const {
        return position_seconds_;
    }
    
    // Source control methods
    void setSource(uint8_t source) {
        source_ = source;
    }
    
    uint8_t getSource() const {
        return source_;
    }
    
    // Test helper methods
    void setStatus(uint8_t status) {
        status_ = status;
        is_playing_ = (status == MP3_STATUS_PLAYING);
    }
    
    void setPosition(uint16_t seconds) {
        position_seconds_ = seconds;
    }
    
private:
    HardwareSerial& serial_;
    uint8_t volume_;
    bool is_playing_;
    uint8_t last_played_track_;
    uint8_t status_;
    uint8_t source_;
    uint16_t position_seconds_;
};