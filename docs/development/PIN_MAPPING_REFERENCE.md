# ESP32-C3 SuperMini Pin Mapping Reference Card

## Quick Reference

| Component | Function | ESP32-C3 Pin | Old Pro Micro Pin | Notes |
|-----------|----------|--------------|-------------------|-------|
| **PN532 RFID** | SS | GPIO10 | Pin 10 | Chip Select |
| | MISO | GPIO5 | MISO (ICSP) | SPI Data In |
| | MOSI | GPIO6 | MOSI (ICSP) | SPI Data Out |
| | SCK | GPIO4 | SCK (ICSP) | SPI Clock |
| **JQ6500** | TX | GPIO1 | Pin 7 | To JQ6500 RX |
| | RX | GPIO0 | Pin 8 | From JQ6500 TX |
| **Relays** | RELAY1 | GPIO9 | Pin 9 | Door Lock |
| | RELAY2 | GPIO3 | Pin 6 | Aux 1 |
| | RELAY3 | GPIO2 | Pin 5 | Aux 2 |
| | RELAY4 | GPIO7 | Pin 4 | Aux 3 |
| **Status** | Blue LED | GPIO8 | Pin 17 (RX LED) | Built-in |
| **Power** | 5V | 5V | RAW/VCC | From Mini360 |
| | 3.3V | 3.3V | 3.3V | Internal LDO |
| | GND | GND | GND | Common Ground |

## Visual Pinout Diagram

```
          ESP32-C3 SuperMini
         ┌─────────────────┐
    5V ──┤ 5V         GND ├── GND
         │                 │
GPIO10 ──┤ 10  (SS)     9 ├── GPIO9 (RELAY1)
GPIO8  ──┤ 8   (LED)    6 ├── GPIO6 (MOSI)
GPIO7  ──┤ 7   (REL4)   5 ├── GPIO5 (MISO)
GPIO4  ──┤ 4   (SCK)    3 ├── GPIO3 (RELAY2)
GPIO2  ──┤ 2   (REL3)   1 ├── GPIO1 (JQ6500 TX)
GPIO0  ──┤ 0   (RX)   3V3 ├── 3.3V Out
         └─────────────────┘
              USB-C
```

## Code Pin Definitions

```cpp
// ESP32-C3 SuperMini Pin Definitions
// PN532 RFID Reader (SPI)
#define PN532_SCK   4
#define PN532_MISO  5
#define PN532_MOSI  6
#define PN532_SS    10

// JQ6500 MP3 Player (UART)
#define JQ6500_RX   0   // ESP32 RX = JQ6500 TX
#define JQ6500_TX   1   // ESP32 TX = JQ6500 RX

// Relay Outputs (Active LOW)
#define RELAY1      9   // Door Lock
#define RELAY2      3   // Auxiliary 1
#define RELAY3      2   // Auxiliary 2  
#define RELAY4      7   // Auxiliary 3

// Status LED
#define BLUE_LED    8   // Built-in blue LED
```

## Important Notes

### ESP32-C3 SuperMini Limitations
- Only 11 GPIO pins (0-10)
- Some pins have special functions at boot
- GPIO8 has built-in blue LED
- No pins 11-21 like larger ESP32 boards

### Pin Considerations
- **GPIO0**: Strapping pin - must be HIGH for normal boot
- **GPIO2**: Strapping pin - affects boot mode
- **GPIO8**: Has built-in blue LED, active HIGH
- **GPIO9**: Strapping pin - must be HIGH during reset

### Power Requirements
- Board operates at 3.3V internally
- 5V input via USB-C or 5V pin
- All GPIO are 3.3V logic levels
- Mini360 provides stable 5V from 12V vehicle power

### SPI Bus Notes
- Hardware SPI uses GPIO 4/5/6
- Multiple devices can share MISO/MOSI/SCK
- Each device needs unique SS (Chip Select) pin

### UART Notes  
- ESP32-C3 has 2 UART interfaces
- Default Serial uses USB-C for debugging
- Hardware Serial1 available for JQ6500

## Wiring Colors (Suggested)
- **Red**: 5V Power
- **Black**: Ground
- **Yellow**: SPI Clock (SCK)
- **Green**: SPI MOSI
- **Blue**: SPI MISO
- **White**: SPI SS
- **Orange**: UART TX
- **Brown**: UART RX
- **Purple**: Relay controls