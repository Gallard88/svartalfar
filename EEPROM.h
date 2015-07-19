#ifndef EEPROM_H
#define EEPROM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Dataflash.h"

/*
	EEPROM emulation.
	This is a layer build on top of a data flash chip to emulate byte 
  accessible EEPROM. This system uses 512 byte pages. Due to internal architecture 
  Read/Write operations *must* not violate this boundary.	System caches reads and 
  writes, ejecting the oldest page first. Or dirty pages can be written manually by 
  calling EE_WriteCachePage();
*/

/* This token dictates the size and it's location on the data flash chip. */
extern const DFToken_t EE_Token;

#define EE_PAGESIZE	DATAFLASH_PAGESIZE_NORMAL

/**************
 *  EE_Init()
 *  Initialises the cache buffer using dynamic memory.
 *  This operation is only done once, and is never free'd.
 */
void EE_Init (uint16_t num_pages);

/**************
 *  EE_NumberDirtyPages()
 *  Returns the number of dirty pages waiting to be written.
 */
uint16_t EE_NumberDirtyPages(void);

/**************
 *  EE_WriteCachePage()
 *  Should be called regularly, around once a second.
 *  Writes dirty EEPROM pages to the flash chip.
 */
void EE_WriteCachePage(void);


/* Read Functions */
uint8_t  EE_Read1     (uint32_t addr);
uint16_t EE_Read2     (uint32_t addr);
uint32_t EE_Read4     (uint32_t addr);
void     EE_Read      (uint32_t addr, uint8_t *buf, uint16_t count);
uint16_t EE_ReadString(uint32_t addr, char *dest,   uint16_t size);

/* Write Functions  */
void EE_Write1   (uint32_t addr, uint8_t data);
void EE_Write2   (uint32_t addr, uint16_t data);
void EE_Write4   (uint32_t addr, uint32_t data);
void EE_Write    (uint32_t addr, const uint8_t *buf, uint16_t count);
void EE_PutString(uint32_t addr, const char *src,    uint16_t size);

#ifdef __cplusplus
}
#endif

#endif

