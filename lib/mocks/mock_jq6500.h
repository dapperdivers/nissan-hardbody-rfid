#pragma once

#include <cstdint>

// Mock JQ6500_Serial class for unit testing
class JQ6500_Serial {
public:
    JQ6500_Serial(HardwareSerial& serial) : serial_(serial), volume_(10), is_playing_(false) {}
    
    void reset() {
        // Mock reset functionality
        is_playing_ = false;
    }
    
    void setVolume(uint8_t volume) {
        volume_ = volume;
    }
    
    uint8_t getVolume() const {
        return volume_;
    }
    
    void playFileByIndexNumber(uint8_t fileNumber) {
        last_played_track_ = fileNumber;
        is_playing_ = true;
    }
    
    bool isPlaying() const {
        return is_playing_;
    }
    
    uint8_t getLastPlayedTrack() const {
        return last_played_track_;
    }
    
private:
    HardwareSerial& serial_;
    uint8_t volume_;
    bool is_playing_;
    uint8_t last_played_track_;
};

// Mock HardwareSerial for unit testing
class HardwareSerial {
public:
    HardwareSerial(int uart_nr) : uart_nr_(uart_nr) {}
    
    void begin(unsigned long baud, uint32_t config = 0x800001c, int8_t rxPin = -1, int8_t txPin = -1) {
        baud_rate_ = baud;
        rx_pin_ = rxPin;
        tx_pin_ = txPin;
    }
    
    void write(uint8_t data) {
        // Mock write
    }
    
    int available() {
        return 0;
    }
    
    int read() {
        return -1;
    }
    
    unsigned long getBaudRate() const {
        return baud_rate_;
    }
    
    int8_t getRxPin() const {
        return rx_pin_;
    }
    
    int8_t getTxPin() const {
        return tx_pin_;
    }
    
private:
    int uart_nr_;
    unsigned long baud_rate_ = 0;
    int8_t rx_pin_ = -1;
    int8_t tx_pin_ = -1;
};