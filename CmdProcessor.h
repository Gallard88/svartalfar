#ifndef CMD_PROCESSOR_H
#define CMD_PROCESSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* ======================================= */
/**************
 *  CmdProc_SkipSpace()
 *  Arg: C-style string
 *  Increments pointer until end of string or non space chars is found.
 *  Returns incremented pointer.
 */
const char *CmdProc_SkipSpace(const char *buf);

/**************
 *  CmdProc_SkipChars()
 *  Arg: C-style string
 *  Increments pointer until end of string or space (0x20) char is found.
 *  Returns incremented pointer.
 */
const char *CmdProc_SkipChars(const char *buf);

/* ======================================= */
typedef struct {
  uint32_t size;  /* Size of the buffer below.  */
  char *buffer;   /* A place to store data      */
} MsgBuff_t;

/**************
 *  CmdProc_Print()
 *  CmdProc_Printf()
 *  Concatenates a message to MsgBuff_t. Only writes message 
 *  if there is sufficient room. 
 */
void CmdProc_Print (MsgBuff_t *ptr, const char *msg);
void CmdProc_Printf(MsgBuff_t *ptr, const char *fmt, ...);

/**************
 *  CmdProc_CRLF()
 *  Appends a carriage return line feed to 
 *  MsgBuff_t. Only appends if there is space to do so.
 */
void CmdProc_CRLF(MsgBuff_t *ptr);

/**************
 *  CmdProc_Clear()
 *  Clears MsgBuff_t.
 */
void CmdProc_Clear(MsgBuff_t *ptr);

/**************
 *  CmdProc_BuildLine()
 *  Builds a line one char at a time.
 *  On \r or \n it will return thr length of the string.
 */
uint32_t CmdProc_BuildLine(MsgBuff_t *line, char c);

/* ======================================= */
typedef int8_t (*SystemCmd)(const char *arg, MsgBuff_t *resp);

typedef struct 
{
	const char * const cmd; /* full command word */
  SystemCmd vector;       /* function to call  */
} CmdTable_t;

/**************
 *  CmdProc_Lookup()
 *
 *  Function steps through CmdTable_t array until matching cmd is found. 
 *  If no match is made, function returns -1. Command is defined as start of string 
 *  till either EOL or first space (0x20). Argument is passed to vector function. 
 *  Argument is defined as the first non-space char after command.
 *  System expects CmdTable_t to be terminated with a null entry { NULL, NULL }
 */
int8_t CmdProc_Lookup(const CmdTable_t *table, const char *line, MsgBuff_t *resp);


#ifdef __cplusplus
}
#endif

#endif /* COMMANDPROCESSOR_H_ */