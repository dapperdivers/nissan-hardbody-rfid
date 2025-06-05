# Hardware Components Guide

This document provides detailed information about each hardware component used in the Nissan Hardbody RFID system, including current usage and potential capabilities.

## Core Components

### 1. SparkFun Pro Micro (3.3V, 8MHz)

**Specifications:**
- Microcontroller: ATmega32U4
- Operating Voltage: 3.3V
- Clock Speed: 8MHz
- Flash Memory: 32KB (4KB used by bootloader)
- SRAM: 2.5KB
- EEPROM: 1KB
- Digital I/O Pins: 18
- PWM Channels: 7
- Analog Inputs: 9 (A0-A3, A6-A10)

**Current Usage:**
- Pin 10: RFID SS (Slave Select)
- Pin 9: Relay 1 (Door unlock)
- Pin 6: Relay 2
- Pin 5: Relay 3
- Pin 4: Relay 4
- Pin 8: DFPlayer RX
- Pin 7: DFPlayer TX / Wake interrupt
- ICSP Header: SPI for RFID

**Available Features Not Yet Used:**
- Pin 17 (RX LED) - Could be used for status indication
- Pin 30 (TX LED) - Additional status LED
- PWM pins - For LED brightness control or servo motors
- Analog pins - For battery monitoring, light sensors
- USB HID - Could act as keyboard/mouse for PC integration
- External interrupts (Pin 0,1,2,3,7) - Pin 7 ready for wake-up

**Future Potential:**
- Deep sleep modes for power saving
- USB communication for configuration
- I2C for additional sensors (SDA/SCL on pins 2/3)
- Hardware serial for better communication

### 2. PN532 NFC/RFID Module

**Specifications:**
- Operating Voltage: 3.3V-5V
- Communication: SPI, I2C, or HSU (High Speed UART)
- Supported Cards: Mifare 1K, 4K, Ultralight, DesFire, and more
- Read Range: 5-7cm typically
- Frequency: 13.56MHz
- Current Consumption: ~150mA peak, ~50mA average

**Current Usage:**
- SPI communication mode
- Reading Mifare Classic 4-byte and 7-byte UIDs
- Basic card presence detection

**Available Features Not Yet Used:**
- Card emulation mode
- Peer-to-peer communication
- Writing data to cards
- Reading card memory sectors
- Authentication with card keys
- Anti-collision for multiple cards
- Low power card detection mode

**Future Potential:**
- Store access levels on the card itself
- Use card memory for logging
- Implement rolling codes for enhanced security
- Clone card detection
- Mobile phone NFC support

### 3. DFPlayer Mini

**Specifications:**
- Operating Voltage: 3.2V-5V
- Supported formats: MP3, WAV, WMA
- Sampling rates: 8/11.025/12/16/22.05/24/32/44.1/48 kHz
- SD Card Support: Up to 32GB (FAT16, FAT32)
- 24-bit DAC output
- 30 volume levels
- Built-in 3W amplifier

**Current Usage:**
- Playing 6 audio tracks:
  1. Power-up sound
  2. "Are you still there" (10s timeout)
  3. Access granted
  4. Access denied (first attempt)
  5. Access denied (second attempt)
  6. Access denied (multiple attempts)
- Volume set to 10/30
- Serial communication at 9600 baud

**Available Features Not Yet Used:**
- Equalizer settings (Normal/Pop/Rock/Jazz/Classic/Bass)
- Repeat and shuffle modes
- Folder-based playback
- Sleep mode for power saving
- ADKey mode for button input
- Busy pin for status detection
- Direct speaker connection
- SD card for data storage (creative use)

**Future Potential:**
- Voice announcements for card holders
- Multi-language support
- Background music/ambience
- Emergency alarm sounds
- Status announcements
- Using SD card for configuration/logging

### 4. Relay Module (4-Channel)

**Specifications:**
- Channels: 4 independent relays
- Type: Active LOW (LOW signal = relay ON)
- Operating Voltage: 3.3V-5V logic
- Relay Ratings: Typically 10A @ 250VAC, 10A @ 30VDC
- Isolation: Optocoupler isolated
- Current per relay: ~70-80mA when active

**Current Usage:**
- Relay 1: Door lock/unlock (10-second activation)
- Relay 2-4: Currently unused

**Available Features Not Yet Used:**
- Relay 2: Could control lights
- Relay 3: Could control alarm/siren
- Relay 4: Could control auxiliary systems
- NO/NC contacts for fail-safe/fail-secure
- Status LED on each relay

**Future Potential:**
- Interior light control
- Engine start/stop
- Alarm system integration
- Garage door control
- Sequential activation patterns
- Power control for other modules

## Power Considerations

### Current Consumption Breakdown:
- Pro Micro: ~20mA (idle)
- PN532: ~50mA (average), 150mA (peak)
- DFPlayer: ~20mA (idle), 40mA (playing)
- Relays: ~80mA per active relay
- **Total**: ~90mA idle, ~200-300mA active

### Power Optimization Opportunities:
1. Pro Micro sleep mode: <1mA
2. PN532 power down: ~10μA
3. DFPlayer sleep mode: <20mA
4. Relay holding current reduction

## Expansion Possibilities

### I2C Bus (Pins 2,3):
- RTC module (DS3231) for time-based access
- OLED display for status
- Additional EEPROM storage
- Temperature/humidity sensors
- Accelerometer for tamper detection

### Remaining Digital Pins:
- Emergency button input
- Status LEDs
- Buzzer for local alerts
- PIR motion sensor
- Door position sensor

### Analog Inputs Available:
- Battery voltage monitoring
- Light sensor for automatic brightness
- Current sensing for each relay
- Potentiometer for volume control

### Serial/USB Options:
- Configuration software via USB
- Bluetooth module for wireless config
- GPS module for location tracking
- GSM module for remote alerts

## Wiring Diagram

```
Pro Micro 3.3V 8MHz
┌─────────────────┐
│                 │
│  10 ─────────── │──── PN532 SS
│  MISO ────────── │──── PN532 MISO
│  MOSI ────────── │──── PN532 MOSI
│  SCK ─────────── │──── PN532 SCK
│                 │
│  9 ──────────── │──── Relay 1 (Door)
│  6 ──────────── │──── Relay 2
│  5 ──────────── │──── Relay 3
│  4 ──────────── │──── Relay 4
│                 │
│  8 ──────────── │──── DFPlayer RX
│  7 ──────────── │──── DFPlayer TX
│                 │
│  3.3V ────────── │──── Power to modules
│  GND ─────────── │──── Common ground
│                 │
└─────────────────┘
```

## Best Practices

1. **Power Supply**: Use a stable 3.3V supply with adequate current
2. **Wiring**: Keep SPI lines short for RFID reliability
3. **Grounding**: Common ground for all components
4. **Protection**: Add flyback diodes on relay coils
5. **Shielding**: Keep RFID antenna away from metal/interference
6. **Audio**: Use shielded cable for audio output

## Troubleshooting Tips

- RFID not reading: Check SPI connections and SS pin
- Relays clicking rapidly: Check power supply current
- Audio distorted: Verify DFPlayer power and SD card format
- Random resets: Inadequate power supply or brown-out
- Communication errors: Check baud rates and wiring

This guide will be updated as new features are implemented from the roadmap.