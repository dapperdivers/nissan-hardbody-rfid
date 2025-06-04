#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <unity.h>
#include "mock_arduino.h"
#include "audio_player.h"
#include "relay_controller.h"
#include "rfid_controller.h"

// Test fixtures
class AudioTestFixture {
public:
    AudioPlayer* audio;
    
    AudioTestFixture() {
        audio = new AudioPlayer(8, 7);
    }
    
    ~AudioTestFixture() {
        delete audio;
    }
};

class RelayTestFixture {
public:
    RelayController* relays;
    
    RelayTestFixture() {
        relays = new RelayController();
        relays->begin();
    }
    
    ~RelayTestFixture() {
        relays->setAllRelays(false);
        delete relays;
    }
};

class IntegrationTestFixture {
public:
    struct SystemState {
        bool relayActive;
        uint8_t invalidAttempts;
        uint8_t lastPlayedTrack;
        millis_t relayActivatedTime;
        
        void reset() {
            relayActive = false;
            invalidAttempts = 0;
            lastPlayedTrack = 0;
            relayActivatedTime = 0;
        }
    } state;
    
    IntegrationTestFixture() {
        state.reset();
    }
    
    ~IntegrationTestFixture() {
        state.reset();
    }
};

class RFIDTestFixture {
public:
    RFIDController* rfid;
    
    RFIDTestFixture() {
        resetMockState();
        rfid = new RFIDController(10);
    }
    
    ~RFIDTestFixture() {
        delete rfid;
    }
};

// Global fixture pointers
extern AudioTestFixture* audioFixture;
extern RelayTestFixture* relayFixture;
extern IntegrationTestFixture* integrationFixture;
extern RFIDTestFixture* rfidFixture;

void setUp(void);
void tearDown(void);

#endif // TEST_HELPERS_H