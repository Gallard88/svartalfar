#include <stdio.h>

#include "ModuleMonitor.h"
#include "Timing.h"

uint8_t ModuleMonitor_Run(uint32_t *timer, uint32_t timeout, ModuleMon_Timeout func)
{
  if ( timer == NULL ) {
    return 0;
  }
  if ( Timing_CheckDelay(*timer, TIMER_SECONDS(timeout)) ) {
    (*func)();
    ModuleMonitor_Clear(timer);
    return 1;
  }
  return 0;
}

void ModuleMonitor_Clear(uint32_t *timer)
{
  if ( timer != NULL ) {
    *timer = Timing_ReadStamp();
  }
}

