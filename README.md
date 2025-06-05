# 🚗 Nissan Hardbody RFID Access Control System

[![PlatformIO CI](https://github.com/dapperdivers/nissan-hardbody-rfid/workflows/PlatformIO%20CI/badge.svg)](https://github.com/dapperdivers/nissan-hardbody-rfid/actions)

![Simple Sexy AC System](Simple%20Sexy%20AC.png)

A modern, modular RFID keyless entry system for the Nissan Hardbody truck, built on the SparkFun Pro Micro (3.3V, 8MHz) platform. This project takes the original SimpleSexyAC concept and transforms it into a robust, testable, and expandable access control system.

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
- Volume-controlled audio output

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
| **Hardware Utilization** | ⚠️ 30% | Significant untapped potential |
| **Documentation** | ✅ Extensive | 14 feature guides + architecture docs |
| **Power Efficiency** | ❌ Basic | Sleep modes not implemented |
| **Configuration** | ❌ Hardcoded | EEPROM storage not utilized |

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
- SparkFun Pro Micro (3.3V, 8MHz) - **⚠️ NOT the 5V version!**
- PN532 NFC/RFID module
- 4-Channel relay module  
- DFPlayer Mini + SD card (optional for audio)

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

3. Upload to your Pro Micro:

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
| **Microcontroller** | SparkFun Pro Micro 3.3V/8MHz | Basic I/O | Sleep modes, EEPROM, USB HID |
| **RFID Reader** | PN532 NFC Module | Read UIDs only | Write cards, encryption, NFC |
| **Relay Module** | 4-Channel 5V | 1 relay (door) | 3 unused channels |
| **Audio Module** | DFPlayer Mini | 6 sound effects | Folders, equalizer, data storage |

## 📈 Improvement Roadmap

### 🎯 Phase 1: Zero-Cost Enhancements (1-2 weeks)

1. **[LED Status Indicators](docs/features/01-led-status-indicators.md)** - Use built-in RX/TX LEDs
2. **[Low Power Sleep Mode](docs/features/11-low-power-sleep-mode.md)** - 99% power reduction
3. **[Master Card Programming](docs/features/06-master-card-programming.md)** - Add/remove cards without PC
4. **[EEPROM Configuration](docs/features/12-dfplayer-sd-persistence.md)** - Persistent settings

### 🔒 Phase 2: Enhanced Security (1-2 weeks)

5. **[Multiple Access Levels](docs/features/02-multiple-access-levels.md)** - Role-based permissions
6. **[Access Logging](docs/features/05-access-logging.md)** - Track all entries
7. **[Emergency Override](docs/features/08-emergency-override.md)** - Safety button

### ⚡ Phase 3: Hardware Additions (2-3 weeks)

8. **[Time-Based Access](docs/features/03-time-based-access.md)** - Schedule restrictions
9. **[Battery Backup](docs/features/09-battery-backup.md)** - Uninterrupted operation
10. **[Dual Authentication](docs/features/07-dual-authentication.md)** - Two-factor security

### 📱 Phase 4: Advanced Features (3-4 weeks)

11. **[Scheduled Relay Control](docs/features/14-scheduled-relay-control.md)** - Automated actions
12. **[Remote Management](docs/features/04-remote-management.md)** - WiFi/Bluetooth control
13. **[Mobile App Integration](docs/features/10-mobile-app-integration.md)** - Smartphone as key
14. **[Configurable Relay Modes](docs/features/13-configurable-relay-modes.md)** - Flexible outputs

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

Future versions will support EEPROM storage and master card programming.

## ⚡ Pin Connections

```txt
Pro Micro 3.3V ←→ Component Connections
├── SPI Bus ←→ PN532 RFID Module
│   ├── Pin 10 (SS)
│   └── ICSP Header (MOSI/MISO/SCK)
├── Digital Outputs ←→ 4-Channel Relay
│   ├── Pin 9 → Relay 1 (Door Lock)
│   ├── Pin 6 → Relay 2 (Available)
│   ├── Pin 5 → Relay 3 (Available)
│   └── Pin 4 → Relay 4 (Available)
└── Serial ←→ DFPlayer Mini (Optional)
    ├── Pin 8 → DFPlayer RX
    └── Pin 7 → DFPlayer TX
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

> **CRITICAL**: This project requires the **3.3V 8MHz Pro Micro**. Using a 5V version will damage the PN532 module!
> **CAUTION**: Incorrect board selection in PlatformIO can brick the Pro Micro. Always verify settings before upload.

## 🤝 Contributing

Contributions are welcome! Areas where help is especially appreciated:

- Testing new feature implementations
- PCB design for permanent installation
- 3D printed enclosure designs
- Security auditing
- Mobile app development

Please check the [Feature Roadmap](docs/features/feature-roadmap.md) for areas to contribute.

## 📄 License

MIT License - See [LICENSE](.github/LICENSE.md) for details

## 🙏 Acknowledgments

- Hardware created by **Chimpo**
- Original software: [SIMPLE-SEXY](https://github.com/chiplocks/SIMPLE-SEXY)
- RFID implants: [Dangerous Things](https://dangerousthings.com/)

---

**Made with ❤️ for the Dangerous Things community**
