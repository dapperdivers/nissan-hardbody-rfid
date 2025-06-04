#pragma once

#ifdef UNIT_TEST
    #include "mock_arduino.h"
#else
    #include <Arduino.h>
#endif

class RelayController {
public:
    RelayController(uint8_t relay1_pin = 9, uint8_t relay2_pin = 6,
                    uint8_t relay3_pin = 5, uint8_t relay4_pin = 4);
    void begin();
    void setRelay(uint8_t relay, bool state);
    void setAllRelays(bool state);
    bool getRelayState(uint8_t relay) const;

private:
    struct RelayPin {
        uint8_t pin;
        bool state;
    };
    
    static constexpr uint8_t NUM_RELAYS = 4;
    RelayPin relays[NUM_RELAYS];
    
    bool isValidRelay(uint8_t relay) const;
};