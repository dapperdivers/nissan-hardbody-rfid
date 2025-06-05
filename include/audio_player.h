#pragma once

#ifdef UNIT_TEST
    #include "mock_arduino.h"
    #ifdef USINGMP3
        #include "mock_jq6500.h"
    #endif
#else
    #include <Arduino.h>
    #ifdef USINGMP3
        #include <JQ6500_Serial.h>
        #include <HardwareSerial.h>
    #endif
#endif

class AudioPlayer {
public:
    AudioPlayer(uint8_t rx_pin = 0, uint8_t tx_pin = 1);  // ESP32-C3 Serial1 pins
    
    bool begin();
    void setVolume(uint8_t volume);
    void playTrack(uint8_t track);
    void reset();  // JQ6500 reset method
    
    // Sound effect constants
    static constexpr uint8_t SOUND_STARTUP = 1;
    static constexpr uint8_t SOUND_WAITING = 2;
    static constexpr uint8_t SOUND_ACCEPTED = 3;
    static constexpr uint8_t SOUND_DENIED_1 = 4;
    static constexpr uint8_t SOUND_DENIED_2 = 5;
    static constexpr uint8_t SOUND_DENIED_3 = 6;

private:
    bool initialized;
    uint8_t current_volume;
    uint8_t rx_pin;
    uint8_t tx_pin;

#ifdef USINGMP3
// Mock state for unit testing
public: 
    bool audio_enabled;
    HardwareSerial* serial;
    JQ6500_Serial* player;
private: // Keep private in production
#endif
};