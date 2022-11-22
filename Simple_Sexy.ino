// PLEASE NOTE THIS CODE IS FOR A 3.3V 8MHZ PRO MICRO, THIS WILL NOT WORK ON THE 5V VERSION,
// PLEASE INSURE YOU HAVE SELECTED THE CORRECT BOARD IN BOARD MANAGER. FAILURE TO DO THIS WILL BRICK THE PRO MICRO.
// PLEASE NOTE PRO MICROS AND PRO MINIS ARE 2 COMPLETELY DIFFERENT BOARDS.

//#define  USINGMP3  // download the Library for the Dfplayer here - https://github.com/PowerBroker2/DFPlayerMini_Fast/archive/refs/heads/master.zip
#include <SPI.h>
#include <PN532_SPI.h>
#include "PN532.h"     // download the Library for the Pn532 here - https://github.com/adafruit/Adafruit-PN532/archive/refs/heads/master.zip

PN532_SPI pn532spi(SPI, 10);
PN532 nfc(pn532spi);

#define RELAY1 9 // this code is set up for an active low relay
#define RELAY2 6
#define RELAY3 5
#define RELAY4 4

#define NUMBER_4BYTE_UIDS 1// QUANTITY OF 4B UIDS
#define NUMBER_7BYTE_UIDS 2//// QUANTITY OF 7B UIDS


uint8_t uids4B[NUMBER_4BYTE_UIDS][4]  = {
  {0x00, 0x00, 0x00, 0x00,} //4b uid 1 //  PUT YOUR 4BYTE UID HERE , JUST REPLACE EACH 00 WITH THE BIT, EG 0XB4,
};

uint8_t uids7B[NUMBER_7BYTE_UIDS][7] = {
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 7b uid 1 //  PUT YOUR 7BYTE UID HERE , JUST REPLACE EACH 00 WITH THE BIT, EG 0XB4,
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 7b uid 2//  PUT YOUR 7BYTE UID HERE , JUST REPLACE EACH 00 WITH THE BIT, EG 0XB4,
};
#ifdef USINGMP3
#include <DFPlayerMini_Fast.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 7); // RX, TX
DFPlayerMini_Fast myMP3;
#endif

#define MAXIMUM_INVALID_ATTEMPTS 13
uint8_t invalidAttempts = 0;
const uint8_t invalidDelays[MAXIMUM_INVALID_ATTEMPTS] = {1, 3, 4, 5, 8, 12, 17, 23, 30, 38, 47, 57, 68};

bool scanned = false;
bool impatient = false;

void setup(void) {
  Serial.begin(115200);
  pinMode(RELAY1, OUTPUT);
  digitalWrite(RELAY1, HIGH); // change all this to LOW if your relays trigger on power up
  pinMode(RELAY2, OUTPUT);
  digitalWrite(RELAY2, HIGH); // change all this to LOW if your relays trigger on power up
  pinMode(RELAY3, OUTPUT);
  digitalWrite(RELAY3, HIGH); // change all this to LOW if your relays trigger on power up
  pinMode(RELAY4, OUTPUT);
  digitalWrite(RELAY4, HIGH); // change all this to LOW if your relays trigger on power up

  #ifdef USINGMP3
  mySerial.begin(9600);
    myMP3.begin(mySerial, false);
#endif
 
  nfc.begin();
  

  delay(1000);
  Serial.println(("Starting up!"));
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print(F("Didn't find PN53x board"));
    while (1); // halt
  }
  
  Serial.print(F("Found chip PN5")); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print(F("Firmware ver. ")); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  nfc.SAMConfig();
    
  Serial.println(F("Waiting for an ISO14443A card"));
 
#ifdef USINGMP3
  myMP3.volume(10);// INCREASE VOLUME ON YOUR AMP. NOT HERE
  delay(500);
  myMP3.play(1); // PLAY POWER UP MP3 0001
  #endif
}


void loop(void) {
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  
  if (millis() > 10000 && !impatient && !scanned) {

    digitalWrite(RELAY1, HIGH);

#ifdef USINGMP3
    myMP3.play(2); // THIS MP3 PLAYS 10 SECONDS AFTER POWER UP IF NOTHING IS SCAN ''ARE YOU STILL THERE''
#endif
    impatient = true;
  }


  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength); 
  // Wait for an ISO14443A type card
  
  if (success) {
    scanned = true;
    Serial.println(F("Found a card!"));
    Serial.print(F("UID Length: "));Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print(F("UID Value: "));
    for (uint8_t i=0; i < uidLength; i++) 
    {Serial.print(" 0x");Serial.print(uid[i], HEX);}
    Serial.println("");
    
    bool validUID = false;
    if (uidLength == 4) {
      Serial.println("4B UID");
      for (int i = 0; i < NUMBER_4BYTE_UIDS; i++) {
        bool same = true;
        for (int j = 0; j < 4; j++) {
          if (uids4B[i][j] != uid[j]) {
            same = false;
          }
        }
        if (same) {
          validUID = true;
        }
      }
    } else if (uidLength == 7) {
      Serial.println("7B UID");
      for (int i = 0; i < NUMBER_7BYTE_UIDS; i++) {
        bool same = true;
        for (int j = 0; j < 7; j++) {
          if (uids7B[i][j] != uid[j]) {
            same = false;
          }
        }
        if (same) {
          validUID = true;
        }
      }
    } else {
      Serial.print(F("Unknown UID type / length"));
    
    }
    
    if (validUID) { // IF CORRECT UID DO THIS.
      Serial.print(F("Card match found!"));
                                                       // ------------- if correct uid do this
    }

      
#ifdef USINGMP3
      myMP3.play(3); // PLAYS THE ACEPTED MP3
#endif 
      invalidAttempts = 0; // RESETS THE DOUBLE DOWN DELAY
    } else {
      Serial.print(F("Unauthorised card"));
                                                       // ---------------if incorrect uid do this
      
      if (invalidAttempts == 0) {  // if wrong uid do this!

       //put your reation here


       #ifdef USINGMP3 
       myMP3.play(4); // PLAYS WRONGMP3 1 
       #endif  
      } else if (invalidAttempts == 1) {
        
        #ifdef USINGMP3
        myMP3.play(5); // 2 WRONG SCANS IN A ROW PLAYS WRONGMP3 2 
        #endif
      } else {
        
        #ifdef USINGMP3
       myMP3.play(6);  // 3 OR MORE WRONG SCANS IN A ROW PLAYS WRONGMP3 3 
       #endif 
      }
      delay(3000);
      delay(invalidDelays[invalidAttempts] * 1000);
      if (invalidAttempts < MAXIMUM_INVALID_ATTEMPTS-1) // THIS DOUBLES THE DELAYTIME BETWEEN EACH WRONG UID SCAN,(BRUTEFORCE DETERRANT) 
      {
        invalidAttempts++;
      }
    }
  }
