#include "unity_fixture.h"

#include "DriverTemplate_SPI.h"
#include "DriverTemplate_USART.h"

static void RunAllTests(void)
{
  RUN_TEST_GROUP(Clock);
  RUN_TEST_GROUP(CmdProcessor);
  RUN_TEST_GROUP(Dataflash);
  RUN_TEST_GROUP(EEPROM);
  RUN_TEST_GROUP(ErrorCheck);
  RUN_TEST_GROUP(Logging);
  RUN_TEST_GROUP(Timing);
  RUN_TEST_GROUP(Syslog);
}

int main(int argc, char *argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}

