#include <unity.h>
#include "audio_player.h"
#include "test_helpers.h"

void test_audio_initialization(void) {
    TEST_ASSERT_TRUE(audioFixture->audio->begin());
}

void test_audio_volume_control(void) {
    audioFixture->audio->begin();
    
    // Test normal volume setting
    audioFixture->audio->setVolume(15);
    // Note: We can't directly verify volume without accessing private members
    // In a real scenario, we'd need getter methods or friend classes
    
    // Test volume clamping
    audioFixture->audio->setVolume(50); // Should be clamped to 30
    audioFixture->audio->setVolume(-5); // Should be clamped to 0
}

void test_audio_play_tracks(void) {
    audioFixture->audio->begin();
    
    // Test playing different tracks and verify with mock
    audioFixture->audio->playTrack(AudioPlayer::SOUND_STARTUP);
    TEST_ASSERT_EQUAL(AudioPlayer::SOUND_STARTUP, audioFixture->audio->player->getLastPlayedTrack());

    audioFixture->audio->playTrack(AudioPlayer::SOUND_WAITING);
    TEST_ASSERT_EQUAL(AudioPlayer::SOUND_WAITING, audioFixture->audio->player->getLastPlayedTrack());

    audioFixture->audio->playTrack(AudioPlayer::SOUND_ACCEPTED);
    TEST_ASSERT_EQUAL(AudioPlayer::SOUND_ACCEPTED, audioFixture->audio->player->getLastPlayedTrack());

    audioFixture->audio->playTrack(AudioPlayer::SOUND_DENIED_1);
    TEST_ASSERT_EQUAL(AudioPlayer::SOUND_DENIED_1, audioFixture->audio->player->getLastPlayedTrack());

    audioFixture->audio->playTrack(AudioPlayer::SOUND_DENIED_2);
    TEST_ASSERT_EQUAL(AudioPlayer::SOUND_DENIED_2, audioFixture->audio->player->getLastPlayedTrack());

    audioFixture->audio->playTrack(AudioPlayer::SOUND_DENIED_3);
    TEST_ASSERT_EQUAL(AudioPlayer::SOUND_DENIED_3, audioFixture->audio->player->getLastPlayedTrack());
    
    // Test playing before initialization
    AudioPlayer uninitializedAudio;
    uninitializedAudio.playTrack(1); // Should not crash or interact with player
    // Add assertion here if possible to confirm no interaction,
    // but mock_jq6500 doesn't currently support this.
}

void test_audio_track_constants(void) {
    // Verify track constants match expected values
    TEST_ASSERT_EQUAL(1, AudioPlayer::SOUND_STARTUP);
    TEST_ASSERT_EQUAL(2, AudioPlayer::SOUND_WAITING);
    TEST_ASSERT_EQUAL(3, AudioPlayer::SOUND_ACCEPTED);
    TEST_ASSERT_EQUAL(4, AudioPlayer::SOUND_DENIED_1);
    TEST_ASSERT_EQUAL(5, AudioPlayer::SOUND_DENIED_2);
    TEST_ASSERT_EQUAL(6, AudioPlayer::SOUND_DENIED_3);
}

void test_audio_reset(void) {
    audioFixture->audio->begin();
    
    // Test reset functionality (JQ6500 specific)
    audioFixture->audio->reset();
    // Reset method exists but we can't verify it was called without
    // adding tracking to the mock
}