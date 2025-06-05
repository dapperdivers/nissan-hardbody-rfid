#include <unity.h>
#include "test_helpers.h"

// Declare test functions
void test_audio_initialization(void);
void test_audio_volume_control(void);
void test_audio_play_tracks(void);
void test_audio_track_constants(void);
void test_audio_reset(void);
void test_audio_status_monitoring(void);
void test_audio_source_control(void);
void test_audio_source_ensures_builtin_on_init(void);

// setUp and tearDown are defined in test_helpers.cpp

int main(int argc, char **argv) {
    UNITY_BEGIN();
    
    RUN_TEST(test_audio_initialization);
    RUN_TEST(test_audio_volume_control);
    RUN_TEST(test_audio_play_tracks);
    RUN_TEST(test_audio_track_constants);
    RUN_TEST(test_audio_reset);
    RUN_TEST(test_audio_status_monitoring);
    RUN_TEST(test_audio_source_control);
    RUN_TEST(test_audio_source_ensures_builtin_on_init);
    
    return UNITY_END();
}