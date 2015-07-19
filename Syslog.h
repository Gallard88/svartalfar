#ifndef SYSLOG_H
#define SYSLOG_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	lDebug = 0, /* Debug level message.                */
	lInfo,      /* Informational message.              */
	lNotice,    /* Normal, but significant, condition. */
	lWarning,   /* Warning conditions.                 */
	lError,     /* Error conditions.                   */
	lCritical,  /* Critical conditions.                */
	lAlert,     /* Action must be taken immediately.   */
	lEmerg,     /* System is unusable.                 */
	lDiag,      /* Diagnostic data                     */
	lUpdate     /* System setting has been updated.    */
} LogLevel;

/**************
 *  Syslog()
 *  Args:
 *    level    - severity level of the message.
 *    Module   - A string to label which module is making the entry.
 *    fmt, ... - Printf style format string.
 *
 *  Functions formats message and calls Syslog_Print()
 *  Output is of the form:
 *  YYYY-MM-DD, HH:MM:SS.mmm, Level, Module, .....\r\n
 */
void Syslog(LogLevel level, const char *module, const char *fmt, ... );

/**************
 *  Syslog_LevelText()
 *  Returns warning text for a given severity level.
 */
const char *Syslog_LevelText(LogLevel level);

/**************
 *  Syslog_Print()
 *
 *  *** This is a user defined function ***
 *  This function is called by Syslog 
 *  with the final message as it's argument. This enabled 
 *  the user to determine where and how this information is reported.
 */
extern void Syslog_Print(LogLevel level, const char *msg);

#ifdef __cplusplus
}
#endif

#endif
