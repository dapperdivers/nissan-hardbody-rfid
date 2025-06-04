#include <unity.h>
#include "test_helpers.h"

// Declare test functions
void test_rfid_initialization(void);
void test_rfid_default_uids(void);
void test_rfid_add_uids(void);
void test_rfid_invalid_uid_length(void);
void test_rfid_max_uid_limit(void);

// setUp and tearDown are defined in test_helpers.cpp

int main(int argc, char **argv) {
    UNITY_BEGIN();
    
    RUN_TEST(test_rfid_initialization);
    RUN_TEST(test_rfid_default_uids);
    RUN_TEST(test_rfid_add_uids);
    RUN_TEST(test_rfid_invalid_uid_length);
    RUN_TEST(test_rfid_max_uid_limit);
    
    return UNITY_END();
}