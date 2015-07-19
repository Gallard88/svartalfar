#ifndef LOGGING_H
#define LOGGING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**************
 *  Logging
 *  The purpose of this module is for reporting data in system which may 
 *  not contain a full time keeping mechanism. These functions will print:
 *  a message with a time stamp relevant to when the system started, a module 
 *  to help diagnose where the message comes from, as well as the message itself,
 *  which may be a fixed string or a printf sytle message.
 *
 *  Log format is as follows:
 *  Minute.millisecond, MODULE, Message\r\n.
 *
 *  Note: For the time stamp to work, this modules requires the 
 *  Timing_Increment() function to be run.
 */
/**************
 *  Log_SetEnable()
 *  Log_GetEnable()
 *
 *  Turn on/off logging. Report this status.
 */
void    Log_SetEnable(uint8_t en);
uint8_t Log_GetEnable(void);

/**************
 *  Logf()
 *
 *  This function will print a fixed string as specified by msg.
 *  As this is a fixed string this function is less capable, but 
 *  faster than calling LogPrintf. These messages are enabled/disabled 
 *  by Log_SetEnable(). The output message will be reported via a 
 *  call to Log_PrintMsg()
 */
void Logf(const char *module, const char *msg );

/**************
 *  LogPrintf()
 *
 *  Uses the same printf modifiers to format a string.
 *  These messages are enabled/disabled by Log_SetEnable().
 *  The output message will be reported via a call to Log_PrintMsg()
 */
void LogPrintf(const char *module, const char *fmt, ... );

/**************
 *  Log_PrintMsg()
 *
 *  *** This is a user defined function ***
 *  This function is called by both Logf and LogPrintf 
 *  with the final message as it's argument. This enabled 
 *  the user to determine where and how this information is reported.
 */
extern void Log_PrintMsg(const char *msg);

#ifdef __cplusplus
}
#endif

#endif
