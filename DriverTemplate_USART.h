#ifndef __USART_DRIVER__
#define __USART_DRIVER__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef uint8_t (*USART_Driver_IsRxReady)(void);
typedef uint8_t (*USART_Driver_GetChar)  (void);
typedef void    (*USART_Driver_PutChar)  (uint8_t c);
typedef void    (*USART_Driver_PutS)(const char *s);
typedef uint8_t (*USART_Driver_IsTxEmpty)(void);

typedef struct {
  USART_Driver_IsRxReady	IsRxReady;
  USART_Driver_GetChar  	GetChar;
  USART_Driver_PutChar    PutChar;	
  USART_Driver_PutS       PutS;
  USART_Driver_IsTxEmpty  IsTxEmpty;
} USART_Driver_t;

#ifdef __cplusplus
}
#endif

#endif
