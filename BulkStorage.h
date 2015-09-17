#ifndef BULK_DATA_H
#define BULK_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "Dataflash.h"

/**************
 * Bulk data storage. 
 * Useful for storing large chunks of data, like flash images,
 * or SSL certificates. For when you want to store data, but don't
 * want the overhead of an entire file system.
 * Features:
 *  - 32 bit address. 
 *  - Byte level address'
 *  - Checksum to test page integrity.
 *  - CRC to test image integrity.
 */

#define BULK_NOTE_SIZE  64 

/**************
 *  
 */
struct BulkStorage_Header
{
  uint32_t hash;  
  uint32_t timestamp;
  uint8_t notes[BULK_NOTE_SIZE];  
};
 
/**************
 *  
 */
struct BulkStorage {
  const DFToken_t *token;
  struct BulkStorage_Header head;
  uint16_t loaded_page; /* which page is current loaded into the buffer. */
  uint8_t  dirty:1;     /* Does this data need to be written to flash?   */
  uint8_t  write:1;     /* Whether we are reading or writing to flash.   */
  uint8_t  marked:1;    /* 1 if any pages have been written              */
  uint8_t  data[DATAFLASH_PAGESIZE_EXT];   
};  

/* ========================================================= */
/* ========================================================= */
/* ========================================================= */

/**************
 *  
 */
uint8_t BulkStorage_ReadTimeStamp(const DFToken_t *token);

/**************
 *  
 */
uint8_t BulkStorage_VerifyImage(const DFToken_t *token);

/**************
 *  
 */
void BulkStorage_EraseImage(const DFToken_t *token);

/**************
 *  
 */
void BulkStorage_WriteNotes(const DFToken_t *token, const uint8_t *buffer);

/**************
 *  
 */
void BulkStorage_ReadNotes(const DFToken_t *token, uint8_t *buffer);

/* ========================================================= */
/* ========================================================= */
/* ========================================================= */
/**************
 *  
 */
uint8_t BulkStorage_WriteInit(struct BulkStorage *b, const DFToken_t *token);

/**************
 *  
 */
uint16_t BulkStorage_WriteData(struct BulkStorage *b, uint32_t addr, const uint8_t *buffer, uint16_t size);

/**************
 *  
 */
void BulkStorage_WriteClose(struct BulkStorage *b);

/* ========================================================= */
/* ========================================================= */
/* ========================================================= */
/**************
 *  
 */
uint8_t BulkStorage_ReadInit(struct BulkStorage *b, const DFToken_t *token);

/**************
 *  
 */
uint16_t  BulkStorage_ReadData(struct BulkStorage *b, uint32_t addr, uint8_t *buffer, uint16_t size);

/**************
 *  
 */
void BulkStorage_ReadClose(struct BulkStorage *b);

#ifdef __cplusplus
}
#endif

#endif
