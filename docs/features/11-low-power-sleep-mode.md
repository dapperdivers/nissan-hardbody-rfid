# Feature: Low Power Sleep Mode

**Complexity**: 🟢 Low  
**Hardware Required**: 🔧 Minor (Capacitive button or switch)  
**User Value**: ⭐⭐⭐ Essential

## Overview

Implement ultra-low power sleep mode for the Arduino Pro Micro, reducing power consumption to microamps when idle. The system wakes instantly when a capacitive button is touched or when motion is detected, making it perfect for battery-powered installations.

## Benefits

- Dramatically extends battery life (weeks to months)
- Reduces power consumption by 99%+ when idle
- Instant wake-up response
- No compromise on functionality
- Perfect for solar/battery powered systems

## Power Consumption Analysis

### Active Mode
- Normal operation: ~80-200mA
- Daily consumption: ~2-5Ah

### Sleep Mode
- Deep sleep: ~0.1-0.5mA
- Daily consumption: ~0.002-0.012Ah
- **Power savings: 99.5%+**

## Implementation Checklist

### Phase 1: Basic Sleep Implementation
- [ ] Create `PowerSaveController` class:
  ```cpp
  class PowerSaveController {
  private:
      uint8_t wakePin;
      volatile bool shouldWake;
      unsigned long lastActivity;
      unsigned long sleepTimeout;
  public:
      void begin(uint8_t pin = 7);
      void checkAndSleep();
      void forceSleep();
      void wake();
      bool isAwake();
      void resetActivityTimer();
  };
  ```
- [ ] Implement sleep/wake logic based on Pro Micro pin 7
- [ ] Use external interrupt 4 (specific to ATmega32U4)

### Phase 2: Wake Trigger Hardware
- [ ] **Capacitive Touch Button**:
  - [ ] TTP223 touch sensor module (~$1)
  - [ ] Metallic surface as touch pad
  - [ ] Weather-resistant implementation
  - [ ] LED feedback on touch
  
- [ ] **PIR Motion Sensor**:
  - [ ] HC-SR501 PIR sensor (~$2)
  - [ ] Adjustable sensitivity
  - [ ] 3-7m detection range
  - [ ] Wake on approach

- [ ] **Simple Button**:
  - [ ] Momentary switch to GND
  - [ ] Internal pullup resistor
  - [ ] Debouncing in software

### Phase 3: Sleep Mode Implementation
- [ ] Core sleep code (based on pin 7 interrupt):
  ```cpp
  #include <avr/sleep.h>
  
  void PowerSaveController::wake() {
      sleep_disable();
      detachInterrupt(digitalPinToInterrupt(wakePin));
      shouldWake = true;
  }
  
  void PowerSaveController::forceSleep() {
      // Prepare for sleep
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);
      
      // Disable unnecessary peripherals
      disablePeripherals();
      
      // Attach wake interrupt
      noInterrupts();
      sleep_enable();
      attachInterrupt(digitalPinToInterrupt(wakePin), 
                     wake, LOW);
      interrupts();
      
      // Enter sleep
      sleep_cpu();
      
      // Wake up here
      enablePeripherals();
  }
  ```

### Phase 4: Peripheral Management
- [ ] **Before Sleep**:
  ```cpp
  void disablePeripherals() {
      // Turn off RFID reader
      digitalWrite(RFID_POWER_PIN, LOW);
      
      // Mute audio
      audio.setPower(false);
      
      // Turn off LEDs
      ledController.allOff();
      
      // Disable unused timers
      power_timer1_disable();
      power_timer2_disable();
      
      // Disable ADC
      ADCSRA &= ~(1 << ADEN);
      
      // Disable SPI/I2C if not needed
      power_spi_disable();
      power_twi_disable();
  }
  ```
  
