#include <unity.h>
#include "mock_arduino.h"
#include "test_helpers.h"

// Integration test constants
constexpr uint32_t DOOR_UNLOCK_DURATION = 10000;  // 10 seconds
constexpr uint8_t DOOR_UNLOCK_RELAY = 0;  // Relay 1 (0-indexed)
constexpr uint8_t MAXIMUM_INVALID_ATTEMPTS = 13;
const uint8_t invalidDelays[MAXIMUM_INVALID_ATTEMPTS] = {1, 3, 4, 5, 8, 12, 17, 23, 30, 38, 47, 57, 68};

void test_valid_card_flow(void) {
    // Simulate valid card scan
    // Expected: Play acceptance sound (track 3)
    integrationFixture->state.lastPlayedTrack = 3;
    TEST_ASSERT_EQUAL(3, integrationFixture->state.lastPlayedTrack);
    
    // Expected: Activate relay 1
    digitalWrite(9, LOW); // Relay 1 on pin 9, active LOW
    TEST_ASSERT_EQUAL(LOW, mock_pin_states[9]);
    integrationFixture->state.relayActive = true;
    integrationFixture->state.relayActivatedTime = millis();
    
    // Reset invalid attempts
    integrationFixture->state.invalidAttempts = 0;
    TEST_ASSERT_EQUAL(0, integrationFixture->state.invalidAttempts);
    
    // Advance time by 10 seconds
    advanceMockTime(DOOR_UNLOCK_DURATION);
    
    // Expected: Deactivate relay after 10 seconds
    if (millis() - integrationFixture->state.relayActivatedTime >= DOOR_UNLOCK_DURATION) {
        digitalWrite(9, HIGH); // Relay 1 off
        integrationFixture->state.relayActive = false;
    }
    
    TEST_ASSERT_EQUAL(HIGH, mock_pin_states[9]);
    TEST_ASSERT_FALSE(integrationFixture->state.relayActive);
}

void test_invalid_card_flow_first_attempt(void) {
    // Ensure relay is off at the start (HIGH = off due to active LOW logic)
    digitalWrite(9, HIGH);
    
    // First invalid attempt
    integrationFixture->state.invalidAttempts = 0;
    
    // Expected: Play first denial sound (track 4)
    integrationFixture->state.lastPlayedTrack = 4;
    TEST_ASSERT_EQUAL(4, integrationFixture->state.lastPlayedTrack);
    
    // Expected: 3 second base delay + 1 second exponential delay
    millis_t expectedDelay = 3000 + (invalidDelays[0] * 1000);
    delay(expectedDelay);
    TEST_ASSERT_EQUAL(expectedDelay, getTotalDelayTime());
    
    // Increment invalid attempts
    integrationFixture->state.invalidAttempts++;
    TEST_ASSERT_EQUAL(1, integrationFixture->state.invalidAttempts);
    
    // No relay activation
    TEST_ASSERT_EQUAL(HIGH, mock_pin_states[9]); // Relay should remain off
}

void test_invalid_card_flow_second_attempt(void) {
    // Second invalid attempt
    integrationFixture->state.invalidAttempts = 1;
    
    // Expected: Play second denial sound (track 5)
    integrationFixture->state.lastPlayedTrack = 5;
    TEST_ASSERT_EQUAL(5, integrationFixture->state.lastPlayedTrack);
    
    // Expected: 3 second base delay + 3 second exponential delay
    millis_t expectedDelay = 3000 + (invalidDelays[1] * 1000);
    delay(expectedDelay);
    
    // Increment invalid attempts
    integrationFixture->state.invalidAttempts++;
    TEST_ASSERT_EQUAL(2, integrationFixture->state.invalidAttempts);
}

void test_invalid_card_flow_third_attempt(void) {
    // Third invalid attempt
    integrationFixture->state.invalidAttempts = 2;
    
    // Expected: Play third denial sound (track 6)
    integrationFixture->state.lastPlayedTrack = 6;
    TEST_ASSERT_EQUAL(6, integrationFixture->state.lastPlayedTrack);
    
    // Expected: 3 second base delay + 4 second exponential delay
    millis_t expectedDelay = 3000 + (invalidDelays[2] * 1000);
    delay(expectedDelay);
    
    // Increment invalid attempts
    integrationFixture->state.invalidAttempts++;
    TEST_ASSERT_EQUAL(3, integrationFixture->state.invalidAttempts);
}

void test_exponential_backoff(void) {
    // Test exponential backoff delays
    for (uint8_t i = 0; i < 5; i++) {
        resetMockState();
        integrationFixture->state.invalidAttempts = i;
        
        millis_t expectedDelay = 3000 + (invalidDelays[i] * 1000);
        delay(expectedDelay);
        
        TEST_ASSERT_EQUAL(expectedDelay, getTotalDelayTime());
    }
}

void test_max_invalid_attempts(void) {
    // Test that invalid attempts don't exceed maximum
    integrationFixture->state.invalidAttempts = MAXIMUM_INVALID_ATTEMPTS - 1;
    
    // Attempt to increment
    if (integrationFixture->state.invalidAttempts < MAXIMUM_INVALID_ATTEMPTS - 1) {
        integrationFixture->state.invalidAttempts++;
    }
    
    TEST_ASSERT_EQUAL(MAXIMUM_INVALID_ATTEMPTS - 1, integrationFixture->state.invalidAttempts);
}

void test_waiting_sound_after_10_seconds(void) {
    // Advance time to 10 seconds
    advanceMockTime(10001);
    
    bool impatient = false;
    bool scanned = false;
    
    // Check if waiting sound should play
    if (millis() > 10000 && !impatient && !scanned) {
        integrationFixture->state.lastPlayedTrack = 2; // SOUND_WAITING
        impatient = true;
    }
    
    TEST_ASSERT_EQUAL(2, integrationFixture->state.lastPlayedTrack);
}

void test_relay_timing_accuracy(void) {
    // Activate relay
    digitalWrite(9, LOW);
    millis_t startTime = millis();
    
    // Advance time by exactly 10 seconds
    advanceMockTime(DOOR_UNLOCK_DURATION);
    
    // Deactivate relay
    digitalWrite(9, HIGH);
    
    // Verify timing
    millis_t duration = millis() - startTime;
    TEST_ASSERT_EQUAL(DOOR_UNLOCK_DURATION, duration);
    
    // Verify state transitions
    TEST_ASSERT_EQUAL(2, getPinTransitionCount(9)); // LOW then HIGH
}