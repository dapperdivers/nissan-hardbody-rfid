#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

// Mock Arduino types and constants
#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define DEC 10
#define HEX 16

// Type definitions
typedef unsigned long millis_t;
typedef bool boolean;

// Maximum number of state changes to track
#define MAX_PIN_HISTORY 50
#define MAX_PINS 20

// Macro definitions
#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif
#define F(string_literal) (string_literal)

// Pin state change record
struct PinStateChange {
    uint8_t pin;
    uint8_t state;
    millis_t timestamp;
};

// Mock serial
class HardwareSerial {
public:
    void begin(unsigned long) {}
    void print(const char*) { printf("[Serial print] %s", ""); }
    void print(const uint32_t, int) { printf("[Serial print] %s %d", "", 0); }
    void print(int) {}
    void println(const char*) { printf("[Serial println] %s\n", ""); }
    void println(const uint32_t, int) { printf("[Serial println] %s %d\n", "", 0); }
    void println(int) { printf("[Serial println] %d\n", 0); }
    void reset() {}
    // Add more as needed
};

extern HardwareSerial Serial;

// Mock state tracking
extern uint8_t mock_pin_modes[MAX_PINS];
extern uint8_t mock_pin_states[MAX_PINS];
extern PinStateChange pin_state_history[MAX_PIN_HISTORY];
extern uint8_t pin_history_count;
extern millis_t mock_current_time;
extern millis_t mock_delay_time;

// Mock function declarations
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
millis_t millis(void);
void delay(millis_t ms);

// Helper functions for testing
void resetMockState(void);
void advanceMockTime(millis_t ms);
millis_t getTimeBetweenStates(uint8_t pin, uint8_t firstState, uint8_t secondState);
uint8_t getPinTransitionCount(uint8_t pin);
bool verifyPinSequence(uint8_t pin, const uint8_t* expectedStates, uint8_t stateCount);
millis_t getTotalDelayTime(void);

// Internal helper
void recordPinStateChange(uint8_t pin, uint8_t state);

