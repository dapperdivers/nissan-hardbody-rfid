# Feature: LED Status Indicators

**Complexity**: 🟢 Low  
**Hardware Required**: ✅ None (uses onboard LEDs)  
**User Value**: ⭐⭐⭐ Essential

## Overview

Use the Arduino Pro Micro's onboard LEDs (RX/TX and built-in LED on pin 17) to provide visual feedback about system status, card reads, and access decisions.

## Benefits

- Immediate visual feedback without audio
- Helps with debugging and system monitoring
- Low power consumption
- No additional hardware required
- Works in noisy environments where audio might be missed

## Implementation Checklist

### Phase 1: Basic LED Controller
- [ ] Create `LEDController` class in `include/led_controller.h`
- [ ] Implement LED control methods:
  - [ ] `void begin()` - Initialize LED pins
  - [ ] `void setSystemReady()` - Solid LED to show system ready
  - [ ] `void setScanning()` - Blink pattern during card read
  - [ ] `void setAccessGranted()` - Fast blink or solid green pattern
  - [ ] `void setAccessDenied()` - Rapid flash pattern
  - [ ] `void setError()` - Distinct error pattern

### Phase 2: LED Patterns
- [ ] Define LED pattern constants:
  ```cpp
  enum LEDPattern {
    PATTERN_OFF,
    PATTERN_SOLID,
    PATTERN_SLOW_BLINK,
    PATTERN_FAST_BLINK,
    PATTERN_DOUBLE_BLINK,
    PATTERN_TRIPLE_BLINK,
    PATTERN_SOS
  };
  ```
- [ ] Implement non-blocking pattern generator using millis()
- [ ] Add pattern queue for sequential patterns

### Phase 3: Status Indicators
- [ ] **System States**:
  - [ ] Power on: Slow fade in/out
  - [ ] Ready: Slow breathing pattern
  - [ ] Waiting (after 10s): Double blink every 2 seconds
  - [ ] Card detected: Rapid blink
  
- [ ] **Access States**:
  - [ ] Valid card: 3 fast blinks then solid for 2 seconds
  - [ ] Invalid card: 5 very rapid flashes
  - [ ] Brute force lockout: SOS pattern
  - [ ] Door unlocked: Solid LED while relay active

- [ ] **Error States**:
  - [ ] RFID reader error: Continuous rapid blink
  - [ ] Audio player error: Long-short-long pattern

### Phase 4: Integration
- [ ] Add `LEDController` instance to `main.cpp`
- [ ] Call LED methods at appropriate points:
  - [ ] `setup()`: Show initialization pattern
  - [ ] Card read attempts
  - [ ] Access granted/denied events
  - [ ] Error conditions
- [ ] Ensure LED patterns don't block main loop

### Phase 5: Advanced Features (Optional)
- [ ] Use PWM for smooth fading effects
- [ ] RGB LED support (if external RGB LED added)
- [ ] Configurable brightness levels
- [ ] Custom pattern builder via serial commands

## Example Code Structure

```cpp
class LEDController {
public:
    LEDController(uint8_t led_pin = LED_BUILTIN);
    void begin();
    void update(); // Call in loop() for non-blocking patterns
    void setPattern(LEDPattern pattern);
    void playSequence(LEDPattern* patterns, uint8_t count);
    
private:
    uint8_t pin;
    LEDPattern current_pattern;
    unsigned long last_update;
    uint8_t pattern_state;
};
```

## Testing Checklist

- [ ] Test all LED patterns individually
- [ ] Verify patterns are non-blocking
- [ ] Test pattern transitions
- [ ] Verify correct patterns for each system state
- [ ] Test LED visibility in different lighting conditions
- [ ] Measure current consumption with LEDs active
- [ ] Test error condition indicators

## Future Enhancements

- External LED strip for more detailed status
- Different colors for different access levels
- LED matrix display for card UID display
- Light sensor for automatic brightness adjustment