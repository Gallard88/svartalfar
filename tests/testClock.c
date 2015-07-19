#include "unity_fixture.h"
#include "../Clock.h"

TEST_GROUP(Clock);

TEST_SETUP(Clock)
{
  ClockTime_t time;
  ClockDate_t date;
  
	time.seconds = 0;
	time.minutes = 0;
	time.hours = 0;

	date.day = 1;
	date.month = 1;
	date.year = 2015;
  
  SystemClock_Init(time, date);
}

TEST_TEAR_DOWN(Clock)
{
}

/* ======================== */
/* ======================== */
TEST(Clock, Clock_SysClock_Init_TimeSetFlag_NotSet)
{
	TEST_ASSERT_EQUAL(0, SysClock_IsTimeSet());
}

TEST(Clock, Clock_SysClock_Init_DateSetFlag_NotSet)
{
	TEST_ASSERT_EQUAL(0, SysClock_IsDateSet());
}

/* ======================== */
/* ======================== */
/*
    void SysClock_Run(uint16_t milliseconds);
 */
TEST(Clock, Clock_SysClock_Run_0)
{
  SystemClock clk;
  
  SysClock_Run(0);
  clk = SysClock_Get();
	TEST_ASSERT_EQUAL(0, clk.milliseconds);
	TEST_ASSERT_EQUAL(0, clk.Time.seconds);
	TEST_ASSERT_EQUAL(0, clk.Time.minutes);
	TEST_ASSERT_EQUAL(0, clk.Time.hours);
	TEST_ASSERT_EQUAL(1, clk.Date.day);
	TEST_ASSERT_EQUAL(1, clk.Date.month);
	TEST_ASSERT_EQUAL(2015, clk.Date.year);
}

TEST(Clock, Clock_SysClock_Run_1)
{
  SystemClock clk;
  
  SysClock_Run(1);
  clk = SysClock_Get();
	TEST_ASSERT_EQUAL(1, clk.milliseconds);
	TEST_ASSERT_EQUAL(0, clk.Time.seconds);
	TEST_ASSERT_EQUAL(0, clk.Time.minutes);
	TEST_ASSERT_EQUAL(0, clk.Time.hours);
	TEST_ASSERT_EQUAL(1, clk.Date.day);
	TEST_ASSERT_EQUAL(1, clk.Date.month);
	TEST_ASSERT_EQUAL(2015, clk.Date.year);
}

TEST(Clock, Clock_SysClock_Run_1s)
{
  SystemClock clk;
  
  SysClock_Run(1000);
  clk = SysClock_Get();
	TEST_ASSERT_EQUAL(0, clk.milliseconds);
	TEST_ASSERT_EQUAL(1, clk.Time.seconds);
	TEST_ASSERT_EQUAL(0, clk.Time.minutes);
	TEST_ASSERT_EQUAL(0, clk.Time.hours);
	TEST_ASSERT_EQUAL(1, clk.Date.day);
	TEST_ASSERT_EQUAL(1, clk.Date.month);
	TEST_ASSERT_EQUAL(2015, clk.Date.year);
}

TEST(Clock, Clock_SysClock_Run_1m)
{
  SystemClock clk;
  
  for ( int i = 0; i < 60; i ++ ) {
    SysClock_Run(1000);
  }
  clk = SysClock_Get();
	TEST_ASSERT_EQUAL(0, clk.milliseconds);
	TEST_ASSERT_EQUAL(0, clk.Time.seconds);
	TEST_ASSERT_EQUAL(1, clk.Time.minutes);
	TEST_ASSERT_EQUAL(0, clk.Time.hours);
	TEST_ASSERT_EQUAL(1, clk.Date.day);
	TEST_ASSERT_EQUAL(1, clk.Date.month);
	TEST_ASSERT_EQUAL(2015, clk.Date.year);
}

TEST(Clock, Clock_SysClock_Run_1h)
{
  SystemClock clk;
  
  for ( int i = 0; i < (60*60); i ++ ) {
    SysClock_Run(1000);
  }
  clk = SysClock_Get();
	TEST_ASSERT_EQUAL(0, clk.milliseconds);
	TEST_ASSERT_EQUAL(0, clk.Time.seconds);
	TEST_ASSERT_EQUAL(0, clk.Time.minutes);
	TEST_ASSERT_EQUAL(1, clk.Time.hours);
	TEST_ASSERT_EQUAL(1, clk.Date.day);
	TEST_ASSERT_EQUAL(1, clk.Date.month);
	TEST_ASSERT_EQUAL(2015, clk.Date.year);
}

