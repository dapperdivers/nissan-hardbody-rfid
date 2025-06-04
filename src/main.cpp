#ifndef UNIT_TEST

#include <Arduino.h>
#include "rfid_controller.h"
#include "relay_controller.h"
#include "audio_player.h"

// Instantiate controllers
RFIDController rfid;
RelayController relays;
AudioPlayer audio;

// Constants
constexpr uint8_t MAXIMUM_INVALID_ATTEMPTS = 13;
const uint8_t invalidDelays[MAXIMUM_INVALID_ATTEMPTS] = {1, 3, 4, 5, 8, 12, 17, 23, 30, 38, 47, 57, 68};
constexpr uint32_t DOOR_UNLOCK_DURATION = 10000;  // 10 seconds
constexpr uint8_t DOOR_UNLOCK_RELAY = 0;  // Relay 1 (0-indexed)

// State variables
uint8_t invalidAttempts = 0;
bool scanned = false;
bool impatient = false;
unsigned long relayActivatedTime = 0;
bool relayActive = false;

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println(F("Starting up!"));
    
    // Initialize controllers
    if (!rfid.begin()) {
        Serial.print(F("Didn't find PN53x board"));
        while (1); // halt
    }
    
    // Print firmware version like the original
    rfid.printFirmwareVersion();
    
    // Initialize with default UIDs
    rfid.initializeDefaultUIDs();
    
    relays.begin();
    relays.setAllRelays(false); // All relays off initially
    
    if (audio.begin()) {
        audio.setVolume(10);
        delay(500);
        audio.playTrack(AudioPlayer::SOUND_STARTUP);
    }
    
    Serial.println(F("Waiting for an ISO14443A card"));
}

void loop() {

    boolean success;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    
    // Check if relay should be deactivated after 10 seconds
    if (relayActive && (millis() - relayActivatedTime >= DOOR_UNLOCK_DURATION)) {
        relays.setRelay(DOOR_UNLOCK_RELAY, false);
        relayActive = false;
        Serial.println(F("Door locked"));
    }
    
    // Check if we should play the waiting sound
    if (millis() > 10000 && !impatient && !scanned) {
        audio.playTrack(AudioPlayer::SOUND_WAITING);
        impatient = true;
    }
    
    // Try to read a card
    success = rfid.readCard(uid, &uidLength);
    
    if (success) {
        scanned = true;
        Serial.println(F("Found a card!"));
        Serial.print(F("UID Length: "));Serial.print(uidLength, DEC);Serial.println(" bytes");
        Serial.print(F("UID Value: "));
        for (uint8_t i=0; i < uidLength; i++)
        {Serial.print(" 0x");Serial.print(uid[i], HEX);}
        Serial.println("");
        
        bool validUID = rfid.validateUID(uid, uidLength);
        
        if (uidLength == 4) {
            Serial.println("4B UID");
        } else if (uidLength == 7) {
            Serial.println("7B UID");
        } else {
            Serial.print(F("Unknown UID type / length"));
        }
        
        if (validUID) { // IF CORRECT UID DO THIS.
            Serial.print(F("Card match found!"));
            invalidAttempts = 0; // RESETS THE DOUBLE DOWN DELAY
            audio.playTrack(AudioPlayer::SOUND_ACCEPTED);
            
            // Activate relay 1 for 10 seconds to unlock door
            relays.setRelay(DOOR_UNLOCK_RELAY, true);
            relayActivatedTime = millis();
            relayActive = true;
            Serial.println(F("Door unlocked for 10 seconds"));
            
        } else {
            Serial.print(F("Unauthorised card"));
            if (invalidAttempts == 0) {
                audio.playTrack(AudioPlayer::SOUND_DENIED_1);
            } else if (invalidAttempts == 1) {
                audio.playTrack(AudioPlayer::SOUND_DENIED_2);
            } else {
                audio.playTrack(AudioPlayer::SOUND_DENIED_3);
            }
            delay(3000);
            delay(invalidDelays[invalidAttempts] * 1000);
            if (invalidAttempts < MAXIMUM_INVALID_ATTEMPTS-1) // THIS DOUBLES THE DELAYTIME BETWEEN EACH WRONG UID SCAN,(BRUTEFORCE DETERRANT)
            {
                invalidAttempts++;
            }
        }
    }
}

#endif