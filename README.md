# 🚗 Nissan Hardbody RFID Access Control System

[![PlatformIO CI](https://github.com/dapperdivers/nissan-hardbody-rfid/workflows/PlatformIO%20CI/badge.svg)](https://github.com/dapperdivers/nissan-hardbody-rfid/actions)
[![codecov](https://codecov.io/gh/dapperdivers/nissan-hardbody-rfid/branch/main/graph/badge.svg)](https://codecov.io/gh/dapperdivers/nissan-hardbody-rfid)

![Simple Sexy AC System](Simple%20Sexy%20AC%20V2.png)

A modern, modular RFID keyless entry system for the Nissan Hardbody truck, built on the ESP32-C3 SuperMini platform. This project takes the original SimpleSexyAC concept and transforms it into a robust, testable, and expandable access control system with WiFi and Bluetooth capabilities.

## ✨ Key Features

### 🔐 **Secure Access Control**

- RFID card reading with support for 4-byte and 7-byte UIDs
- Brute-force protection with progressive delays (1-68 seconds)
- 10-second door unlock on valid card presentation
- Anti-passback protection through delay mechanism

### 🔊 **Audio Feedback System**

- 6 different sound effects for various states
- Power-up confirmation
- "Are you still there?" prompt
- Multiple access denied levels
- JQ6500 MP3 player module for reliable audio

### 📡 **Wireless Connectivity**

- WiFi 802.11 b/g/n for remote management
- Bluetooth 5.0 (BLE) for mobile integration
- OTA (Over-The-Air) update capability
- Web-based configuration portal (future)

### 🏗️ **Modern Architecture**

- **Modular OOP Design** - Clean separation of concerns
- **Comprehensive Unit Testing** - Full test coverage with Unity framework
- **Mock Objects** - Hardware abstraction for development
- **PlatformIO Based** - Modern embedded development workflow

## 📊 Project Status

| Aspect | Status | Details |
|--------|--------|---------|
| **Core Functionality** | ✅ Complete | RFID reading, relay control, audio feedback |
| **Code Quality** | ✅ Excellent | Modular OOP, unit tested, well-documented |
| **Hardware Utilization** | ⚠️ 20% | Significant untapped potential (WiFi/BLE unused) |
| **Documentation** | ✅ Extensive | 14 feature guides + architecture docs |
| **Power Efficiency** | ❌ Basic | Deep sleep modes not implemented |
| **Configuration** | ❌ Hardcoded | Flash storage not utilized |

## 📚 Comprehensive Documentation

### Core Documentation

- **[Project Overview](docs/PROJECT_OVERVIEW.md)** - Complete system architecture and capabilities
- **[Hardware Guide](docs/HARDWARE_GUIDE.md)** - Detailed component specifications  
- **[Current Features](docs/CURRENT_FEATURES.md)** - Implemented vs. possible features

### Feature Enhancement Guides

- **[Feature Roadmap](docs/features/feature-roadmap.md)** - 14 enhancements with implementation timeline
- **[Feature Documentation](docs/features/)** - Detailed guides for each enhancement

## 🚀 Quick Start

### Prerequisites

- PlatformIO Core or IDE
- ESP32-C3 SuperMini board
- PN532 NFC/RFID module
- 4-Channel relay module (SRD-05VDC-SL-C)
- JQ6500 MP3 player module + speaker (optional for audio)
- Mini360 DC-DC buck converter (12V to 5V)

### Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/dapperdivers/nissan-hardbody-rfid.git
   cd nissan-hardbody-rfid
   ```

2. Build the project:

   ```bash
   pio run
   ```

3. Upload to your ESP32-C3:

   ```bash
   pio run -t upload
   ```

4. Run unit tests:

   ```bash
   pio test
   ```

## 🛠️ Hardware Components

| Component | Model | Current Use | Potential |
|-----------|-------|-------------|-----------|
| **Microcontroller** | ESP32-C3 SuperMini | Basic I/O | WiFi, BLE, deep sleep (43μA) |
| **RFID Reader** | PN532 NFC Module | Read UIDs only | Write cards, encryption, NFC |
| **Relay Module** | 4-Channel SRD-05VDC | 1 relay (door) | 3 unused channels |
| **Audio Module** | JQ6500 MP3 Player | 6 sound effects | Multiple folders, status |
| **Power Supply** | Mini360 Buck Converter | 12V→5V conversion | 96% efficiency, protection |

## 📈 Improvement Roadmap

### 🎯 Phase 1: Zero-Cost Enhancements (1-2 weeks)

1. **[LED Status Indicators](docs/features/01-led-status-indicators.md)** - Use built-in blue LED
2. **[Low Power Sleep Mode](docs/features/11-low-power-sleep-mode.md)** - 43μA deep sleep
3. **[Master Card Programming](docs/features/06-master-card-programming.md)** - Add/remove cards without PC
4. **[Flash Storage](docs/features/12-dfplayer-sd-persistence.md)** - Persistent settings in 4MB flash

### 🔒 Phase 2: Wireless Features (1-2 weeks)

5. **[WiFi Portal](docs/features/04-remote-management.md)** - Web-based configuration
6. **[BLE Integration](docs/features/10-mobile-app-integration.md)** - Smartphone as key
7. **[OTA Updates](docs/features/04-remote-management.md)** - Remote firmware updates
8. **[MQTT Support](docs/features/04-remote-management.md)** - Home automation integration

### ⚡ Phase 3: Enhanced Security (2-3 weeks)

9. **[Multiple Access Levels](docs/features/02-multiple-access-levels.md)** - Role-based permissions
10. **[Access Logging](docs/features/05-access-logging.md)** - Track all entries
11. **[Time-Based Access](docs/features/03-time-based-access.md)** - Schedule restrictions
12. **[Emergency Override](docs/features/08-emergency-override.md)** - Safety button

### 📱 Phase 4: Advanced Features (3-4 weeks)

13. **[Battery Backup](docs/features/09-battery-backup.md)** - Uninterrupted operation
14. **[Dual Authentication](docs/features/07-dual-authentication.md)** - Two-factor security
15. **[Scheduled Relay Control](docs/features/14-scheduled-relay-control.md)** - Automated actions
16. **[Configurable Relay Modes](docs/features/13-configurable-relay-modes.md)** - Flexible outputs

## 🏗️ Project Structure

```txt
nissan-hardbody-rfid/
├── docs/                    # Comprehensive documentation
│   ├── features/           # 14 enhancement guides
│   ├── PROJECT_OVERVIEW.md # System architecture
│   ├── HARDWARE_GUIDE.md   # Component details
│   └── CURRENT_FEATURES.md # Feature comparison
├── include/                # Header files
│   ├── rfid_controller.h
│   ├── relay_controller.h
│   └── audio_player.h
├── src/                    # Implementation files
│   ├── main.cpp
│   ├── rfid_controller.cpp
│   ├── relay_controller.cpp
│   └── audio_player.cpp
├── lib/                    # Mock libraries for testing
│   └── mocks/
├── test/                   # Comprehensive unit tests
│   └── test_native/
└── platformio.ini         # Build configuration
```

## 🔧 Configuration

Currently, authorized UIDs must be hardcoded in `src/rfid_controller.cpp`:

```cpp
void RFIDController::initializeDefaultUIDs() {
    // Add your UIDs here
}
```

Future versions will support flash storage and master card programming.

## ⚡ Pin Connections

```txt
ESP32-C3 SuperMini ←→ Component Connections
├── SPI Bus ←→ PN532 RFID Module
│   ├── Pin 10 (SS)
│   └── MISO/MOSI/SCK pins
├── Digital Outputs ←→ 4-Channel Relay
│   ├── Pin 9 → Relay 1 (Door Lock)
│   ├── Pin 6 → Relay 2 (Available)
│   ├── Pin 5 → Relay 3 (Available)
│   └── Pin 4 → Relay 4 (Available)
├── Serial ←→ JQ6500 MP3 Player (Optional)
│   ├── Pin 1 → JQ6500 RX
│   └── Pin 0 → JQ6500 TX
└── Power ←→ Mini360 Buck Converter
    ├── 5V → System power
    └── GND → Common ground
```

## 🧪 Testing

The project includes comprehensive unit tests for all modules:

```bash
# Run all tests
pio test

# Run specific test
pio test -f test_relay

# Test with verbose output
pio test -v
```

## ⚠️ Important Warnings

> **IMPORTANT**: The ESP32-C3 operates at 3.3V logic levels. The system uses 5V power through the Mini360 converter for relay compatibility.

> **CRITICAL**: Ensure proper voltage settings on the Mini360 buck converter (5V) before connecting to the system.

## 🤝 Contributing

Contributions are welcome! Areas where help is especially appreciated:

- Testing WiFi/BLE features
- Web dashboard development
- Mobile app development (iOS/Android)
- PCB design for permanent installation
- 3D printed enclosure designs
- Security auditing

Please check the [Feature Roadmap](docs/features/feature-roadmap.md) for areas to contribute.

## 📄 License

MIT License - See [LICENSE](.github/LICENSE.md) for details

## 🙏 Acknowledgments

- Hardware created by **Chimpo**
- Original software: [SIMPLE-SEXY](https://github.com/chiplocks/SIMPLE-SEXY)
- RFID implants: [Dangerous Things](https://dangerousthings.com/)

---

**Made with ❤️ for the Dangerous Things community**
