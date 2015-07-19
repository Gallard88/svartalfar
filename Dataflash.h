#ifndef DATAFLASH_H
#define DATAFLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "DriverTemplate_SPI.h"

/**************
 *  SPI_Driver_t Dataflash_SPI
 *  *** User Defined Function ***
 *  This is a user defined function. Used to control
 *  dependencies. The SPI driver is now a compile time dep.
 */
extern const SPI_Driver_t Dataflash_SPI;

/**************
 *  DFToken_t
 *  This controls access to the data flash. 
 *  Stops a module read/writing flash it shouldn't.
 */
typedef struct {
 uint16_t start_page;
 uint16_t number_pages;
} DFToken_t;

/**************
 *  DataFlash_CheckToken()
 *  This function checks if the given page falls with in the 
 *  allocated space. Returns the absolute page address.
 */
uint16_t DataFlash_CheckToken(const DFToken_t *token, uint16_t page);

/**************
 *  DataFlash_Chip_t
 *  A enum showing the supported chip types.
 */
typedef enum {
  
	FLASH_DEVID_UNKNOWN,
	FLASH_DEVID_321,
	FLASH_DEVID_161,
	FLASH_DEVID_081
  
} DataFlash_Chip_t;

/**************
 *  Local #def's
 */
#define DATAFLASH_FLASHBUF1	0			// buffer 1
#define DATAFLASH_FLASHBUF2	1			// buffer 2

#define DATAFLASH_PAGESIZE_NORMAL  512
#define DATAFLASH_PAGESIZE_EXT     528

/**************
 *  DataFlash_Init()
 *  Initialises the data flash module. 
 */
void DataFlash_Init(void);

/**************
 *  DataFlash_ReadChipType()
 *  Interrogates the data flash chip and reports chip type.
 */
DataFlash_Chip_t DataFlash_ReadChipType(void);

/**************
 *  DataFlash_PageRead()
 *  Used to bypass the buffer system and read directly from the chip.
 */
void DataFlash_PageRead(const DFToken_t *token, uint16_t page, uint16_t offset, uint16_t size, uint8_t *dest);

/**************
 *  DataFlash_ReadFlashToBuffer()
 *  DataFlash_WriteBufferToFlash()
 *  Used to move data to/from the flash memory into it's internal buffer.
 *  Note: buffer writes can takes up to 20ms to complete.
 */
void DataFlash_ReadFlashToBuffer(const DFToken_t *token, uint16_t page, uint8_t whichbuf);
void DataFlash_WriteBufferToFlash(const DFToken_t *token, uint16_t page, uint8_t whichbuf);

/**************
 *  DataFlash_BufferRead()
 *  DataFlash_BufferWrite()
 *  Read/Write to data flash IC's internal memory buffers.
 */
void DataFlash_BufferRead (const DFToken_t *token, uint16_t page, uint16_t offset, uint16_t size, uint8_t whichbuf, uint8_t *dest);
void DataFlash_BufferWrite (const DFToken_t *token, uint16_t page, uint16_t offset, uint16_t size, uint8_t whichbuf, uint8_t *src);

#ifdef __cplusplus
}
#endif

#endif
