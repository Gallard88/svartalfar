#include "unity_fixture.h"
#include "../Syslog.h"

#include <string.h>

TEST_GROUP(Syslog);

char SyslogBuffer[1024];
LogLevel Level;

TEST_SETUP(Syslog)
{
  memset(SyslogBuffer, 0, sizeof(SyslogBuffer));
}

TEST_TEAR_DOWN(Syslog)
{
}

void Syslog_Print(LogLevel level, const char *msg)
{
  strncpy(SyslogBuffer, msg, sizeof(SyslogBuffer));
  Level = level;
}

/* ======================== */
/* ======================== */
/*
    void Syslog(LogLevel level, const char *module, const char *fmt, ... )
 */
TEST(Syslog, Syslog_Level_PassedCorrectly)
{
  Syslog(lEmerg, "Module", "Message");
	TEST_ASSERT_EQUAL(lEmerg, Level);
}

TEST(Syslog, Syslog_MessageGenerated)
{
  Syslog(lEmerg, "Module", "Message");
	TEST_ASSERT_NOT_EQUAL(0, strlen(SyslogBuffer));
}

/* ======================== */
/* ======================== */
TEST_GROUP_RUNNER(Syslog)
{
	RUN_TEST_CASE(Syslog, Syslog_Level_PassedCorrectly);
  RUN_TEST_CASE(Syslog, Syslog_MessageGenerated);
}

