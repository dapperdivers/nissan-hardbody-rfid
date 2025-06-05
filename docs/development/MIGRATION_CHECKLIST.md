# ESP32-C3 Migration Checklist

This checklist guides the code migration from Pro Micro + DFPlayer to ESP32-C3 + JQ6500.

## Phase 1: Platform Configuration
- [ ] Update `platformio.ini` for ESP32-C3 board
- [ ] Update library dependencies (JQ6500_Serial)
- [ ] Configure build flags for ESP32-C3
- [ ] Set monitor speed to 115200

## Phase 2: Pin Definitions Update
- [ ] Update pin definitions in all header files
- [ ] Change PN532 SS pin from 7 to 10
- [ ] Update relay pins (0→9, 1→3, 2→2, 3→7)
- [ ] Add blue LED pin definition (GPIO8)
- [ ] Update Serial1 pins for JQ6500 (0,1)

## Phase 3: Audio Module Migration
- [ ] Replace DFPlayer includes with JQ6500
- [ ] Update audio_player.h for JQ6500
- [ ] Update audio_player.cpp implementation
- [ ] Add MP3 reset on startup
- [ ] Update volume to 20 (from 10)
- [ ] Remove SoftwareSerial, use HardwareSerial

## Phase 4: Core Functionality Updates
- [ ] Update main.cpp for ESP32-C3
- [ ] Fix Serial1 initialization (pins 0,1)
- [ ] Update SPI initialization if needed
- [ ] Add relay sequential activation pattern
- [ ] Verify timing matches manufacturer code

## Phase 5: Test Files Update
- [ ] Update mock libraries for JQ6500
- [ ] Update test cases for new pins
- [ ] Ensure tests compile for native platform

## Phase 6: New ESP32 Features (Optional)
- [ ] Add WiFi initialization stub
- [ ] Add BLE initialization stub
- [ ] Add deep sleep mode stub
- [ ] Add flash storage stub

## Phase 7: Testing & Validation
- [ ] Compile test for ESP32-C3
- [ ] Run unit tests
- [ ] Document any breaking changes
- [ ] Update README with new requirements

## Notes:
- Manufacturer code has pins 20,21 which don't exist on ESP32-C3 SuperMini
- We'll use pins 0,1 for Serial1 as documented
- Keep backward compatibility where possible