TEST(Clock, Clock_SysClock_Run_1d)
{
  SystemClock clk;
  
  for ( int i = 0; i < (60*60*24); i ++ ) {
    SysClock_Run(1000);
  }
  clk = SysClock_Get();
	TEST_ASSERT_EQUAL(0, clk.milliseconds);
	TEST_ASSERT_EQUAL(0, clk.Time.seconds);
	TEST_ASSERT_EQUAL(0, clk.Time.minutes);
	TEST_ASSERT_EQUAL(0, clk.Time.hours);
	TEST_ASSERT_EQUAL(2, clk.Date.day);
	TEST_ASSERT_EQUAL(1, clk.Date.month);
	TEST_ASSERT_EQUAL(2015, clk.Date.year);
}

/* ======================== */
/*
    uint8_t SysClock_CheckTimeValid(ClockTime_t time);
 */
TEST(Clock, Clock_SysClock_CheckTimeValid_Bad_Second)
{
  ClockTime_t time;
  
  time.seconds = 60;
  time.minutes = 0;
  time.hours   = 0;

	TEST_ASSERT_EQUAL(0, SysClock_CheckTimeValid(time));
}

TEST(Clock, Clock_SysClock_CheckTimeValid_Bad_Minute)
{
  ClockTime_t time;
  
  time.seconds = 0;
  time.minutes = 60;
  time.hours   = 0;

	TEST_ASSERT_EQUAL(0, SysClock_CheckTimeValid(time));
}

TEST(Clock, Clock_SysClock_CheckTimeValid_Bad_Hour)
{
  ClockTime_t time;
  
  time.seconds = 0;
  time.minutes = 0;
  time.hours   = 24;

	TEST_ASSERT_EQUAL(0, SysClock_CheckTimeValid(time));
}

TEST(Clock, Clock_SysClock_CheckTimeValid_GoodTime)
{
  ClockTime_t time;
  
  time.seconds = 0;
  time.minutes = 0;
  time.hours   = 0;

	TEST_ASSERT_NOT_EQUAL(0, SysClock_CheckTimeValid(time));
}

/* ======================== */
/*
    uint8_t SysClock_CheckDateValid(ClockTime_t time);
 */
TEST(Clock, Clock_SysClock_CheckDateValid_Bad_Day_0)
{
  ClockDate_t date;
  
  date.day   = 0;
  date.month = 1;
  date.year  = 0;

	TEST_ASSERT_EQUAL(0, SysClock_CheckDateValid(date));
}

TEST(Clock, Clock_SysClock_CheckDateValid_Bad_Day_Over)
{
  ClockDate_t date;
  
  date.day   = 32;
  date.month = 1;
  date.year  = 0;

	TEST_ASSERT_EQUAL(0, SysClock_CheckDateValid(date));
}

TEST(Clock, Clock_SysClock_CheckDateValid_Bad_Month_0)
{
  ClockDate_t date;
  
  date.day   = 1;
  date.month = 0;
  date.year  = 0;

	TEST_ASSERT_EQUAL(0, SysClock_CheckDateValid(date));
}

TEST(Clock, Clock_SysClock_CheckDateValid_Bad_Month_Over)
{
  ClockDate_t date;
  
  date.day   = 1;
  date.month = 13;
  date.year  = 0;

	TEST_ASSERT_EQUAL(0, SysClock_CheckDateValid(date));
}

TEST(Clock, Clock_SysClock_CheckDateValid_GoodDate)
{
  ClockDate_t date;
  
  date.day   = 1;
  date.month = 1;
  date.year  = 0;

	TEST_ASSERT_NOT_EQUAL(0, SysClock_CheckDateValid(date));
}

/* ======================== */
/*
    void SysClock_SetTime(ClockTime_t time);
 */
