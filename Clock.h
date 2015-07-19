#ifndef CLOCK_H
#define CLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
/**************
 *  Clock Module.
 *  This module is used to keep calendar time.
 */
typedef struct {
	uint8_t seconds; /* 0 - 59 */
	uint8_t minutes; /* 0 - 59 */
	uint8_t hours;   /* 0 - 23 */
} ClockTime_t; 

typedef struct {
	uint8_t day;   /* 1 - 31 */
	uint8_t month; /* 1 - 12 */
	int16_t year;
} ClockDate_t; 

typedef struct {
  ClockTime_t Time;
  ClockDate_t Date;
	uint16_t milliseconds;
  unsigned TimeSet:1;
  unsigned TimeUpdated:1;
  unsigned DateSet:1;
  unsigned DateUpdated:1;
  
} SystemClock;

/**************
 *  Some useful constants...
 */
#define CLOCK_MINUTES_IN_DAY (60u*24u)
#define CLOCK_SECONDS_IN_DAY (60u*60u*24u)
 
/**************
 *  SystemClock_Init()
 *  Loads the software clock with a default value.
 *  Does not set either TimeSet or DateSet flag.
 */
void SystemClock_Init(ClockTime_t time, ClockDate_t date);

/**************
 *  SysClock_Run()
 *  Arg: how many milliseconds have passed since the last call.
 *  Increments the software clock accordingly.
 */
void SysClock_Run(uint16_t milliseconds);

/**************
 *  SysClock_GetMilliseconds()
 *  Returns the Milliseconds field of the software clock.
 */
uint16_t SysClock_GetMilliseconds(void);

/**************
 *  SysClock_Get()
 *  Returns a full copy of the software clock.
 */
SystemClock SysClock_Get(void);

 /**************
 *  SysClock_CheckTimeValid()
 *  SysClock_CheckDateValid()
 *  Arg: ClockTime_t/ClockDate_t struct.
 *  Returns, 0 if time/date not valid.
 *  Non-Zero if valid.
 */
uint8_t SysClock_CheckTimeValid(ClockTime_t time);
uint8_t SysClock_CheckDateValid(ClockDate_t date);

/**************
 *  SysClock_SetTime()
 *  SysClock_SetDate()
 *  Sets the software clock to time/date. Only updates if data if valid.
 *  Once updated the TimeSet/DateSet flag is set.
 *  Once updated the TimeUpdated/DateUpdated flag is set.
 */
void SysClock_SetTime(ClockTime_t time);
void SysClock_SetDate(ClockDate_t date);

/**************
 *  SysClock_GetTime()
 *  SysClock_GetDate()
 *  Returns the current Time/Date as stored in the software clock.
 */
ClockTime_t SysClock_GetTime(void);
ClockDate_t SysClock_GetDate(void);

/**************
 *  SysClock_IsTimeUpdated()
 *  SysClock_IsDateUpdated()
 *  Returns non-zero once the time/date has been updated. 
 *  Flag is cleared on read.
 *  This is useful for edge detection.
 */
uint8_t SysClock_IsTimeUpdated(void);
uint8_t SysClock_IsDateUpdated(void);

/**************
 *  SysClock_DaysInYear()
 */
uint16_t SysClock_DaysInYear (uint16_t year);

/**************
 *  SysClock_IsLeapYear()
 *  Returns non-zer if leap year.
 */
uint8_t  SysClock_IsLeapYear (uint16_t year);

/**************
 *  SysClock_DaysInMonth()
 */
uint8_t  SysClock_DaysInMonth(uint8_t month, uint16_t year);

/**************
 *  SysClock_IsTimeSet()
 *  SysClock_IsDateSet()
 *  Returns non-zero if the time and date have been set since power up.
 */
uint8_t SysClock_IsTimeSet(void);
uint8_t SysClock_IsDateSet(void);

/**************
 *  SysClock_GetDaysInMonth()
 *  Returns 0 if month is invalid
 */
uint8_t SysClock_GetDaysInMonth(uint8_t month);

/**************
 *  SysClock_GetDaysName()
 *  Returns NULL is day is invalid.
 */
const char *SysClock_GetDaysName(uint8_t day);

/**************
 *  SysClock_GetMonthName()
 *  Returns NULL if month is invalid
 */
const char *SysClock_GetMonthName(uint8_t month);

/**************
 *  The following are proposed functions that maybe implemented
 *  when I have time, or a need.
 */
/* 
uint32_t    SysClock_ClockToEpoch(SystemClock clock);
SystemClock SysClock_EpochToClock(uint32_t time);

SystemClock SysClock_Convert_UTCToLocal(SystemClock clock, int16_t timezone);
SystemClock SysClock_Convert_LocalToUTC(SystemClock clock, int16_t timezone);
*/

#ifdef __cplusplus
}
#endif

#endif
