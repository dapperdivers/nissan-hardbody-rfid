#pragma once

#ifdef UNIT_TEST
    #include "mock_arduino.h"
    #ifdef USINGMP3
        #include "mock_dfplayer.h"
    #endif
#else
    #include <Arduino.h>
    #ifdef USINGMP3
        #include <DFPlayerMini_Fast.h>
        #include <SoftwareSerial.h>
    #endif
#endif

class AudioPlayer {
public:
    AudioPlayer(uint8_t rx_pin = 8, uint8_t tx_pin = 7);
    bool begin();
    void setVolume(uint8_t volume);
    void playTrack(uint8_t track);
    
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

#ifdef USINGMP3
// Mock state for unit testing
public: 
    bool dfplayer_enabled;
    SoftwareSerial serial;
    DFPlayerMini_Fast player;
private: // Keep private in production
#endif
};