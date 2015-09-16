#ifndef MODULE_MONITOR_H
#define MODULE_MONITOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**************
 *  typedef for timeout function.
 */
typedef void (*ModuleMon_Timeout)(void);

/**************
 *  ModuleMonitor_Run()
 *  Runs the timer.
 *  If the timer expires fuc is called.
 *  Returns 1 when the timer expires.
 */
uint8_t ModuleMonitor_Run(uint32_t *timer, uint32_t timeout, ModuleMon_Timeout func);

/**************
 *  ModuleMonitor_Clear()
 *  Resets the Module Monitor timer.
 */
void ModuleMonitor_Clear(uint32_t *timer);


#ifdef __cplusplus
}
#endif

#endif

