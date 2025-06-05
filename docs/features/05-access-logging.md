# Feature: Access Logging

**Complexity**: 🟡 Medium  
**Hardware Required**: 🔧 Minor (SD card module or none for EEPROM)  
**User Value**: ⭐⭐⭐ Essential

## Overview

Implement comprehensive logging of all access attempts, both successful and failed. Store logs locally on SD card, EEPROM, or transmit to remote server. Essential for security auditing and troubleshooting.

## Benefits

- Security audit trail
- Usage pattern analysis
- Troubleshooting system issues
- Compliance with security policies
- Evidence in case of security incidents

## Storage Options

### EEPROM (Built-in)
- No additional hardware
- Limited space (~1KB on Pro Micro)
- Circular buffer for recent events
- Best for last 50-100 events

### SD Card Module
- Massive storage capacity
- Removable for analysis
- Requires SPI connection
- ~$5 for module + SD card

### External EEPROM (I2C)
- 24LC256 provides 32KB storage
- Non-volatile storage
- ~$2 per chip
- Can chain multiple chips

## Implementation Checklist

### Phase 1: Log Entry Structure
- [ ] Define log entry format:
  ```cpp
  struct LogEntry {
      uint32_t timestamp;      // Unix timestamp
      uint8_t eventType;       // Access granted/denied/error
      uint8_t uid[7];         // Card UID
      uint8_t uidLength;      // 4 or 7 bytes
      uint8_t accessLevel;    // If using access levels
      uint8_t relayActivated; // Which relay was triggered
      uint16_t checksum;      // Data integrity check
  };
  ```
- [ ] Event type constants:
  ```cpp
  enum EventType {
      EVENT_ACCESS_GRANTED = 0x01,
      EVENT_ACCESS_DENIED = 0x02,
      EVENT_INVALID_CARD = 0x03,
      EVENT_EXPIRED_CARD = 0x04,
      EVENT_TIME_RESTRICTED = 0x05,
      EVENT_EMERGENCY_UNLOCK = 0x06,
      EVENT_REMOTE_UNLOCK = 0x07,
      EVENT_SYSTEM_STARTUP = 0x08,
      EVENT_SYSTEM_ERROR = 0x09,
      EVENT_TAMPER_ALERT = 0x0A
  };
  ```

### Phase 2: Storage Implementation
- [ ] Create `LogController` class:
  ```cpp
  class LogController {
  public:
      virtual bool begin() = 0;
      virtual bool logEvent(LogEntry entry) = 0;
      virtual uint32_t getLogCount() = 0;
      virtual bool readLog(uint32_t index, LogEntry& entry) = 0;
      virtual bool clearLogs() = 0;
      virtual uint32_t getOldestTimestamp() = 0;
  };
  ```
- [ ] EEPROM implementation:
  - [ ] Circular buffer management
  - [ ] Wear leveling algorithm
  - [ ] Header with write position
- [ ] SD card implementation:
  - [ ] File rotation (daily/weekly)
  - [ ] CSV format for easy import
  - [ ] Binary format for space efficiency

### Phase 3: Logging Integration
- [ ] Log points in main code:
  - [ ] System startup
  - [ ] Each card scan attempt
  - [ ] Access decisions
  - [ ] Relay activations
  - [ ] Error conditions
  - [ ] Configuration changes
- [ ] Add contextual information:
  - [ ] Door open duration
  - [ ] Multiple scan attempts
  - [ ] System voltage (if monitored)

### Phase 4: Log Management
- [ ] **Retrieval Methods**:
  - [ ] Serial command interface
  - [ ] Export to CSV
  - [ ] Filter by date range
  - [ ] Filter by card UID
  - [ ] Filter by event type
  
- [ ] **Storage Management**:
  - [ ] Auto-rotation by size/time
  - [ ] Compression for old logs
  - [ ] Backup reminders
  - [ ] Low storage warnings

### Phase 5: Enhanced Logging Features
- [ ] **Analytics**:
  - [ ] Peak usage times
  - [ ] Most frequent users
  - [ ] Failed attempt patterns
  - [ ] Average door open time
  - [ ] Unusual activity detection
  
- [ ] **Reporting**:
  - [ ] Daily summary
  - [ ] Weekly reports
  - [ ] Monthly statistics
  - [ ] Custom date ranges

### Phase 6: Serial Commands
- [ ] Implement commands:
  ```
  LOG_COUNT              - Get total log entries
  LOG_READ <index>       - Read specific entry
  LOG_TAIL <count>       - Read last N entries
  LOG_FILTER <type>      - Filter by event type
  LOG_EXPORT             - Export all logs
  LOG_CLEAR              - Clear all logs
  LOG_STATS              - Show statistics
  ```

### Phase 7: Log Format Examples

#### CSV Format
```csv
Timestamp,Date,Time,Event,UID,AccessLevel,Relay,Details
1704067200,2024-01-01,12:00:00,ACCESS_GRANTED,04:12:34:56:78:9A:BC,USER,1,Door opened
1704067230,2024-01-01,12:00:30,ACCESS_DENIED,01:23:45:67,NONE,0,Invalid card
```

#### Binary Format (Compact)
```
[Header: 8 bytes]
[Entry 1: 20 bytes]
[Entry 2: 20 bytes]
...
```

## Memory Calculations

### EEPROM (1KB)
- Header: 8 bytes
- Per entry: 20 bytes
- Capacity: ~50 entries

### SD Card (1GB)
- CSV format: ~80 bytes/entry
- Binary format: 20 bytes/entry
- Capacity: 50M+ entries

### External EEPROM (32KB)
- Header: 8 bytes
- Per entry: 20 bytes
- Capacity: ~1,600 entries

## Testing Checklist

- [ ] Log write performance
- [ ] Storage full handling
- [ ] Power loss during write
- [ ] Log retrieval speed
- [ ] Data integrity verification
- [ ] Time accuracy across reboots
- [ ] Export functionality
- [ ] Filter accuracy

## Privacy Considerations

- [ ] GDPR compliance options
- [ ] Automatic log deletion after X days
- [ ] Anonymization options
- [ ] Access control to logs
- [ ] Encryption of sensitive data

## Future Enhancements

- Remote log streaming
- Cloud backup integration
- Machine learning for anomaly detection
- Integration with SIEM systems
- Blockchain for tamper-proof logs