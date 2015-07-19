#ifndef TIME_H
#define TIME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**************
 *  Timing
 *  The purpose of this module is to create an ever incrementing 
 *  time stamp that can be used to measure duration.
 *
 *  This module uses a uint32_t to measure milliseconds elapsed. This 
 *  will give abut 49.7 days (7 weeks) before it overflows.
 */

/**************
 *  Timing_Increment()
 *  Increments the elapsed milliseconds timer by 1.
 *  The accuracy of this module depends on how accurately this function is called. 
 *  For best results call from an interrupt.
 */
void Timing_Increment(void);

/**************
 *  Timing_LoadValue()
 *  Set the elapsed milliseconds timer. 
 *  Useful for testing purposes and debugging.
 */
void Timing_LoadValue(uint32_t ms);

/**************
 *  Timing_ReadStamp()
 *  Returns the current time stamp.
 */
uint32_t Timing_ReadStamp(void);

/**************
 *  Timing_CheckDelay()
 *  Used to measure time outs. 
 *  Returns 0 if stamp + delay < elapsed time.
 *  Returns 1 if stamp + delay > elapsed time.
 */
uint8_t Timing_CheckDelay(uint32_t stamp, uint32_t delay);

#define TIMER_FORCE_TIMEOUT(x)	x = 0
#define TIMER_SECONDS(x)       (x*1000)
#define TIMER_MINUTES(x)       (x*1000*60)

#ifdef __cplusplus
}
#endif

#endif /* TIME_H */