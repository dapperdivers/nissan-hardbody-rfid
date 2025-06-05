# Feature: Battery Backup

**Complexity**: 🟡 Medium  
**Hardware Required**: 🏗️ Major (Battery, charging circuit)  
**User Value**: ⭐⭐⭐ Essential

## Overview

Implement battery backup to maintain access control functionality during power outages. This ensures security is maintained and authorized users can still enter during emergencies.

## Benefits

- Continuous operation during power outages
- Security maintained 24/7
- Emergency access preserved
- System status preservation
- Compliance with security standards

## Power Requirements Analysis

### Current Consumption Estimates
- Arduino Pro Micro (3.3V): ~20mA idle
- PN532 RFID Reader: ~50mA active, ~10mA standby
- DFPlayer Mini: ~20mA idle, ~40mA playing
- Relays (4x): ~80mA when active
- **Total**: ~80mA idle, ~200mA peak

### Battery Runtime Calculations
- 2000mAh battery: ~25 hours idle, ~10 hours active use
- 5000mAh battery: ~62 hours idle, ~25 hours active use
- 10000mAh battery: ~125 hours idle, ~50 hours active use

## Implementation Checklist

### Phase 1: Power Management System
- [ ] Create `PowerController` class:
  ```cpp
  class PowerController {
  private:
      uint8_t batteryPin;
      uint8_t chargerPin;
      uint8_t powerGoodPin;
      float batteryVoltage;
      bool onBattery;
  public:
      void begin();
      float getBatteryVoltage();
      uint8_t getBatteryPercent();
      bool isOnBattery();
      bool isCharging();
      void enablePowerSaving();
  };
  ```
- [ ] Voltage monitoring circuit
- [ ] Power source detection
- [ ] Automatic switchover

### Phase 2: Battery System Hardware
- [ ] **Battery Options**:
  - [ ] 18650 Li-ion cells (3.7V, 2000-3500mAh)
  - [ ] LiPo battery pack (3.7V-7.4V)
  - [ ] Sealed Lead Acid (6V/12V)
  - [ ] NiMH rechargeable pack
  
- [ ] **Charging Circuit**:
  - [ ] TP4056 for single Li-ion
  - [ ] MCP73831 for LiPo
  - [ ] Smart charger for SLA
  - [ ] Protection circuit (overcharge/discharge)

### Phase 3: Power Switching
- [ ] **Automatic Switchover**:
  ```cpp
  // Diode-OR circuit for seamless switching
  Main Power ---|>|---+
                      |--- To System
  Battery -------|>|--+
  
  // Or use power management IC like LTC4412
  ```
- [ ] Zero-downtime switching
- [ ] Reverse polarity protection
- [ ] Surge protection

### Phase 4: Power Saving Features
- [ ] **Low Power Modes**:
  ```cpp
  void enterPowerSaving() {
      // Reduce RFID scan frequency
      scanInterval = 500;  // ms (vs normal 100ms)
      
      // Dim/disable LEDs
      ledController.setBrightness(10);
      
      // Reduce audio volume
      audio.setVolume(5);
      
      // Disable non-essential features
      wifiEnabled = false;
  }
  ```
- [ ] Sleep between card scans
- [ ] Reduced LED brightness
- [ ] Lower audio volume
- [ ] Disable WiFi/Bluetooth

### Phase 5: Battery Monitoring
- [ ] **Voltage Monitoring**:
  ```cpp
  float readBatteryVoltage() {
      // Voltage divider: Battery -> R1 -> ADC -> R2 -> GND
      int raw = analogRead(BATTERY_PIN);
      float voltage = (raw * 3.3 / 1023.0) * ((R1 + R2) / R2);
      return voltage;
  }
  ```
- [ ] Low battery warnings:
  - [ ] 20%: Yellow LED warning
  - [ ] 10%: Red LED + audio alert
  - [ ] 5%: Emergency mode
  - [ ] 2%: Graceful shutdown

### Phase 6: Charging System
- [ ] **Smart Charging**:
  - [ ] Trickle charge when full
  - [ ] Temperature monitoring
  - [ ] Charge cycle counting
  - [ ] Battery health estimation
  
- [ ] **Charging Indicators**:
  - [ ] LED: Red = charging, Green = full
  - [ ] Serial status reports
  - [ ] Remote monitoring

### Phase 7: Emergency Power Mode
- [ ] **Critical Battery Response**:
  ```cpp
  void emergencyPowerMode() {
      // Keep only door unlock functional
      audio.disable();
      ledController.disable();
      
      // Unlock fail-safe option
      if (batteryPercent < 2 && FAIL_SAFE_ENABLED) {
          relays.setRelay(DOOR_RELAY, true);
          saveStateToEEPROM();
          enterDeepSleep();
      }
  }
  ```
- [ ] Preserve access logs
- [ ] Maintain card database
- [ ] Emergency unlock option

## Hardware Design

### Battery Protection Circuit
```
Battery + ----+---- Protection IC ---- Charge +
              |
              +---- Current Sense
              |
Battery - ----+---- Protection IC ---- Charge -
```

### Complete Power System
```
Main Power -> Fuse -> Rectifier -> Regulator -> Diode -+-> System
                          |                              |
                          +-> Charger -> Battery -> Diode +
```

## Testing Checklist

- [ ] Power failure simulation
- [ ] Battery runtime testing
- [ ] Charge cycle testing
- [ ] Switchover timing (<1ms)
- [ ] Low battery warnings
- [ ] Deep discharge recovery
- [ ] Temperature testing
- [ ] Long-term reliability

## Safety Considerations

- [ ] Use protected batteries
- [ ] Implement thermal monitoring
- [ ] Fuse all power paths
- [ ] Ventilation for charging
- [ ] Fire-resistant enclosure
- [ ] Regular battery replacement schedule

## Maintenance Schedule

- **Daily**: Automatic self-test
- **Weekly**: Battery voltage check
- **Monthly**: Full discharge/charge cycle
- **Quarterly**: Clean contacts
- **Annually**: Replace battery (or as needed)

## Cost Analysis

### Basic System (8-hour backup)
- 18650 battery (2x): $10
- TP4056 charger: $2
- Protection circuit: $3
- Diodes and components: $5
- **Total: ~$20**

### Advanced System (48-hour backup)
- LiPo pack (10Ah): $40
- Smart charger: $15
- Power management IC: $10
- Monitoring circuit: $10
- **Total: ~$75**

## Future Enhancements

- Solar panel charging option
- Supercapacitor for instant backup
- Wireless charging capability
- Battery hot-swap support
- Predictive battery failure alerts
- Integration with building UPS