- [ ] **After Wake**:
  ```cpp
  void enablePeripherals() {
      // Re-enable ADC
      ADCSRA |= (1 << ADEN);
      
      // Re-enable timers
      power_timer1_enable();
      power_timer2_enable();
      
      // Power up RFID
      digitalWrite(RFID_POWER_PIN, HIGH);
      delay(50); // Stabilization
      
      // Re-initialize components
      rfid.begin();
      audio.setPower(true);
  }
  ```

### Phase 5: Activity-Based Sleep
- [ ] **Auto-Sleep Logic**:
  - [ ] Monitor last card scan time
  - [ ] Monitor last button press
  - [ ] Monitor last remote command
  - [ ] Sleep after configurable timeout (e.g., 30 seconds)
  
- [ ] **Smart Wake Conditions**:
  - [ ] Capacitive button touch
  - [ ] Motion detection
  - [ ] Scheduled wake times
  - [ ] Remote wake command

### Phase 6: Integration
- [ ] Modify main loop:
  ```cpp
  void loop() {
      // Check if we should sleep
      powerSave.checkAndSleep();
      
      // Normal operation when awake
      if (powerSave.isAwake()) {
          // Existing RFID checking code
          checkRFID();
          
          // Reset activity on any action
          if (cardDetected || buttonPressed) {
              powerSave.resetActivityTimer();
          }
      }
  }
  ```

### Phase 7: Power Optimization
- [ ] **RFID Power Control**:
  - [ ] Add MOSFET to control RFID power
  - [ ] Cut power completely during sleep
  - [ ] Quick power-up on wake
  
- [ ] **LED Brightness**:
  - [ ] Reduce brightness in battery mode
  - [ ] Disable non-essential LEDs
  - [ ] Use PWM for efficiency

## Hardware Connections

### Capacitive Touch Wake Button
```
Arduino Pin 7 ----+---- TTP223 OUT
                  |
GND --------------+---- TTP223 GND
3.3V -----------------  TTP223 VCC
```

### PIR Motion Sensor Wake
```
Arduino Pin 7 -------- HC-SR501 OUT
GND ----------------- HC-SR501 GND
3.3V ---------------- HC-SR501 VCC
```

### RFID Power Control
```
Arduino Pin 8 ---- Gate (MOSFET) ---- RFID VCC
                        |
                      Source -------- 3.3V
                        |
                      Drain --------- RFID Power
```

## Testing Checklist

- [ ] Measure sleep current (<1mA)
- [ ] Test wake responsiveness
- [ ] Verify all peripherals restart
- [ ] Test activity timeout
- [ ] Battery life testing
- [ ] Temperature stability
- [ ] EMI/RFI immunity
- [ ] Long-term reliability

## Configuration Options

```cpp
// Power save configuration
#define ENABLE_SLEEP_MODE true
#define SLEEP_TIMEOUT_MS 30000      // 30 seconds
#define WAKE_PIN 7                   // Pin 7 for Pro Micro
#define USE_MOTION_WAKE true
#define MOTION_PIN A0
#define MIN_BATTERY_SLEEP 10         // Sleep if battery < 10%
#define DEEP_SLEEP_HOUR_START 22     // 10 PM
#define DEEP_SLEEP_HOUR_END 6        // 6 AM
```

## Power Consumption Examples

### Without Sleep Mode (24/7 operation)
- 100mA average × 24h = 2.4Ah/day
- 2000mAh battery = 0.8 days runtime

### With Sleep Mode (10 uses/day, 1min each)
- Active: 100mA × 10min = 16.7mAh
- Sleep: 0.5mA × 1430min = 11.9mAh
- Total: 28.6mAh/day
- 2000mAh battery = **70 days runtime**

## Troubleshooting

- **Won't Wake**: Check pull-up resistor on pin 7
- **Resets on Wake**: Check power supply stability
- **RFID Not Working**: Add delay after power-up
- **High Sleep Current**: Check all peripherals are disabled

## Future Enhancements

- Solar charging integration
- Adaptive sleep based on usage patterns
- Network wake-on-LAN support
- Accelerometer wake on vibration
- Temperature-based sleep adjustments
- Energy harvesting from door movement