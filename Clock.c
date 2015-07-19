
#include <stdio.h>

#include "Clock.h"

static SystemClock SoftwareClock;

const uint8_t DaysInMonth[12] = {
  31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

const char *DayNames[7] = {
  "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

const char *MonthNames[12] = {
  "January", "February", "March", "April", "May", "June", 
  "July", "August", "September", "October", "November", "December"
};
// IsTimeUpdated;
// IsDateUpdated;

void SystemClock_Init(ClockTime_t time, ClockDate_t date)
{
  SoftwareClock.Time = time;
  SoftwareClock.Date = date;
	SoftwareClock.milliseconds = 0;
  SoftwareClock.TimeSet      = 0;
  SoftwareClock.TimeUpdated  = 0;
  SoftwareClock.DateSet      = 0;
  SoftwareClock.DateUpdated  = 0;
}

SystemClock SysClock_Get(void)
{
  return SoftwareClock;
}

/* ============================================== */
void SysClock_Run(uint16_t milliseconds)
{
  SoftwareClock.milliseconds += milliseconds;
  while ( SoftwareClock.milliseconds >= 1000 ) {
    SoftwareClock.milliseconds -= 1000;
    
    SoftwareClock.Time.seconds ++;
    if ( SoftwareClock.Time.seconds >= 60 ) {
      SoftwareClock.Time.seconds = 0;
      
      SoftwareClock.Time.minutes++;
      if ( SoftwareClock.Time.minutes >= 60 ) {
        SoftwareClock.Time.minutes = 0;
        
        SoftwareClock.Time.hours ++;
        if ( SoftwareClock.Time.hours >= 24 ) {
          SoftwareClock.Time.hours = 0;
          
          SoftwareClock.Date.day++;
          if ( SoftwareClock.Date.day >= SysClock_DaysInMonth(SoftwareClock.Date.month, SoftwareClock.Date.year)) {
            SoftwareClock.Date.day = 1;
            SoftwareClock.Date.month++;
            if ( SoftwareClock.Date.month > 12 ) {
              SoftwareClock.Date.year ++;
              SoftwareClock.Date.month = 1;
            }
          }
        }
      }
    }
  }
}

/* ============================================== */
uint16_t SysClock_GetMilliseconds(void)
{
  return SoftwareClock.milliseconds;
}

/* ============================================== */
uint8_t SysClock_CheckTimeValid(ClockTime_t time)
{
  if (( time.seconds >= 60 ) ||
      ( time.minutes >= 60 ) ||
      ( time.hours   >= 24 )) {
    return 0;
  }
  return 1;
}

uint8_t SysClock_CheckDateValid(ClockDate_t date)
{
  if (( date.month == 0 ) || ( date.month > 12 ))
    return 0;
  if (( date.day == 0 ) || ( date.day >= SysClock_DaysInMonth(date.month, date.year) ))
    return 0;
  return 1;
}

/* ============================================== */
void SysClock_SetTime(ClockTime_t time)
{  
  if ( SysClock_CheckTimeValid(time) != 0 ) {
    SoftwareClock.Time = time;
    SoftwareClock.TimeSet = 1;
    SoftwareClock.TimeUpdated = 1;
  }
}

ClockTime_t SysClock_GetTime(void)
{  
  return SoftwareClock.Time;
}

uint8_t SysClock_IsTimeSet(void)
{
  return SoftwareClock.TimeSet;
}

uint8_t SysClock_IsTimeUpdated(void)
{
  uint8_t flag = SoftwareClock.TimeUpdated;
  SoftwareClock.TimeUpdated = 0;
  return flag;
}

ClockDate_t SysClock_GetDate(void)
{  
  return SoftwareClock.Date;
}

void SysClock_SetDate(ClockDate_t date)
{  
  if ( SysClock_CheckDateValid(date) != 0 ) {
    SoftwareClock.Date = date;
    SoftwareClock.DateSet = 1;
    SoftwareClock.DateUpdated = 1;
  }
}

uint8_t SysClock_IsDateSet(void)
{
  return SoftwareClock.DateSet;
}

uint8_t SysClock_IsDateUpdated(void)
{
  uint8_t flag = SoftwareClock.DateUpdated;
  SoftwareClock.DateUpdated = 0;
  return flag;
}

/* ============================================== */
uint16_t SysClock_DaysInYear (uint16_t year)
{
  return (SysClock_IsLeapYear(year) == 0 )? 365: 366;
}

uint8_t  SysClock_IsLeapYear (uint16_t year)
{
 if (((((year % 4) == 0) && ((year % 100) != 0))) ||
      (( year % 400 ) == 0 )) {
    return 1;
  }
  return 0;
}

uint8_t  SysClock_DaysInMonth(uint8_t month, uint16_t year)
{
  if (( month == 0 ) || ( month > 12 )) {
    return 0;
  }
  if (( month == 2 ) && ( SysClock_IsLeapYear(year) != 0 )) {
    return 29;
  }
  return DaysInMonth[month - 1];
}

/* ============================================== */
uint8_t SysClock_GetDaysInMonth(uint8_t month)
{
  if (( month == 0) || ( month > 12 )) {
    return 0;
  }
  return DaysInMonth[month - 1];
}

const char *SysClock_GetDaysName(uint8_t day)
{
  if ( day > 7 ) {
    return NULL;
  }
  return DayNames[day];
}

const char *SysClock_GetMonthName(uint8_t month)
{
  if (( month == 0) || ( month > 12 )) {
    return NULL;
  }
  return MonthNames[month - 1];
}

/* ============================================== */
/* ============================================== */


