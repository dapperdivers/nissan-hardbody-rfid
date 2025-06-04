#pragma once
#include <stdint.h>

// SPI settings
#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV32 0x06
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03

#define MSBFIRST 1
#define LSBFIRST 0


class MockSPI {
public:
    void begin() {}
    void end() {}
    void reset() {}
    uint8_t transfer(uint8_t) { return 0; }
};