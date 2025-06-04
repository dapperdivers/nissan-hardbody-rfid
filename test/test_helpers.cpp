#include <unity.h>
#include "mock_arduino.h"
#include "test_helpers.h"

// Common test utilities and helper functions
void reset_test_state() {
    resetMockState();
}

// Initialize fixture pointers
AudioTestFixture* audioFixture = nullptr;
RelayTestFixture* relayFixture = nullptr;
IntegrationTestFixture* integrationFixture = nullptr;
RFIDTestFixture* rfidFixture = nullptr;

void setUp(void) {
    TEST_PROTECT();
    resetMockState();
    audioFixture = new AudioTestFixture();
    relayFixture = new RelayTestFixture();
    integrationFixture = new IntegrationTestFixture();
    rfidFixture = new RFIDTestFixture();
}

void tearDown(void) {
    TEST_PROTECT();
    delete audioFixture;
    delete relayFixture;
    delete integrationFixture;
    delete rfidFixture;
    audioFixture = nullptr;
    relayFixture = nullptr;
    integrationFixture = nullptr;
    rfidFixture = nullptr;
}
