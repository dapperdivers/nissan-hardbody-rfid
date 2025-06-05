# Feature: DFPlayer SD Card Persistence

**Complexity**: 🟡 Medium  
**Hardware Required**: ✅ None (uses existing DFPlayer Mini SD card)  
**User Value**: ⭐⭐⭐ Essential

## Overview

Leverage the SD card in the DFPlayer Mini module for data persistence, storing authorized UIDs, access logs, and configuration settings. This provides virtually unlimited storage without additional hardware, using the existing audio module's SD card creatively.

## Benefits

- No additional hardware needed
- Massive storage capacity (GB vs KB)
- Easy backup/restore via SD card swap
- Survives firmware updates
- Portable configuration between devices
- No EEPROM wear limitations

## Implementation Approaches

Since the DFPlayer Mini doesn't provide direct SD card access, we need creative solutions:

### Approach 1: Audio File Metadata
- Encode data in MP3 file metadata (ID3 tags)
- Use specific track numbers for data storage
- Leverage file naming conventions

### Approach 2: Dual SD Card Reader
- Add a separate SD card module
- Share the same SD card (with careful timing)
- Direct file system access

### Approach 3: Serial Command Extensions
- Use undocumented DFPlayer commands
- Store data in unused sectors
- Risk of compatibility issues

## Implementation Checklist

### Phase 1: Data Structure Design
- [ ] Define storage format:
  ```cpp
  // File structure on SD card
  // /SYSTEM/CONFIG.DAT - Configuration
  // /SYSTEM/UIDS.DAT - Authorized cards
  // /SYSTEM/LOGS/YYYY-MM.LOG - Monthly logs
  // /MP3/0001.mp3 - Audio files remain unchanged
  
  struct PersistentConfig {
      uint32_t magic;          // 0xDEADBEEF for validation
      uint8_t version;         // Config version
      uint8_t volume;          // Audio volume
      uint16_t relayTime;      // Door unlock duration
      uint32_t sleepTimeout;   // Auto-sleep delay
      uint8_t accessMode;      // Security level
      uint8_t checksum;        // Data integrity
  };
  ```

### Phase 2: File-Based Storage System
- [ ] Create `SDPersistence` class:
  ```cpp
  class SDPersistence {
  private:
      bool sdAvailable;
      uint32_t lastWrite;
      
  public:
      bool begin();
      
      // UID Management
      bool saveUID(const uint8_t* uid, uint8_t length, uint8_t accessLevel);
      bool removeUID(const uint8_t* uid, uint8_t length);
      bool loadAllUIDs(RFIDController* rfid);
      uint16_t getUIDCount();
      
      // Configuration
      bool saveConfig(const PersistentConfig& config);
      bool loadConfig(PersistentConfig& config);
      
      // Logging
      bool logAccess(const LogEntry& entry);
      bool readLogs(uint32_t startTime, uint32_t endTime);
      
      // Maintenance
      bool backup(const char* filename);
      bool restore(const char* filename);
      bool format();
  };
  ```

### Phase 3: Creative DFPlayer Integration
- [ ] **Method 1: Track Number Encoding**:
  ```cpp
  // Use tracks 900-999 for data storage
  // Track 900: Config marker (specific duration = valid config)
  // Track 901-950: UID storage (duration encodes UID)
  // Track 951-999: Reserved for future use
  
  void encodeUIDAsTrack(uint8_t* uid, uint8_t length) {
      // Create silent MP3 with specific duration
      // Duration in ms encodes the UID data
      uint32_t duration = 0;
      for (int i = 0; i < length; i++) {
          duration = (duration << 8) | uid[i];
      }
      // Generate MP3 with exact duration
  }
  ```

- [ ] **Method 2: Filename Encoding**:
  ```cpp
  // Use hex-encoded filenames in special folder
  // /DATA/04123456789ABC.mp3 - 7-byte UID
  // /DATA/B4123456.mp3 - 4-byte UID
  // File existence = authorized
  // File size = access level
  ```

