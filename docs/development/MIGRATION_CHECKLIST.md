# ESP32-C3 Migration Checklist

This checklist guides the code migration from Pro Micro + DFPlayer to ESP32-C3 + JQ6500.

## Phase 1: Platform Configuration ✅
- [x] Update `platformio.ini` for ESP32-C3 board
- [x] Update library dependencies (JQ6500_Serial)
- [x] Configure build flags for ESP32-C3
- [x] Set monitor speed to 115200

## Phase 2: Pin Definitions Update 🔄
- [x] Update pin definitions in all header files
- [x] Change PN532 SS pin from 10 to 5 (ESP32-C3)
- [x] Update relay pins (GPIO 9, 10, 20, 21 for ESP32-C3)
- [ ] Add blue LED pin definition (GPIO8)
- [x] Update Serial1 pins for JQ6500 (0,1)

## Phase 3: Audio Module Migration 🔄
- [x] Replace DFPlayer includes with JQ6500
- [x] Update audio_player.h for JQ6500
- [~] Update audio_player.cpp implementation (paused - compilation issues)
- [x] Add MP3 reset on startup
- [x] Update volume to 20 (from 10)
- [x] Remove SoftwareSerial, use HardwareSerial

## Phase 4: Core Functionality Updates ✅
- [x] Update main.cpp for ESP32-C3
- [x] Fix Serial1 initialization (pins 0,1)
- [ ] Update SPI initialization if needed
- [x] Add relay sequential activation pattern
- [x] Verify timing matches manufacturer code

## Phase 5: Test Files Update 🔄
- [x] Update mock libraries for JQ6500
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
- audio_player.cpp has compilation issues that need to be resolved

## Progress Summary:
- ✅ Phase 1: Complete
- 🔄 Phase 2: 80% (missing LED definition)
- 🔄 Phase 3: 85% (audio_player.cpp issues)
- ✅ Phase 4: 80% (may need SPI updates)
- 🔄 Phase 5: 33% (mock created, tests not updated)
- ⏳ Phase 6: Not started
- ⏳ Phase 7: Not started