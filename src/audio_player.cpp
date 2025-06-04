#include <string.h> 
#include "audio_player.h"

AudioPlayer::AudioPlayer(uint8_t rx_pin, uint8_t tx_pin)
#ifdef USINGMP3
    : initialized(false)
    , current_volume(10)
    , dfplayer_enabled(false)
    , serial(rx_pin, tx_pin)
#else
    : initialized(false)
    , current_volume(10)
#endif
{
}

bool AudioPlayer::begin() {
#ifdef USINGMP3
    serial.begin(9600);
    if (player.begin(serial, false)) {
        dfplayer_enabled = true;
        player.volume(current_volume);
        initialized = true;
        return true;
    }
    return false;
#else
    initialized = true;
    return true;
#endif
}

void AudioPlayer::setVolume(uint8_t volume) {
    if (!initialized) return;
    
    current_volume = min(30, max(0, (int)volume));
#ifdef USINGMP3
    if (dfplayer_enabled) {
        player.volume(current_volume);
    }
#endif
}

void AudioPlayer::playTrack(uint8_t track) {
    if (!initialized) return;
    
#ifdef USINGMP3
    if (dfplayer_enabled) {
        player.play(track);
    }
#endif
}