TEST(Clock, Clock_SysClock_SetTime_Bad)
{
  ClockTime_t time;
  
  time.seconds = 60;
  time.minutes = 60;
  time.hours   = 24;

  SysClock_SetTime(time);
	TEST_ASSERT_EQUAL(0, SysClock_IsTimeSet());
}

TEST(Clock, Clock_SysClock_SetTime_Good)
{
  ClockTime_t time;
  
  time.seconds = 0;
  time.minutes = 0;
  time.hours   = 0;

  SysClock_SetTime(time);
	TEST_ASSERT_NOT_EQUAL(0, SysClock_IsTimeSet());
}

/* ======================== */
/*
    void SysClock_SetDate(ClockDate_t date);
 */
TEST(Clock, Clock_SysClock_SetDate_Bad)
{
  ClockDate_t date;
  
  date.day   = 1;
  date.month = 13;
  date.year  = 0;

  SysClock_SetDate(date);
	TEST_ASSERT_EQUAL(0, SysClock_IsDateSet());
}

TEST(Clock, Clock_SysClock_SetDate_Good)
{
  ClockDate_t date;
  
  date.day   = 1;
  date.month = 12;
  date.year  = 0;

  SysClock_SetDate(date);
	TEST_ASSERT_NOT_EQUAL(0, SysClock_IsDateSet());
}

/* ======================== */
/*
    uint8_t  SysClock_DaysInMonth(uint8_t month, uint16_t year);
 */
TEST(Clock, Clock_SysClock_DaysInMonth_Bad_Date)
{
	TEST_ASSERT_EQUAL(0, SysClock_DaysInMonth(0, 0));
}

TEST(Clock, Clock_SysClock_DaysInMonth_Good_Date)
{
	TEST_ASSERT_NOT_EQUAL(0, SysClock_DaysInMonth(1, 0));
}

TEST(Clock, Clock_SysClock_DaysInMonth_Feb_Leap)
{
	TEST_ASSERT_EQUAL(29, SysClock_DaysInMonth(2, 2000));
}

TEST(Clock, Clock_SysClock_DaysInMonth_Feb_Common)
{
	TEST_ASSERT_EQUAL(28, SysClock_DaysInMonth(2, 2001));
}
 
/* ======================== */
/*
    uint8_t  SysClock_IsLeapYear (uint16_t year);
 */
TEST(Clock, Clock_SysClock_IsLeapYear_1900)
{
	TEST_ASSERT_EQUAL(0, SysClock_IsLeapYear(1900));
}

TEST(Clock, Clock_SysClock_IsLeapYear_2000)
{
	TEST_ASSERT_EQUAL(1, SysClock_IsLeapYear(2000));
}

TEST(Clock, Clock_SysClock_IsLeapYear_1996)
{
	TEST_ASSERT_EQUAL(1, SysClock_IsLeapYear(1996));
}

TEST(Clock, Clock_SysClock_IsLeapYear_2001)
{
	TEST_ASSERT_EQUAL(0, SysClock_IsLeapYear(2001));
}

/* ======================== */
/*
    uint16_t  SysClock_DaysInYear (uint16_t year);
 */
TEST(Clock, Clock_SysClock_IsLeapYear_Leap)
{
	TEST_ASSERT_EQUAL(366, SysClock_DaysInYear(2000));
}

TEST(Clock, Clock_SysClock_IsLeapYear_Common)
{
	TEST_ASSERT_EQUAL(365, SysClock_DaysInYear(2001));
}


/* ======================== */
/*
    uint16_t  SysClock_GetDaysInMonth (uint8_t month);
 */
TEST(Clock, Clock_SysClock_GetDaysInMonth_M0)
{
	TEST_ASSERT_EQUAL(0, SysClock_GetDaysInMonth(0));
}

TEST(Clock, Clock_SysClock_GetDaysInMonth_M13)
{
	TEST_ASSERT_EQUAL(0, SysClock_GetDaysInMonth(13));
}

TEST(Clock, Clock_SysClock_GetDaysInMonth_Good)
{
	TEST_ASSERT_NOT_EQUAL(0, SysClock_GetDaysInMonth(6));
}

/* ======================== */
/*
    const char *SysClock_GetDaysName(uint8_t day);
 */
