#include "unity_fixture.h"
#include "../EEPROM.h"

TEST_GROUP(EEPROM);

const DFToken_t EE_Token = {
 .start_page = 0,
 .number_pages = 100
};


TEST_SETUP(EEPROM)
{
}

TEST_TEAR_DOWN(EEPROM)
{
}

/* ======================== */
TEST(EEPROM, DataflashDriver)
{
//	TEST_ASSERT_EQUAL(0, SysClock_IsDateUpdated());
}

/* ======================== */
/* ======================== */
TEST_GROUP_RUNNER(EEPROM)
{
	RUN_TEST_CASE(EEPROM, DataflashDriver);
}