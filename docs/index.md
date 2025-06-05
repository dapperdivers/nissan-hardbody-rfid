---
title: Home
layout: home
nav_order: 1
---

# Nissan Hardbody RFID Access Control Documentation
{: .fs-9 }

A modern, modular RFID keyless entry system built on the ESP32-C3 SuperMini platform.
{: .fs-6 .fw-300 }

[Get started](#getting-started){: .btn .btn-primary .fs-5 .mb-4 .mb-md-0 .mr-2 } [View on GitHub](https://github.com/dapperdivers/simple-sexy){: .btn .fs-5 .mb-4 .mb-md-0 }

---

## Getting Started

### Quick Links

- [Project Overview](PROJECT_OVERVIEW.html) - Complete system architecture and capabilities
- [Hardware Guide](HARDWARE_GUIDE.html) - Detailed component specifications
- [Current Features](CURRENT_FEATURES.html) - Implemented vs. possible features
- [Feature Roadmap](features/feature-roadmap.html) - Comprehensive enhancement guide

### About the Project

This project transforms the original SimpleSexyAC concept into a robust, testable, and expandable access control system for the Nissan Hardbody truck.

## Key Features

- **RFID Access Control** - Support for 4-byte and 7-byte UIDs
- **Audio Feedback** - 6 different sound effects for various states
- **Relay Control** - 4 relay outputs with automatic timeout
- **Modular Architecture** - Clean OOP design with comprehensive unit testing

## Hardware Components

| Component | Model | Purpose |
|-----------|-------|---------|
| **Microcontroller** | ESP32-C3 SuperMini | Main controller with WiFi/BLE |
| **RFID Reader** | PN532 NFC Module | Card reading |
| **Relay Module** | 4-Channel SRD-05VDC | Actuator control |
| **Audio Module** | JQ6500 MP3 Player | Sound feedback |
| **Power Supply** | Mini360 Buck Converter | 12V→5V conversion |

---

## Documentation Structure

### Core Documentation
Essential guides for understanding the system architecture and implementation.

### Feature Guides
Detailed implementation guides for various system enhancements, from simple LED indicators to advanced wireless features.

### Development
Technical documentation for developers including migration guides, pin mappings, and development best practices.