TEST(Clock, Clock_SysClock_GetDaysName_BadDay)
{
	TEST_ASSERT_EQUAL(NULL, SysClock_GetDaysName(8));
}

TEST(Clock, Clock_SysClock_GetDaysName_GoodDay)
{
	TEST_ASSERT_NOT_EQUAL(NULL, SysClock_GetDaysName(0));
}

/* ======================== */
/*
    const char *SysClock_GetMonthName(uint8_t month);
 */
TEST(Clock, Clock_SysClock_GetMonthName_BadMonth_Over)
{
	TEST_ASSERT_EQUAL(NULL, SysClock_GetMonthName(13));
}

TEST(Clock, Clock_SysClock_GetMonthName_BadMonth_Under)
{
	TEST_ASSERT_EQUAL(NULL, SysClock_GetMonthName(0));
}

TEST(Clock, Clock_SysClock_GetMonthName_GoodMonth)
{
	TEST_ASSERT_NOT_EQUAL(NULL, SysClock_GetMonthName(12));
}


/* ======================== */
/*
    uint8_t SysClock_IsTimeUpdated(void);
 */
TEST(Clock, Clock_SysClock_IsTimeSet_Default)
{
	TEST_ASSERT_EQUAL(0, SysClock_IsTimeUpdated());
}

TEST(Clock, Clock_SysClock_IsTimeSet_SetBadTime)
{
  ClockTime_t time;
  
  time.seconds = 0;
  time.minutes = 0;
  time.hours   = 24;
  SysClock_SetTime(time);
	TEST_ASSERT_EQUAL(0, SysClock_IsTimeUpdated());
}

TEST(Clock, Clock_SysClock_IsTimeSet_SetGoodTime)
{
  ClockTime_t time;
  
  time.seconds = 0;
  time.minutes = 0;
  time.hours   = 0;
  SysClock_SetTime(time);
	TEST_ASSERT_EQUAL(1, SysClock_IsTimeUpdated());
}

TEST(Clock, Clock_SysClock_IsTimeSet_NonLatching)
{
  ClockTime_t time;
  
  time.seconds = 0;
  time.minutes = 0;
  time.hours   = 24;
  SysClock_SetTime(time);
  SysClock_IsTimeUpdated();
	TEST_ASSERT_EQUAL(0, SysClock_IsTimeUpdated());
}

/* ======================== */
/*
    uint8_t SysClock_IsDateUpdated(void);
 */
TEST(Clock, Clock_SysClock_IsDateSet_Default)
{
	TEST_ASSERT_EQUAL(0, SysClock_IsDateSet());
}

TEST(Clock, Clock_SysClock_IsDateSet_SetBadDate)
{
  ClockDate_t date;
  
  date.day   = 1;
  date.month = 13;
  date.year  = 0;
  SysClock_SetDate(date);
	TEST_ASSERT_EQUAL(0, SysClock_IsDateUpdated());
}

TEST(Clock, Clock_SysClock_IsDateSet_SetGoodDate)
{
  ClockDate_t date;
  
  date.day   = 1;
  date.month = 12;
  date.year  = 0;
  SysClock_SetDate(date);
	TEST_ASSERT_EQUAL(1, SysClock_IsDateUpdated());
}

TEST(Clock, Clock_SysClock_IsDateSet_NonLatching)
{
  ClockDate_t date;
  
  date.day   = 1;
  date.month = 12;
  date.year  = 0;
  SysClock_SetDate(date);
  SysClock_IsDateUpdated();
	TEST_ASSERT_EQUAL(0, SysClock_IsDateUpdated());
}

