#include "mock_arduino.h"

// Mock state variables
uint8_t mock_pin_modes[MAX_PINS] = {0};
uint8_t mock_pin_states[MAX_PINS] = {0};
PinStateChange pin_state_history[MAX_PIN_HISTORY];
uint8_t pin_history_count = 0;
millis_t mock_current_time = 0;
millis_t mock_delay_time = 0;
HardwareSerial Serial;

// Mock function implementations
void pinMode(uint8_t pin, uint8_t mode) {
    if (pin < MAX_PINS) {
        mock_pin_modes[pin] = mode;
    }
}
void pinMode(unsigned int pin, unsigned int mode) {
    pinMode(static_cast<uint8_t>(pin), static_cast<uint8_t>(mode));
}

void digitalWrite(uint8_t pin, uint8_t val) {
    if (pin < MAX_PINS) {
        mock_pin_states[pin] = val;
        recordPinStateChange(pin, val);
    }
}

void digitalWrite(unsigned int pin, unsigned int val) {
    digitalWrite(static_cast<uint8_t>(pin), static_cast<uint8_t>(val));
}

millis_t millis(void) {
    return mock_current_time;
}

void delay(millis_t ms) {
    mock_delay_time += ms;
    mock_current_time += ms;
}

// Helper function implementations
void resetMockState(void) {
    for (int i = 0; i < MAX_PINS; i++) {
        mock_pin_modes[i] = 0;
        mock_pin_states[i] = 0;
    }
    pin_history_count = 0;
    mock_current_time = 0;
    mock_delay_time = 0;
    Serial.reset();
}

// Additional helper function implementations
void advanceMockTime(millis_t ms) {
    mock_current_time += ms;
}

void recordPinStateChange(uint8_t pin, uint8_t state) {
    if (pin_history_count < MAX_PIN_HISTORY) {
        pin_state_history[pin_history_count].pin = pin;
        pin_state_history[pin_history_count].state = state;
        pin_state_history[pin_history_count].timestamp = mock_current_time;
        pin_history_count++;
    }
}

uint8_t getPinTransitionCount(uint8_t pin) {
    uint8_t count = 0;
    for (uint8_t i = 0; i < pin_history_count; i++) {
        if (pin_state_history[i].pin == pin) {
            count++;
        }
    }
    return count;
}

bool verifyPinSequence(uint8_t pin, const uint8_t* expectedStates, uint8_t stateCount) {
    uint8_t pinStateIndex = 0;
    for (uint8_t i = 0; i < pin_history_count && pinStateIndex < stateCount; i++) {
        if (pin_state_history[i].pin == pin) {
            if (pin_state_history[i].state != expectedStates[pinStateIndex]) {
                return false;
            }
            pinStateIndex++;
        }
    }
    return pinStateIndex == stateCount;
}

millis_t getTimeBetweenStates(uint8_t pin, uint8_t firstState, uint8_t secondState) {
    millis_t firstTime = 0;
    bool foundFirst = false;
    
    for (uint8_t i = 0; i < pin_history_count; i++) {
        if (pin_state_history[i].pin == pin) {
            if (!foundFirst && pin_state_history[i].state == firstState) {
                firstTime = pin_state_history[i].timestamp;
                foundFirst = true;
            } else if (foundFirst && pin_state_history[i].state == secondState) {
                return pin_state_history[i].timestamp - firstTime;
            }
        }
    }
    return 0; // Not found
}

millis_t getTotalDelayTime(void) {
    return mock_delay_time;
}