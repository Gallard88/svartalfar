#ifndef __SPI_DRIVER__
#define __SPI_DRIVER__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef uint8_t (*SPI_Driver_Read)(void);
typedef void    (*SPI_Driver_Write)(uint8_t data);
typedef void    (*SPI_Driver_ReadWrite)(uint8_t *buffer, uint16_t length);
typedef void    (*SPI_Driver_InitCsPin)(void);
typedef void    (*SPI_Driver_CsOn)     (void);
typedef void    (*SPI_Driver_CsOff)    (void);

typedef struct {
  SPI_Driver_Read       Read;
  SPI_Driver_Write      Write;
  SPI_Driver_ReadWrite  ReadWrite;
  SPI_Driver_InitCsPin  InitCsPin;
  SPI_Driver_CsOn       CsOn;
  SPI_Driver_CsOff      CsOff;
} SPI_Driver_t;

#ifdef __cplusplus
}
#endif

#endif
