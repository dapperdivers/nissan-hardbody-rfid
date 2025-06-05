#include "audio_player.h"

// Global HardwareSerial instance for ESP32-C3
HardwareSerial MySerial(1); // Use Serial1

AudioPlayer::AudioPlayer(uint8_t rx_pin, uint8_t tx_pin)
    : initialized(false)
    , current_volume(20)
    , rx_pin(rx_pin)
    , tx_pin(tx_pin)
#ifdef USINGMP3
    , audio_enabled(false)
    , serial(&MySerial)
    , player(nullptr)
#endif
{
}

bool AudioPlayer::begin() {
#ifdef USINGMP3
    // ESP32-C3 with JQ6500
    // Initialize Serial1 on specified pins
    // Try without config parameter - use default
    #ifdef ESP32
        MySerial.begin(9600, SERIAL_8N1, rx_pin, tx_pin);
    #else
        MySerial.begin(9600);
    #endif
    delay(500);
    
    player = new JQ6500_Serial(MySerial);
    
    reset();  // Reset JQ6500 on startup
    delay(500);
    player->setVolume(current_volume);
    
    audio_enabled = true;
    initialized = true;
    return true;
#else
    initialized = true;
    return true;
#endif
}

void AudioPlayer::setVolume(uint8_t volume) {
    if (!initialized) return;
    
    // Clamp volume between 0-30
    if (volume > 30) {
        current_volume = 30;
    } else {
        current_volume = volume;
    }
    
#ifdef USINGMP3
    if (audio_enabled) {
        player->setVolume(current_volume);
    }
#endif
}

void AudioPlayer::playTrack(uint8_t track) {
    if (!initialized) return;
    
#ifdef USINGMP3
    if (audio_enabled) {
        player->playFileByIndexNumber(track);
    }
#endif
}

void AudioPlayer::reset() {
#ifdef USINGMP3
    if (player != nullptr) {
        player->reset();
    }
#endif
}