/* ======================== */
/* ======================== */
TEST_GROUP_RUNNER(Clock)
{
	RUN_TEST_CASE(Clock, Clock_SysClock_Init_TimeSetFlag_NotSet);
	RUN_TEST_CASE(Clock, Clock_SysClock_Init_DateSetFlag_NotSet);
  
	RUN_TEST_CASE(Clock, Clock_SysClock_Run_0);
	RUN_TEST_CASE(Clock, Clock_SysClock_Run_1);
	RUN_TEST_CASE(Clock, Clock_SysClock_Run_1s);
	RUN_TEST_CASE(Clock, Clock_SysClock_Run_1m);
	RUN_TEST_CASE(Clock, Clock_SysClock_Run_1h);
	RUN_TEST_CASE(Clock, Clock_SysClock_Run_1d);

	RUN_TEST_CASE(Clock, Clock_SysClock_CheckTimeValid_Bad_Second);
	RUN_TEST_CASE(Clock, Clock_SysClock_CheckTimeValid_Bad_Minute);
	RUN_TEST_CASE(Clock, Clock_SysClock_CheckTimeValid_Bad_Hour);
	RUN_TEST_CASE(Clock, Clock_SysClock_CheckTimeValid_GoodTime);

	RUN_TEST_CASE(Clock, Clock_SysClock_CheckDateValid_Bad_Day_0);
	RUN_TEST_CASE(Clock, Clock_SysClock_CheckDateValid_Bad_Day_Over);
  RUN_TEST_CASE(Clock, Clock_SysClock_CheckDateValid_Bad_Month_0);
  RUN_TEST_CASE(Clock, Clock_SysClock_CheckDateValid_Bad_Month_Over);
  RUN_TEST_CASE(Clock, Clock_SysClock_CheckDateValid_GoodDate);
  
  RUN_TEST_CASE(Clock, Clock_SysClock_SetTime_Bad);
  RUN_TEST_CASE(Clock, Clock_SysClock_SetTime_Good);
  
  RUN_TEST_CASE(Clock, Clock_SysClock_SetDate_Bad);
  RUN_TEST_CASE(Clock, Clock_SysClock_SetDate_Good);
  
  RUN_TEST_CASE(Clock, Clock_SysClock_DaysInMonth_Bad_Date);
  RUN_TEST_CASE(Clock, Clock_SysClock_DaysInMonth_Good_Date);
  RUN_TEST_CASE(Clock, Clock_SysClock_DaysInMonth_Feb_Leap);
  RUN_TEST_CASE(Clock, Clock_SysClock_DaysInMonth_Feb_Common);
  
  RUN_TEST_CASE(Clock, Clock_SysClock_IsLeapYear_1900);
  RUN_TEST_CASE(Clock, Clock_SysClock_IsLeapYear_2000);
  RUN_TEST_CASE(Clock, Clock_SysClock_IsLeapYear_1996);
  RUN_TEST_CASE(Clock, Clock_SysClock_IsLeapYear_2001);

  RUN_TEST_CASE(Clock, Clock_SysClock_IsLeapYear_Leap);
  RUN_TEST_CASE(Clock, Clock_SysClock_IsLeapYear_Common);

  RUN_TEST_CASE(Clock, Clock_SysClock_GetDaysInMonth_M0);
  RUN_TEST_CASE(Clock, Clock_SysClock_GetDaysInMonth_M13);
  RUN_TEST_CASE(Clock, Clock_SysClock_GetDaysInMonth_Good);
  
  RUN_TEST_CASE(Clock, Clock_SysClock_GetDaysName_BadDay);
  RUN_TEST_CASE(Clock, Clock_SysClock_GetDaysName_GoodDay);

  RUN_TEST_CASE(Clock, Clock_SysClock_GetMonthName_BadMonth_Over);
  RUN_TEST_CASE(Clock, Clock_SysClock_GetMonthName_BadMonth_Under);
  RUN_TEST_CASE(Clock, Clock_SysClock_GetMonthName_GoodMonth);
  
  RUN_TEST_CASE(Clock, Clock_SysClock_IsTimeSet_Default);
  RUN_TEST_CASE(Clock, Clock_SysClock_IsTimeSet_SetBadTime);
  RUN_TEST_CASE(Clock, Clock_SysClock_IsTimeSet_SetGoodTime);
  RUN_TEST_CASE(Clock, Clock_SysClock_IsTimeSet_NonLatching);

  RUN_TEST_CASE(Clock, Clock_SysClock_IsDateSet_Default);
  RUN_TEST_CASE(Clock, Clock_SysClock_IsDateSet_SetBadDate);
  RUN_TEST_CASE(Clock,Clock_SysClock_IsDateSet_SetGoodDate);
  RUN_TEST_CASE(Clock, Clock_SysClock_IsDateSet_NonLatching);  
}

