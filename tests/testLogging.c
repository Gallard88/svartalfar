#include "unity_fixture.h"
#include "../Logging.h"
#include "../Timing.h"

#include <string.h>

TEST_GROUP(Logging);

char LogBuffer[1024];

TEST_SETUP(Logging)
{
  memset(LogBuffer, 0, sizeof(LogBuffer));
  Timing_LoadValue(0);
  Log_SetEnable(1);
}

TEST_TEAR_DOWN(Logging)
{
  Timing_LoadValue(0);
}

void Log_PrintMsg(const char *msg)
{
  strncpy(LogBuffer, msg, sizeof(LogBuffer));
}

/* ======================== */
/* ======================== */
/*
    void Logf(const char *module, const char *fmt, ... );
 */
TEST(Logging, Logging__LogF_NULL_Module_NULL_MSG)
{
  Logf(NULL, NULL);
	TEST_ASSERT_NOT_EQUAL(0, strlen(LogBuffer));
}

TEST(Logging, Logging__LogF_Module_NULL_MSG)
{
  Logf("Test", NULL);
	TEST_ASSERT_NOT_EQUAL(0, strlen(LogBuffer));
}

TEST(Logging, Logging__LogF_NULL_Module_MSG)
{
  Logf(NULL, "Msg");
	TEST_ASSERT_NOT_EQUAL(0, strlen(LogBuffer));
}

TEST(Logging, Logging__LogF_Module_MSG)
{
  Logf("Test", "Module");
	TEST_ASSERT_NOT_EQUAL(0, strlen(LogBuffer));
}

/* ======================== */
/*
    void    Log_SetEnable(uint8_t en);
    uint8_t Log_GetEnable(void);
 */
TEST(Logging, Logging__Report_Disabled)
{
  Log_SetEnable(0);
  TEST_ASSERT_EQUAL(0, Log_GetEnable());
}

TEST(Logging, Logging__Report_Enabled)
{
  Log_SetEnable(1);
	TEST_ASSERT_NOT_EQUAL(0, Log_GetEnable());
}

TEST(Logging, Logging__LogF_Disabled)
{
  Log_SetEnable(0);
  Logf("Test", "Module");
	TEST_ASSERT_EQUAL(0, strlen(LogBuffer));
}

TEST(Logging, Logging__LogF_Enabled)
{
  Log_SetEnable(1);
  Logf("Test", "Module");
	TEST_ASSERT_NOT_EQUAL(0, strlen(LogBuffer));
}

/* ======================== */
/* ======================== */
TEST_GROUP_RUNNER(Logging)
{
	RUN_TEST_CASE(Logging, Logging__LogF_NULL_Module_NULL_MSG);
	RUN_TEST_CASE(Logging, Logging__LogF_Module_NULL_MSG);
	RUN_TEST_CASE(Logging, Logging__LogF_NULL_Module_MSG);
	RUN_TEST_CASE(Logging, Logging__LogF_Module_MSG);

	RUN_TEST_CASE(Logging, Logging__Report_Disabled);
	RUN_TEST_CASE(Logging, Logging__Report_Enabled);
  
	RUN_TEST_CASE(Logging, Logging__LogF_Disabled); 
	RUN_TEST_CASE(Logging, Logging__LogF_Enabled);
}

