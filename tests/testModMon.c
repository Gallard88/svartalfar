
#include <stdio.h>

#include "unity_fixture.h"

#include "../ModuleMonitor.h"
#include "../Timing.h"

TEST_GROUP(ModMon);

uint8_t  callback_triggered;

TEST_SETUP(ModMon)
{
  Timing_LoadValue(0);
  callback_triggered = 0;
}

TEST_TEAR_DOWN(ModMon)
{
}

static void Callback(void)
{
  callback_triggered = 1;
}


TEST(ModMon, Run_NULL)
{
  uint8_t rv = ModuleMonitor_Run(NULL, 0, Callback);
  TEST_ASSERT_EQUAL(0, rv);
}

TEST(ModMon, Run_Timer_NotExp_Rv)
{
  uint32_t timer = 0;
  uint8_t rv = ModuleMonitor_Run(&timer, 10, Callback);
  TEST_ASSERT_EQUAL(0, rv);
}

TEST(ModMon, Run_Timer_NotExp_Func)
{
  uint32_t timer = 0;
  ModuleMonitor_Run(&timer, 10, Callback);
  TEST_ASSERT_EQUAL(0, callback_triggered);
}

TEST(ModMon, Run_Timer_Exp_Rv)
{
  uint32_t timer = 0;
  Timing_LoadValue(TIMER_SECONDS(11));
  uint8_t rv = ModuleMonitor_Run(&timer, 10, Callback);
  TEST_ASSERT_EQUAL(1, rv);
}

TEST(ModMon, Run_Timer_Exp_Func)
{
  uint32_t timer = 0;
  Timing_LoadValue(TIMER_SECONDS(11));
  ModuleMonitor_Run(&timer, 10, Callback);
  TEST_ASSERT_EQUAL(1, callback_triggered);
}

TEST(ModMon, Run_Timer_Exp_TimerValue)
{
  uint32_t timer = 0;
  Timing_LoadValue(TIMER_SECONDS(11));
  ModuleMonitor_Run(&timer, 10, Callback);
  TEST_ASSERT_EQUAL(TIMER_SECONDS(11), timer);
}

TEST(ModMon, Clear_NULL)
{
  ModuleMonitor_Clear(NULL);
}

TEST(ModMon, Clear_Timer)
{
  uint32_t timer = 0;

  Timing_LoadValue(10);
  ModuleMonitor_Clear(&timer);
  TEST_ASSERT_EQUAL(10, timer);
}

/* ======================== */
/* ======================== */
TEST_GROUP_RUNNER(ModMon)
{
  RUN_TEST_CASE(ModMon, Clear_NULL);
  RUN_TEST_CASE(ModMon, Clear_Timer);

  RUN_TEST_CASE(ModMon, Run_NULL);
  RUN_TEST_CASE(ModMon, Run_Timer_NotExp_Rv);
  RUN_TEST_CASE(ModMon, Run_Timer_NotExp_Func);
  RUN_TEST_CASE(ModMon, Run_Timer_Exp_Rv);
  RUN_TEST_CASE(ModMon, Run_Timer_Exp_Func);
  RUN_TEST_CASE(ModMon, Run_Timer_Exp_TimerValue);
}

