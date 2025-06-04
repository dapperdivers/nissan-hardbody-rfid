#include "relay_controller.h"

RelayController::RelayController(uint8_t relay1_pin, uint8_t relay2_pin, 
                                uint8_t relay3_pin, uint8_t relay4_pin) {
    relays[0] = {relay1_pin, false};
    relays[1] = {relay2_pin, false};
    relays[2] = {relay3_pin, false};
    relays[3] = {relay4_pin, false};
}

void RelayController::begin() {
    for (uint8_t i = 0; i < NUM_RELAYS; i++) {
        pinMode(relays[i].pin, OUTPUT);
        setRelay(i, false); // Initialize all relays to OFF state
    }
}

void RelayController::setRelay(uint8_t relay, bool state) {
    if (isValidRelay(relay)) {
        relays[relay].state = state;
        // Active LOW relay logic
        digitalWrite(relays[relay].pin, !state);
    }
}

void RelayController::setAllRelays(bool state) {
    for (uint8_t i = 0; i < NUM_RELAYS; i++) {
        setRelay(i, state);
    }
}

bool RelayController::getRelayState(uint8_t relay) const {
    if (isValidRelay(relay)) {
        return relays[relay].state;
    }
    return false;
}

bool RelayController::isValidRelay(uint8_t relay) const {
    return relay < NUM_RELAYS;
}