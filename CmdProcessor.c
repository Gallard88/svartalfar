
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "CmdProcessor.h"

const char *CmdProc_SkipSpace(const char *buf)
{
  if ( buf == NULL ) {
    return NULL;
  }
	while (( buf[0] == ' ' ) && ( buf[0] != 0 )) {
		++buf;
  }
	return buf;
}

const char *CmdProc_SkipChars(const char *buf)
{
  if ( buf == NULL ) {
    return NULL;
  }
	while (( buf[0] != 0 ) && ( buf[0] != ' ' )) {
		++buf;
  }
	return buf;
}

void CmdProc_CRLF(MsgBuff_t *ptr)
{
  if (( ptr != NULL ) &&
      ( ptr->buffer != NULL ) &&
      ((strlen(ptr->buffer)+2) < ptr->size)) {

    strcat(ptr->buffer, "\r\n");
  }
}

void CmdProc_Clear(MsgBuff_t *ptr)
{
  if (( ptr != NULL ) && ( ptr->buffer != NULL )) {
    ptr->buffer[0] = 0;
  }
}

void CmdProc_Print (MsgBuff_t *ptr, const char *msg)
{
  if (( ptr != NULL ) &&
      ( ptr->buffer != NULL ) &&
      ((strlen(ptr->buffer)+strlen(msg)) < ptr->size)) {

    strcat(ptr->buffer, msg);
  }
}

void CmdProc_Printf(MsgBuff_t *ptr, const char *fmt, ...)
{
  char buff[512];
	va_list args;

  if (( ptr != NULL ) &&
      ( fmt != NULL ) &&
      ( ptr->buffer != NULL )) {

    va_start(args,fmt);
    vsnprintf(buff, sizeof(buff),fmt,args);
    va_end(args);
    CmdProc_Print(ptr, buff);
  }
}

uint32_t CmdProc_BuildLine(MsgBuff_t *line, char c)
{
  if (( line != NULL ) && ( line->buffer != NULL )) {
    uint32_t size = strlen(line->buffer);
    if (( c == '\r' ) || ( c == '\n' )) {
      return size;
    }
    if (( size+1) < line->size ) {
      line->buffer[size] = c;
      line->buffer[size+1] = 0; /* terminate string */
    }
  }
  return 0;
}

int8_t CmdProc_Lookup(const CmdTable_t *table, const char *line, MsgBuff_t *resp)
{
  const char *ptr;
  uint32_t length = 0;

  if (( line == NULL ) ||
      ( table == NULL ) ||
      (resp == NULL )) {
    return -1;
  }
  ptr = line;
  while (( *ptr != ' ' ) && ( *ptr != 0 )) {
    ptr++;
    length ++;
  }

  while (( table->cmd != NULL ) &&
         ( table->vector != NULL )) {

    if (( strncmp(table->cmd, line, strlen(table->cmd)) == 0 ) &&
        ( strlen(table->cmd) == length )) {

      ptr = CmdProc_SkipChars(line);
      ptr = CmdProc_SkipSpace(ptr);
      return (table->vector)(ptr, resp);
    }
    table++;
  }
  return -1;
}


