# Hardware Migration Guide: Pro Micro to ESP32-C3 SuperMini

This guide provides step-by-step instructions for migrating from the SparkFun Pro Micro + DFPlayer Mini setup to the new ESP32-C3 SuperMini + JQ6500 hardware configuration.

## Table of Contents
1. [Hardware Differences](#hardware-differences)
2. [Pin Mapping Changes](#pin-mapping-changes)
3. [Code Migration Steps](#code-migration-steps)
4. [Library Changes](#library-changes)
5. [New Features Available](#new-features-available)
6. [Testing Checklist](#testing-checklist)

## Hardware Differences

### Microcontroller Comparison
| Feature | Pro Micro (ATmega32U4) | ESP32-C3 SuperMini |
|---------|------------------------|-------------------|
| Architecture | 8-bit AVR | 32-bit RISC-V |
| Clock Speed | 8MHz | 160MHz |
| RAM | 2.5KB | 400KB |
| Flash | 32KB | 4MB |
| GPIO Pins | 18 | 11 |
| WiFi | No | Yes (802.11 b/g/n) |
| Bluetooth | No | Yes (BLE 5.0) |
| Deep Sleep | ~200μA | 43μA |
| Operating Voltage | 3.3V | 3.3V |
| USB | Micro-USB | USB-C |

### Audio Module Comparison
| Feature | DFPlayer Mini | JQ6500 |
|---------|--------------|---------|
| Communication | UART (9600 baud) | UART (9600 baud) |
| Audio Formats | MP3, WAV, WMA | MP3, WAV |
| Volume Levels | 30 | 30 |
| Power | 3.2-5V | 3.3-5V |
| Current (Playing) | 40mA | 200mA (with speaker) |
| SD Card | Required | Optional (has onboard flash) |

## Pin Mapping Changes

### Critical Pin Reassignments

```cpp
// OLD (Pro Micro) Pin Definitions from manufacturer code
#define PN532_SCK   4    // SPI Clock
#define PN532_MISO  5    // SPI MISO
#define PN532_MOSI  6    // SPI MOSI
#define PN532_SS    7    // SPI Chip Select
// Serial1 pins 20,21 (don't exist on ESP32-C3!)

// NEW (ESP32-C3) Pin Definitions
#define PN532_SCK   4    // GPIO4 - SPI Clock
#define PN532_MISO  5    // GPIO5 - SPI MISO
#define PN532_MOSI  6    // GPIO6 - SPI MOSI
#define PN532_SS    10   // GPIO10 - SPI Chip Select (moved from 7)
// Serial1 pins 0,1 for JQ6500
```

### Complete Pin Mapping Table
See [PIN_MAPPING_REFERENCE.md](../PIN_MAPPING_REFERENCE.md) for visual diagram.

| Component | Old Pin | New Pin | Change Notes |
|-----------|---------|---------|--------------|
| PN532 SS | 7 | GPIO10 | Moved to avoid conflict |
| JQ6500 TX | 20 | GPIO1 | ESP32-C3 Serial1 |
| JQ6500 RX | 21 | GPIO0 | ESP32-C3 Serial1 |
| RELAY1 | 0 | GPIO9 | Safer pin choice |
| RELAY2 | 1 | GPIO3 | Avoid boot pins |
| RELAY3 | 2 | GPIO2 | Strapping pin (use carefully) |
| RELAY4 | 3 | GPIO7 | General GPIO |

## Code Migration Steps

### Step 1: Update Board Configuration

**platformio.ini changes:**
```ini
; OLD Configuration
[env:promicro]
platform = atmelavr
board = sparkfun_promicro8
framework = arduino

; NEW Configuration
[env:esp32c3]
platform = espressif32
board = esp32-c3-devkitm-1
framework = arduino
monitor_speed = 115200
lib_deps = 
    adafruit/Adafruit PN532@^1.3.0
    sleemanj/JQ6500_Serial@^1.0.0
```

### Step 2: Update Pin Definitions

**In your main code:**
```cpp
// Remove old definitions
// #define PN532_SCK (4)
// #define PN532_MOSI (6)
// #define PN532_SS (7)
// #define PN532_MISO (5)

// Add new definitions for ESP32-C3
#define PN532_SCK   4
#define PN532_MISO  5
#define PN532_MOSI  6
#define PN532_SS    10  // Changed from 7 to 10

#define RELAY1      9   // Changed from 0
#define RELAY2      3   // Changed from 1
#define RELAY3      2   // Kept same (be aware of boot behavior)
#define RELAY4      7   // Changed from 3

#define BLUE_LED    8   // New - built-in LED
```

### Step 3: Update Serial Configuration

**OLD Code (Pro Micro):**
```cpp
HardwareSerial MySerial(1); // Use Serial1
JQ6500_Serial mp3(MySerial);

void setup() {
    Serial.begin(115200);
    MySerial.begin(9600, SERIAL_8N1, 20, 21); // Pins 20,21 don't exist!
}
```

**NEW Code (ESP32-C3):**
```cpp
HardwareSerial MySerial(1); // Use Serial1
JQ6500_Serial mp3(MySerial);

void setup() {
    Serial.begin(115200);
    // ESP32-C3 Serial1 uses GPIO0 (RX) and GPIO1 (TX)
    MySerial.begin(9600, SERIAL_8N1, 0, 1); // RX=GPIO0, TX=GPIO1
}
```

### Step 4: Add ESP32-Specific Features

**Power Management:**
```cpp
// Add deep sleep capability
void enterDeepSleep(uint32_t seconds) {
    esp_sleep_enable_timer_wakeup(seconds * 1000000ULL);
    esp_deep_sleep_start();
}

// Add wake on pin
void setupWakeOnPin() {
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0); // Wake when GPIO0 goes LOW
}
```

**WiFi Setup (New Feature!):**
```cpp
#include <WiFi.h>

void setupWiFi() {
    WiFi.begin("your-ssid", "your-password");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected!");
    Serial.println(WiFi.localIP());
}
```

### Step 5: Update Audio Player Code

The JQ6500 library usage remains similar, but initialization differs slightly:

```cpp
// Audio feedback helper functions
void playAccessGranted() {
    mp3.playFileByIndexNumber(3);  // Same as before
}

void playAccessDenied(uint8_t attempt) {
    if (attempt == 0) {
        mp3.playFileByIndexNumber(4);
    } else if (attempt == 1) {
        mp3.playFileByIndexNumber(5);
    } else {
        mp3.playFileByIndexNumber(6);
    }
}
```

## Library Changes

### Remove Old Libraries
- `DFPlayerMini_Fast`
- `SoftwareSerial` (not needed, ESP32 has multiple hardware serials)

### Add/Update Libraries
- `JQ6500_Serial` by sleemanj
- `Adafruit PN532` (same library, works with ESP32)
- ESP32 core libraries (automatically included)

### Library Installation
```bash
# Using PlatformIO
pio lib install "sleemanj/JQ6500_Serial"
pio lib install "adafruit/Adafruit PN532"

# Or add to platformio.ini as shown above
```

## New Features Available

With ESP32-C3, you now have access to:

### 1. Built-in Storage
```cpp
#include <Preferences.h>
Preferences preferences;

void saveUID(uint8_t* uid, uint8_t length) {
    preferences.begin("rfid", false);
    preferences.putBytes("lastUID", uid, length);
    preferences.end();
}
```

### 2. Status LED
```cpp
void setupStatusLED() {
    pinMode(BLUE_LED, OUTPUT);
}

void blinkLED(int times) {
    for(int i = 0; i < times; i++) {
        digitalWrite(BLUE_LED, HIGH);
        delay(200);
        digitalWrite(BLUE_LED, LOW);
        delay(200);
    }
}
```

### 3. Web Server
```cpp
#include <WebServer.h>
WebServer server(80);

void setupWebServer() {
    server.on("/", []() {
        server.send(200, "text/plain", "RFID System OK");
    });
    server.on("/unlock", []() {
        digitalWrite(RELAY1, LOW);
        delay(5000);
        digitalWrite(RELAY1, HIGH);
        server.send(200, "text/plain", "Door Unlocked");
    });
    server.begin();
}
```

### 4. OTA Updates
```cpp
#include <ArduinoOTA.h>

void setupOTA() {
    ArduinoOTA.setHostname("rfid-system");
    ArduinoOTA.begin();
}

// In loop()
void loop() {
    ArduinoOTA.handle();
    // ... rest of code
}
```

## Testing Checklist

### Hardware Testing
- [ ] Verify 5V power from Mini360 buck converter
- [ ] Check 3.3V output on ESP32-C3
- [ ] Test each relay individually
- [ ] Verify RFID reader communication
- [ ] Test audio playback on all 6 tracks
- [ ] Check blue LED functionality

### Software Testing
- [ ] Upload test sketch via USB-C
- [ ] Verify Serial Monitor at 115200 baud
- [ ] Test RFID card reading (4-byte and 7-byte)
- [ ] Verify relay activation sequence
- [ ] Test brute-force protection delays
- [ ] Check audio volume levels

### New Feature Testing
- [ ] Test deep sleep and wake
- [ ] Verify WiFi connection
- [ ] Test web server endpoints
- [ ] Try OTA firmware update
- [ ] Check flash storage persistence

## Common Migration Issues

### Issue 1: Relays trigger on boot
**Solution:** ESP32 pins can have different boot states. Add pull-up resistors or adjust initialization:
```cpp
void setup() {
    // Set relay pins HIGH before setting as OUTPUT
    digitalWrite(RELAY1, HIGH);
    digitalWrite(RELAY2, HIGH);
    digitalWrite(RELAY3, HIGH);
    digitalWrite(RELAY4, HIGH);
    
    pinMode(RELAY1, OUTPUT);
    pinMode(RELAY2, OUTPUT);
    pinMode(RELAY3, OUTPUT);
    pinMode(RELAY4, OUTPUT);
}
```

### Issue 2: RFID not responding
**Solution:** ESP32-C3 SPI might need different initialization:
```cpp
SPI.begin(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);
```

### Issue 3: Serial output garbled
**Solution:** Ensure baud rate is 115200 for ESP32:
```cpp
Serial.begin(115200);  // Not 9600!
```

### Issue 4: High power consumption
**Solution:** Implement sleep modes:
```cpp
// In loop when idle
if (millis() - lastActivity > 30000) {  // 30 seconds
    enterDeepSleep(3600);  // Sleep for 1 hour
}
```

## Conclusion

The migration to ESP32-C3 SuperMini provides significant improvements:
- 20x faster processing
- Built-in WiFi and Bluetooth
- More memory for features
- Better power efficiency
- Modern development experience

Follow this guide step-by-step, test thoroughly, and enjoy the enhanced capabilities of your upgraded RFID system!