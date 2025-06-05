#include "mock_arduino.h"

// Mock state variables
uint8_t mock_pin_modes[MAX_PINS] = {0};
uint8_t mock_pin_states[MAX_PINS] = {0};
PinStateChange mock_pin_history[MAX_PIN_HISTORY];
uint8_t mock_history_index = 0;
millis_t mock_millis = 0;
millis_t mock_delay_time = 0;
HardwareSerial Serial;

// Forward declarations
void recordPinStateChange(uint8_t pin, uint8_t state);

// Mock function implementations
void pinMode(uint8_t pin, uint8_t mode) {
    if (pin < MAX_PINS) {
        mock_pin_modes[pin] = mode;
    }
}

void digitalWrite(uint8_t pin, uint8_t val) {
    if (pin < MAX_PINS) {
        mock_pin_states[pin] = val;
        recordPinStateChange(pin, val);
    }
}

uint8_t digitalRead(uint8_t pin) {
    if (pin < MAX_PINS) {
        return mock_pin_states[pin];
    }
    return 0;
}

millis_t millis(void) {
    return mock_millis;
}

void delay(unsigned long ms) {
    mock_delay_time += ms;
    mock_millis += ms;
}

// Helper function implementations
void resetMockState(void) {
    for (int i = 0; i < MAX_PINS; i++) {
        mock_pin_modes[i] = 0;
        mock_pin_states[i] = 0;
    }
    mock_history_index = 0;
    mock_millis = 0;
    mock_delay_time = 0;
    // Serial doesn't need reset - it's stateless in our mock
}

uint8_t getPinState(uint8_t pin) {
    if (pin < MAX_PINS) {
        return mock_pin_states[pin];
    }
    return 0;
}

uint8_t getPinMode(uint8_t pin) {
    if (pin < MAX_PINS) {
        return mock_pin_modes[pin];
    }
    return 0;
}

void setMockMillis(millis_t value) {
    mock_millis = value;
}

uint8_t getPinHistoryCount() {
    return mock_history_index;
}

PinStateChange getPinHistoryEntry(uint8_t index) {
    if (index < mock_history_index) {
        return mock_pin_history[index];
    }
    return {0, 0, 0};
}

void resetPinHistory(void) {
    mock_history_index = 0;
}

// Internal helper functions
void recordPinStateChange(uint8_t pin, uint8_t state) {
    if (mock_history_index < MAX_PIN_HISTORY) {
        mock_pin_history[mock_history_index].pin = pin;
        mock_pin_history[mock_history_index].state = state;
        mock_pin_history[mock_history_index].timestamp = mock_millis;
        mock_history_index++;
    }
}