
#include "EEPROM.h"
#include "Syslog.h"
#include "Timing.h"
#include "assert.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ======================================= */
const uint16_t INVALID_PAGE_ADDRESS = 0xFFFF;

typedef struct
{
  uint16_t page;
  unsigned dirty:1;
  uint32_t timestamp;
  uint8_t  buf[EE_PAGESIZE];

} EE_CachePage;

static EE_CachePage *ExtCache;
static uint16_t NumberPages;

/* ======================================= */
void EE_Init (uint16_t num_pages)
{
  uint16_t i;

  ExtCache = malloc(sizeof(EE_CachePage) * num_pages);
  assert(ExtCache == NULL);
  if ( ExtCache == NULL ) {
    Syslog(lEmerg, __FILE__, "Failed to allocate cache");
    while (1) ;
  }
  NumberPages = num_pages;
  for ( i = 0; i < num_pages; i ++ ) {
    ExtCache[i].page = INVALID_PAGE_ADDRESS;
    ExtCache[i].timestamp = 0;
    ExtCache[i].dirty = 0;
  }
}

/*************************************
 *
 * ExtCache_Fill()
 * -> page, check to see if this page is currently in the cache
 * Several conditions exist.
 * If pages exists in cache:
 *   Clear count
 *   return ptr to page
 * Page not in cache:
 * Find oldest page.
 * If not dirty,
 *   Read in new page
 *   Clear count
 *   return ptr to page
 * else:
 *   Write old page to SDF BUF2
 *   Read new page into Cache
 *   Write BUF2 to Flash
 *   Clear count
 *   return ptr to page
 */
static EE_CachePage *ExtCache_Fill (uint16_t page)
{
	EE_CachePage *ptr, *oldest = &ExtCache[0];
	uint16_t i, old_page;

	for ( i = 0; i < NumberPages; i++ ) {

		ptr = &ExtCache[i];
		if ( ptr->page == page ) {/* found a match */
			ptr->timestamp = Timing_ReadStamp();
			return ptr;
		}
		if ( ptr->timestamp < oldest->timestamp ) {
			oldest = ptr;
		}
	}

	/* if here then page not found */
	if ( oldest->dirty == 0 ) {
		DataFlash_PageRead(&EE_Token, page, 0, EE_PAGESIZE, oldest->buf);
	} else 	{
		/* We write to buf2
		 * read in new page,
		 * THEN write buffer to flash
		 * this way we avoid the chip busy delay.
		 */
		old_page = oldest->page;
		DataFlash_BufferWrite(&EE_Token, old_page,0,EE_PAGESIZE, DATAFLASH_FLASHBUF2, oldest->buf);	/* write cache to SDF buffer (ignore extra bytes) */

		DataFlash_PageRead(&EE_Token, page, 0, EE_PAGESIZE, oldest->buf);
		DataFlash_WriteBufferToFlash(&EE_Token, old_page, DATAFLASH_FLASHBUF2);	/* commit to SDF */
	}
	oldest->page = page;
	oldest->timestamp = Timing_ReadStamp();
	oldest->dirty = 0;
	return oldest;
}

/* ======================================= */
uint16_t EE_NumberDirtyPages(void)
{
	uint16_t count = 0, i;

	for ( i = 0; i < NumberPages; i ++ ) {
		if ( ExtCache[i].dirty != 0 ) {
			count ++;
		}
	}
	return count;
}

void EE_WriteCachePage(void)
{
	uint16_t i;
	for ( i = 0; i < NumberPages; i ++ ) {
		EE_CachePage *ptr = &ExtCache[i];

		if ( ptr->dirty != 0 ) {
			DataFlash_BufferWrite(&EE_Token, ptr->page, 0,EE_PAGESIZE,DATAFLASH_FLASHBUF2, ptr->buf);
			DataFlash_WriteBufferToFlash(&EE_Token, ptr->page ,DATAFLASH_FLASHBUF2);
			ptr->dirty = 0;
		}
	}
}

/* ************************************************************************* */
/* ========================================================================= */
/* ************************************************************************* */
/* Read functions.  */

uint8_t EE_Read1 (uint32_t addr)
{
	EE_CachePage *ptr = ExtCache_Fill ( addr / EE_PAGESIZE );
	return ptr->buf[addr % EE_PAGESIZE];
}

uint16_t EE_Read2 (uint32_t addr)
{
	return EE_Read1(addr) | ((uint16_t)EE_Read1(addr+1) * 256u);
}

uint32_t EE_Read4 (uint32_t addr)
{
  return EE_Read2(addr) | (uint32_t)EE_Read2(addr+2)*65536u;
}

void EE_Read (uint32_t addr, uint8_t *buf, uint16_t count)
{	/* read a block from EEPROM */
	while ( count-- ) {
		*buf++ = EE_Read1(addr++);
	}
}

uint16_t EE_ReadString (uint32_t addr, char *dest, uint16_t size)
{
	uint16_t length;
	for ( length = 0; length < (size-1); length++ ) {

		char c = EE_Read1(addr++);
		if ( c == 0xff ) {
			break;
		}
		*dest++ = c;
	}
	*dest = 0;
	return length;
}

/* ************************************************************************* */
/* ========================================================================= */
/* ************************************************************************* */
/* Write Functions */

void EE_Write1 (uint32_t addr, uint8_t data)
{	/* write specified byte to external data flash */
	EE_CachePage *ptr = ExtCache_Fill ( addr / EE_PAGESIZE );
	uint8_t *flash_data;

	flash_data = &ptr->buf[addr % EE_PAGESIZE];
	if ( *flash_data != data  ) {

		ptr->dirty = 1;
		*flash_data = data;
	}
}

void EE_Write2 (uint32_t addr, uint16_t data)
{
	EE_Write1(addr,   (uint8_t)data);
	EE_Write1(addr+1, (uint8_t)(data / 256));
}

void EE_Write4 (uint32_t addr, uint32_t data)
{
	EE_Write2(addr,   (uint16_t) data);
	EE_Write2(addr+2, (uint16_t)(data / 65536));
}

void EE_Write (uint32_t addr, const uint8_t *buf, uint16_t count)
{	/* write a block to EEPROM */
	while ( count-- ) {
		EE_Write1(addr++,*buf++);
	}
}

void EE_PutString (uint32_t addr, const char *src, uint16_t size)
{
	if ( (strlen(src)+1) < size ) {
		size = strlen(src)+1;	/* if possible save the terminating null */
	}
	EE_Write(addr,(const uint8_t *)src,size);
}

/* ************************************************************************* */
/* ========================================================================= */
/* ************************************************************************* */
