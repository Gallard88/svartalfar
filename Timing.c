
#include <stdio.h>

#include "Timing.h"

static volatile uint32_t ElapsedTime;

void Timing_Increment(void)
{
  ElapsedTime += 1;
}

void Timing_LoadValue(uint32_t ms)
{
  ElapsedTime = ms;
}

uint32_t Timing_ReadStamp(void)
{
  return ElapsedTime;
}

uint8_t Timing_CheckDelay(uint32_t stamp, uint32_t delay)
{
  if ( ElapsedTime < stamp ) {
    return 0;
  }
  uint32_t diff = ElapsedTime - stamp;
  if ( diff >= delay ) {
    return 1;
  }
  return 0;
}