### Phase 4: SD Card Access Methods
- [ ] **Parallel SD Module** (Recommended):
  ```cpp
  // Add SD card module on separate SPI
  // Coordinate access with DFPlayer
  
  void accessSDCard() {
      // 1. Pause DFPlayer
      audio.pause();
      delay(50);
      
      // 2. Access SD card directly
      SD.begin(SD_CS_PIN);
      File dataFile = SD.open("/SYSTEM/UIDS.DAT");
      // ... read/write operations
      dataFile.close();
      SD.end();
      
      // 3. Resume DFPlayer
      delay(50);
      audio.resume();
  }
  ```

### Phase 5: Data Management Features
- [ ] **Automatic Backup**:
  - [ ] Daily config backup
  - [ ] Rolling log files
  - [ ] Corruption detection
  - [ ] Automatic recovery
  
- [ ] **Import/Export**:
  - [ ] CSV export for logs
  - [ ] JSON config format
  - [ ] Batch UID import
  - [ ] QR code generation

### Phase 6: File System Layout
```
SD Card Root/
├── MP3/
│   ├── 0001.mp3  (Power up sound)
│   ├── 0002.mp3  (Waiting sound)
│   ├── 0003.mp3  (Access granted)
│   ├── 0004.mp3  (Access denied 1)
│   ├── 0005.mp3  (Access denied 2)
│   └── 0006.mp3  (Access denied 3)
├── SYSTEM/
│   ├── CONFIG.DAT
│   ├── UIDS.DAT
│   ├── MASTER.DAT
│   └── BACKUP/
│       └── CONFIG_20240604.BAK
└── LOGS/
    ├── 2024-01.LOG
    ├── 2024-02.LOG
    └── 2024-03.LOG
```

### Phase 7: Implementation Strategies

- [ ] **Option A: Minimal Hardware Change**:
  ```cpp
  // Use only DFPlayer, encode in filenames
  // Pros: No hardware changes
  // Cons: Limited data capacity, slow
  
  bool uidExists(uint8_t* uid, uint8_t length) {
      char filename[32];
      sprintf(filename, "/DATA/%02X%02X%02X%02X.mp3", 
              uid[0], uid[1], uid[2], uid[3]);
      // Check if DFPlayer can play this track
      return dfPlayerCanPlay(filename);
  }
  ```

- [ ] **Option B: Shared SD Access**:
  ```cpp
  // Add SD module, share card with DFPlayer
  // Pros: Full file system access
  // Cons: Timing coordination needed
  
  class SharedSDAccess {
      bool acquireSDAccess() {
          audio.sleep();  // Put DFPlayer in sleep
          return SD.begin(SD_CS_PIN);
      }
      
      void releaseSDAccess() {
          SD.end();
          audio.wake();  // Wake DFPlayer
      }
  };
  ```

## Testing Checklist

- [ ] SD card detection
- [ ] Read/write reliability
- [ ] DFPlayer coexistence
- [ ] Power loss during write
- [ ] File corruption recovery
- [ ] Performance impact
- [ ] Multi-GB card support
- [ ] FAT32 compatibility

## Configuration Example

```cpp
// Enable SD persistence
#define USE_SD_PERSISTENCE true
#define SD_CS_PIN 4           // If using separate SD module
#define MAX_LOG_FILES 12      // Keep 1 year of logs
#define BACKUP_INTERVAL 86400 // Daily backup (seconds)
#define DATA_FORMAT_VERSION 1 // Increment on structure change
```

## Migration Path

1. **From EEPROM to SD**:
   ```cpp
   void migrateFromEEPROM() {
       // Read all EEPROM data
       for (int i = 0; i < EEPROM_UID_COUNT; i++) {
           UID uid = readUIDFromEEPROM(i);
           sdPersistence.saveUID(uid);
       }
       // Mark EEPROM as migrated
   }
   ```

2. **Backward Compatibility**:
   - Check SD first, fall back to EEPROM
   - Sync changes to both
   - Gradual migration

## Advantages Over EEPROM

| Feature | EEPROM | SD Card |
|---------|---------|----------|
| Capacity | 1KB | 32GB+ |
| Write Cycles | 100,000 | Unlimited* |
| Portability | No | Yes |
| Backup | Difficult | Easy |
| External Access | No | Yes |

## Future Enhancements

- Encrypted storage for security
- Cloud sync via WiFi module
- Multi-site configuration sharing
- Automated report generation
- Integration with access control software
- Real-time replication to backup SD