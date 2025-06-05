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

// ESP32 Serial constants
#ifndef SERIAL_8N1
#define SERIAL_8N1 0x800001c
#endif

// Type definitions
typedef unsigned long millis_t;
typedef bool boolean;

// Maximum number of state changes to track
#define MAX_PIN_HISTORY 50
#define MAX_PINS 22  // Increased to support GPIO 20 and 21 on ESP32-C3

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

// Mock HardwareSerial for unit testing (ESP32 pattern)
class HardwareSerial {
public:
    HardwareSerial(int uart_nr = 0) : uart_nr_(uart_nr) {}
    
    // ESP32-style begin with config and pins
    void begin(unsigned long baud, uint32_t config = SERIAL_8N1, int8_t rxPin = -1, int8_t txPin = -1) {
        baud_rate_ = baud;
        rx_pin_ = rxPin;
        tx_pin_ = txPin;
    }
    
    // Basic serial functions
    void print(const char* str) { printf("[Serial print] %s", str); }
    void print(const uint32_t val, int base = DEC) { 
        if (base == HEX) {
            printf("[Serial print] 0x%x", val);
        } else {
            printf("[Serial print] %u", val);
        }
    }
    void print(int val) { printf("[Serial print] %d", val); }
    void println(const char* str) { printf("[Serial println] %s\n", str); }
    void println(const uint32_t val, int base = DEC) { 
        if (base == HEX) {
            printf("[Serial println] 0x%x\n", val);
        } else {
            printf("[Serial println] %u\n", val);
        }
    }
    void println(int val) { printf("[Serial println] %d\n", val); }
    void println() { printf("\n"); }
    
    // Additional serial functions
    void write(uint8_t data) { /* Mock write */ }
    int available() { return 0; }
    int read() { return -1; }
    
    // Getters for testing
    unsigned long getBaudRate() const { return baud_rate_; }
    int8_t getRxPin() const { return rx_pin_; }
    int8_t getTxPin() const { return tx_pin_; }
    
private:
    int uart_nr_;
    unsigned long baud_rate_ = 0;
    int8_t rx_pin_ = -1;
    int8_t tx_pin_ = -1;
};

// Global Serial object
extern HardwareSerial Serial;

// Mock state tracking
extern uint8_t mock_pin_modes[MAX_PINS];
extern uint8_t mock_pin_states[MAX_PINS];
extern millis_t mock_millis;
extern PinStateChange mock_pin_history[MAX_PIN_HISTORY];
extern uint8_t mock_history_index;

// Mock Arduino functions
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t state);
uint8_t digitalRead(uint8_t pin);
void delay(unsigned long ms);
millis_t millis();

// Test helper functions
void resetMockState();
uint8_t getPinState(uint8_t pin);
uint8_t getPinMode(uint8_t pin);
void setMockMillis(millis_t value);
uint8_t getPinHistoryCount();
PinStateChange getPinHistoryEntry(uint8_t index);
void resetPinHistory();  // Reset only pin state change history
