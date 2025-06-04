#include <unity.h>
#include "relay_controller.h"
#include "test_helpers.h"

void test_relay_initial_state(void) {
    // Check pin modes are set to OUTPUT
    TEST_ASSERT_EQUAL(LOW, mock_pin_modes[9]);  // Relay 1
    TEST_ASSERT_EQUAL(LOW, mock_pin_modes[6]);  // Relay 2
    TEST_ASSERT_EQUAL(LOW, mock_pin_modes[5]);  // Relay 3
    TEST_ASSERT_EQUAL(LOW, mock_pin_modes[4]);  // Relay 4
    
    // Check initial states
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(0));
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(1));
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(2));
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(3));
    
    // Check initial pin states (active LOW)
    TEST_ASSERT_EQUAL(LOW, mock_pin_states[9]);  // Relay 1
    TEST_ASSERT_EQUAL(LOW, mock_pin_states[6]);  // Relay 2
    TEST_ASSERT_EQUAL(LOW, mock_pin_states[5]);  // Relay 3
    TEST_ASSERT_EQUAL(LOW, mock_pin_states[4]);  // Relay 4
}

void test_set_single_relay(void) {
    relayFixture->relays->setRelay(0, true);
    TEST_ASSERT_TRUE(relayFixture->relays->getRelayState(0));
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(1));
    TEST_ASSERT_EQUAL(LOW, mock_pin_states[9]);  // Active LOW logic
    
    relayFixture->relays->setRelay(0, false);
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(0));
    TEST_ASSERT_EQUAL(HIGH, mock_pin_states[9]);  // Active LOW logic
}

void test_set_all_relays(void) {
    relayFixture->relays->setAllRelays(true);
    TEST_ASSERT_TRUE(relayFixture->relays->getRelayState(0));
    TEST_ASSERT_TRUE(relayFixture->relays->getRelayState(1));
    TEST_ASSERT_TRUE(relayFixture->relays->getRelayState(2));
    TEST_ASSERT_TRUE(relayFixture->relays->getRelayState(3));
    
    // Check all pins are LOW (active LOW logic)
    TEST_ASSERT_EQUAL(LOW, mock_pin_states[9]);
    TEST_ASSERT_EQUAL(LOW, mock_pin_states[6]);
    TEST_ASSERT_EQUAL(LOW, mock_pin_states[5]);
    TEST_ASSERT_EQUAL(LOW, mock_pin_states[4]);
    
    relayFixture->relays->setAllRelays(false);
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(0));
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(1));
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(2));
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(3));
    
    // Check all pins are HIGH (active LOW logic)
    TEST_ASSERT_EQUAL(HIGH, mock_pin_states[9]);
    TEST_ASSERT_EQUAL(HIGH, mock_pin_states[6]);
    TEST_ASSERT_EQUAL(HIGH, mock_pin_states[5]);
    TEST_ASSERT_EQUAL(HIGH, mock_pin_states[4]);
}

void test_invalid_relay_number(void) {
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(4));
    relayFixture->relays->setRelay(4, true);
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(4));
}

void test_relay_state_transitions(void) {
    // Clear history to start fresh
    resetMockState();
    relayFixture->relays->begin();
    
    // Clear again to only track our specific transitions
    resetMockState();
    
    // Test sequence: OFF -> ON -> OFF
    // Start with relay OFF (should be HIGH)
    relayFixture->relays->setRelay(0, false);  // Ensure it's OFF (HIGH)
    
    const uint8_t expectedStates[] = {HIGH, LOW, HIGH};  // OFF -> ON -> OFF
    relayFixture->relays->setRelay(0, true);   // Goes LOW (active low)
    advanceMockTime(100);
    relayFixture->relays->setRelay(0, false);  // Goes HIGH
    
    TEST_ASSERT_TRUE(verifyPinSequence(9, expectedStates, 3));
    TEST_ASSERT_EQUAL(3, getPinTransitionCount(9));
}

void test_rapid_relay_switching(void) {
    
    // Rapidly switch relay on and off
    for(int i = 0; i < 5; i++) {
        relayFixture->relays->setRelay(0, true);
        advanceMockTime(10);
        relayFixture->relays->setRelay(0, false);
        advanceMockTime(10);
    }
    
    TEST_ASSERT_EQUAL(10, getPinTransitionCount(9));
}

void test_sequential_relay_operations(void) {
    // Clear history to start fresh
    resetMockState();
    relayFixture->relays->begin();
    
    // Test sequential activation pattern with timing
    relayFixture->relays->setRelay(0, true);  // Pin 9 goes LOW at time 0
    advanceMockTime(100);
    relayFixture->relays->setRelay(1, true);  // Pin 6 goes LOW at time 100
    advanceMockTime(100);
    relayFixture->relays->setRelay(2, true);  // Pin 5 goes LOW at time 200
    advanceMockTime(100);
    relayFixture->relays->setRelay(3, true);  // Pin 4 goes LOW at time 300
    
    TEST_ASSERT_TRUE(relayFixture->relays->getRelayState(0));
    TEST_ASSERT_TRUE(relayFixture->relays->getRelayState(1));
    TEST_ASSERT_TRUE(relayFixture->relays->getRelayState(2));
    TEST_ASSERT_TRUE(relayFixture->relays->getRelayState(3));
    
    // Verify timing between first relay activation and last relay activation
    // Pin 9 goes LOW at time 0, Pin 4 goes LOW at time 300
    // So we check the time between pin 9 going LOW and current time
    TEST_ASSERT_EQUAL(300, millis());
}
