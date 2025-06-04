#include <unity.h>
#include "test_helpers.h"

// Declare test functions
void test_relay_initial_state(void);
void test_set_single_relay(void);
void test_set_all_relays(void);
void test_invalid_relay_number(void);
void test_relay_state_transitions(void);
void test_rapid_relay_switching(void);
void test_sequential_relay_operations(void);

// setUp and tearDown are defined in test_helpers.cpp

int main(int argc, char **argv) {
    UNITY_BEGIN();
    
    RUN_TEST(test_relay_initial_state);
    RUN_TEST(test_set_single_relay);
    RUN_TEST(test_set_all_relays);
    RUN_TEST(test_invalid_relay_number);
    RUN_TEST(test_relay_state_transitions);
    RUN_TEST(test_rapid_relay_switching);
    RUN_TEST(test_sequential_relay_operations);
    
    return UNITY_END();
}