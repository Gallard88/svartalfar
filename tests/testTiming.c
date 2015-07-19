#include "unity_fixture.h"
#include "../Timing.h"

TEST_GROUP(Timing);

TEST_SETUP(Timing)
{
  Timing_LoadValue(0);
}

TEST_TEAR_DOWN(Timing)
{
  Timing_LoadValue(0);
}

/* ======================== */
/* ======================== */
/*
  uint32_t Timing_ReadStamp(void);
 */
TEST(Timing, Timing_Init_Timing_ReadStamp)
{
	TEST_ASSERT_EQUAL(0, Timing_ReadStamp());
}

/* ======================== */
/*
    void Timing_Increment(void);
 */
TEST(Timing, Timing_Timing_Increment_1)
{
  Timing_Increment();
	TEST_ASSERT_EQUAL(1, Timing_ReadStamp());
}

/* ======================== */
/*
    uint8_t Timing_CheckDelay(uint32_t stamp, uint32_t delay);
 */
TEST(Timing, Timing_Timing_CheckDelay)
{
  Timing_Increment();
	TEST_ASSERT_EQUAL(0, Timing_CheckDelay(0,0));
}

TEST(Timing, Timing_Timing_CheckDelay_TimeBeforeStamp)
{
  Timing_LoadValue(5);
	TEST_ASSERT_EQUAL(0, Timing_CheckDelay(10,10));
}

TEST(Timing, Timing_Timing_CheckDelay_TimeAfterStampBeforeDelay)
{
  Timing_LoadValue(11);
	TEST_ASSERT_EQUAL(0, Timing_CheckDelay(10,10));
}

TEST(Timing, Timing_Timing_CheckDelay_TimeAfterStampAfterDelay)
{
  Timing_LoadValue(20);
	TEST_ASSERT_EQUAL(1, Timing_CheckDelay(10,10));
}


/* ======================== */
/* ======================== */
TEST_GROUP_RUNNER(Timing)
{
	RUN_TEST_CASE(Timing, Timing_Init_Timing_ReadStamp);
	RUN_TEST_CASE(Timing, Timing_Timing_Increment_1);
  RUN_TEST_CASE(Timing, Timing_Timing_CheckDelay_TimeBeforeStamp);
  RUN_TEST_CASE(Timing, Timing_Timing_CheckDelay_TimeAfterStampBeforeDelay);
  RUN_TEST_CASE(Timing, Timing_Timing_CheckDelay_TimeAfterStampAfterDelay);
}

