# RFID Access Control System

A PlatformIO-based project for the Nissan Hardbody RFID access control system using a 3.3V 8MHz Pro Micro.

## Hardware Requirements

- SparkFun Pro Micro (3.3V, 8MHz version only)
- PN532 NFC/RFID module
- 4x Relays (Active LOW)
- Optional: DFPlayer Mini for audio feedback

## Dependencies

- Adafruit PN532 Library
- DFPlayerMini_Fast (optional)

## Project Structure

```
.
├── include/
│   ├── rfid_controller.h
│   ├── relay_controller.h
│   └── audio_player.h
├── src/
│   ├── main.cpp
│   ├── rfid_controller.cpp
│   ├── relay_controller.cpp
│   └── audio_player.cpp
├── test/
│   └── test_relay_controller.cpp
└── platformio.ini
```

## Setup

1. Install PlatformIO Core or PlatformIO IDE
2. Clone this repository
3. Build the project:
   ```bash
   pio run
   ```
4. Upload to your Pro Micro:
   ```bash
   pio run -t upload
   ```
5. Run tests:
   ```bash
   pio test
   ```

## Configuration

### RFID Cards

To add authorized RFID cards, modify the UIDs in `src/main.cpp`:

```cpp
// For 4-byte UIDs
rfid.addUID4B(uint8_t[4] { 0x00, 0x00, 0x00, 0x00 });

// For 7-byte UIDs
rfid.addUID7B(uint8_t[7] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });
```

### Audio Support

To enable DFPlayer support, uncomment the following line in `platformio.ini`:

```ini
build_flags = -D ENABLE_DFPLAYER
```

## Pin Configuration

- RFID Module (PN532):
  - SS: Pin 10
  - MOSI: ICSP
  - MISO: ICSP
  - SCK: ICSP

- Relays:
  - Relay 1: Pin 9
  - Relay 2: Pin 6
  - Relay 3: Pin 5
  - Relay 4: Pin 4

- DFPlayer (optional):
  - RX: Pin 8
  - TX: Pin 7

## Testing

The project includes unit tests using Unity framework. Run the tests with:

```bash
pio test
```

## License

MIT License - Feel free to use and modify as needed.

## Warning

⚠️ This project is specifically designed for the 3.3V 8MHz Pro Micro. Using a 5V version will damage the board.
