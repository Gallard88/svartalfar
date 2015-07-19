
#include <stdio.h>
#include <stdarg.h>
#include "Logging.h"
#include "Timing.h"

static uint8_t LogEn;

void Log_SetEnable(uint8_t en)
{
  LogEn = en;
}

uint8_t Log_GetEnable(void)
{
  return LogEn;
}

void Logf(const char *module, const char *msg)
{
  char buff[512];
  if ( LogEn == 0 ) {
    return;
  }
  
  uint32_t  time = Timing_ReadStamp();
  snprintf(buff, sizeof(buff),"%04lu.%03u, %s, %s\r\n", 
    (uint32_t)(time / 1000u), (uint16_t)(time % 1000u), module, msg );
  Log_PrintMsg(buff);
}

void LogPrintf(const char *module, const char *fmt, ... )
{
  char buff[512];
	va_list args;
  
  if ( LogEn == 0 ) {
    return;
  }

	va_start(args,fmt);
	vsnprintf(buff, sizeof(buff),fmt,args);
	va_end(args);
  
  Logf(module, buff);
}

