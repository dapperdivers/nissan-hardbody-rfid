#include <unity.h>
#include "test_helpers.h"

// Declare test functions
void test_valid_card_flow(void);
void test_invalid_card_flow_first_attempt(void);
void test_invalid_card_flow_second_attempt(void);
void test_invalid_card_flow_third_attempt(void);
void test_exponential_backoff(void);
void test_max_invalid_attempts(void);
void test_waiting_sound_after_10_seconds(void);
void test_relay_timing_accuracy(void);

// setUp and tearDown are defined in test_helpers.cpp

int main(int argc, char **argv) {
    UNITY_BEGIN();
    
    RUN_TEST(test_valid_card_flow);
    RUN_TEST(test_invalid_card_flow_first_attempt);
    RUN_TEST(test_invalid_card_flow_second_attempt);
    RUN_TEST(test_invalid_card_flow_third_attempt);
    RUN_TEST(test_exponential_backoff);
    RUN_TEST(test_max_invalid_attempts);
    RUN_TEST(test_waiting_sound_after_10_seconds);
    RUN_TEST(test_relay_timing_accuracy);
    
    return UNITY_END();
}