
#include <stdio.h>
#include <stdarg.h>
#include "Syslog.h"
#include "Clock.h"

const char * LogLevel_Text[] = {
  
	"Debug",    /*  lDebug,    Debug level message.                */
	"Info",     /*  lInfo,     Informational message.              */
	"Notice"    /*  lNotice,   Normal, but significant, condition. */
	"Warning",  /*  lWarning,  Warning conditions.                 */
	"Error",    /*  lError,    Error conditions.                   */
	"Critical", /*  lCritical, Critical conditions.                */
	"Alert",    /*  lAlert,    Action must be taken immediately.   */
	"Emerg",    /*  lEmerg,    System is unusable.                 */
	"Diag",     /*  lDiag,     Diagnostic data                     */
	"Update"    /*  lUpdate    System setting has been updated.    */
};

void Syslog(LogLevel level, const char *module, const char *fmt, ... )
{
	va_list args;
  char fmt_buff[512];
  char combined[1024];
  char time_str[64];
  char date_str[64];
  ClockTime_t time;
  ClockDate_t date;
  
	va_start(args,fmt);
	vsnprintf(fmt_buff, sizeof(fmt_buff),fmt,args);
	va_end(args);
  
  time = SysClock_GetTime();
  snprintf(time_str, sizeof(time_str),"%02u:%02u:%02u.%03u", 
    time.hours, time.minutes, time.seconds, SysClock_GetMilliseconds());

  date = SysClock_GetDate();
  snprintf(date_str, sizeof(date_str),"%04d-%02u-%02u", 
    date.year, date.month, date.day);
  
  snprintf(combined, sizeof(combined), "%s, %s, %s, %s, %s\r\n",
    date_str, time_str, Syslog_LevelText(level), module, fmt_buff);
  
  Syslog_Print(level, combined);
}

const char *Syslog_LevelText(LogLevel level)
{
  return LogLevel_Text[level];
}

