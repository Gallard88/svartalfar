
#include <stdio.h>

#include "Dataflash.h"

/* defines for all opcodes    */
#define BUFFER_1_WRITE 0x84			/* buffer 1 write */
#define BUFFER_2_WRITE 0x87			/* buffer 2 write */

#define BUFFER_1_READ 0xD4			/* buffer 1 read  */
#define BUFFER_2_READ 0xD6			/* buffer 2 read  */

/* buffer 1 to main memory page program with built-in erase */
#define B1PROGWITHERASE 0x83
/* buffer 2 to main memory page program with built-in erase */
#define B2PROGWITHERASE 0x86

/* buffer 1 to main memory page program without built-in erase  */
#define B1PROGNOERASE 0x88
/* buffer 2 to main memory page program without built-in erase  */
#define B2PROGNOERASE 0x89

/* main memory page program through buffer 1  */
#define MM_PAGE_PROG_THROUGH_B1 0x82
/* main memory page program through buffer 2  */
#define MM_PAGE_PROG_THROUGH_B2 0x85

/* auto page rewrite through buffer 1   */
#define AUTO_PAGE_REWRITE_THROUGH_B1 0x58
/* auto page rewrite through buffer 2   */
#define AUTO_PAGE_REWRITE_THROUGH_B2 0x59

/* main memory page compare to buffer 1 */
#define MM_PAGE_TO_B1_COMP 0x60
/* main memory page compare to buffer 2 */
#define MM_PAGE_TO_B2_COMP 0x61

/* main memory page to buffer 1 transfer  */
#define MM_PAGE_TO_B1_XFER 0x53
/* main memory page to buffer 2 transfer  */
#define MM_PAGE_TO_B2_XFER 0x55

/* DataFlash status register for reading density, compare status,   */
/* and ready/busy status                                            */
#define STATUS_REGISTER 0xD7


#define MAIN_MEMORY_PAGE_READ 0xD2 /* main memory page read  */
#define PAGE_ERASE 0x81            /* erase a page */
#define BLOCK_ERASE 0x50           /* erase a block of pages */
#define DEVICE_ID_READ		0x9F


/* ======================================= */
/*static const uint32_t FLASHPAGESPERCHIP	= 8192;*/

/* ======================================= */
void DataFlash_Init(void)
{
  Dataflash_SPI.InitCsPin();
  Dataflash_SPI.CsOff();
}

/* ======================================= */
static uint8_t IsFlashReady (uint16_t page)
{	/* use page to determine which flash chip to test */
	register uint8_t status;

	Dataflash_SPI.CsOn();
	Dataflash_SPI.Write(STATUS_REGISTER);
	Dataflash_SPI.Write(0xFF);
	status = Dataflash_SPI.Read();
	Dataflash_SPI.CsOff();

	return (status & (1<<7));	/* test the READY bit */
}

uint16_t DataFlash_CheckToken(const DFToken_t *token, uint16_t page)
{
	if ( page < token->number_pages )
	{
		page = token->start_page + page;
		while ( !IsFlashReady(page) )
			;
		return page;	/* translate to real page number  */
	}
	return 0xFFFF;
}

/* ======================================= */
DF_Return_t DataFlash_PageRead(const DFToken_t *token, uint16_t page, uint16_t offset, uint16_t size, uint8_t *dest)
{	/* Main Memory Page Read (does not use either buffer)
	 * 24 address bits, 4 don't care bytes
	 * address = 13 bits page, 10bits byte
	 * 0ppp pppp pppp ppbb bbbb bbbb
   */
  if ( token == NULL ) {
    return DF_NoToken;
  }    

	page = DataFlash_CheckToken(token, page);
	if ( page == 0xFFFF ) {
		return DF_BadPage;
	}

	Dataflash_SPI.CsOn();
	Dataflash_SPI.Write(MAIN_MEMORY_PAGE_READ);
	Dataflash_SPI.Write(page >> 6);
	Dataflash_SPI.Write((page << 2) | (offset >> 8));
	Dataflash_SPI.Write((uint8_t)offset);

 	/* don't care bytes */
	Dataflash_SPI.Write(0);
	Dataflash_SPI.Write(0);
	Dataflash_SPI.Write(0);
	Dataflash_SPI.Write(0);
	/* ready to start reading */
	Dataflash_SPI.ReadWrite( dest, size);

	Dataflash_SPI.CsOff();
  return DF_Good;
}

/* ======================================= */
DF_Return_t DataFlash_ReadFlashToBuffer(const DFToken_t *token, uint16_t page, uint8_t whichbuf)
{	/* do a Main Memory Page Read to Buffer
	 * 24 address bits, 32 don't care bits
	 * address = 1bits 0, 13bits page, 10bits 0
	 * 0ppp pppp pppp pp00 0000 0000
   */
  if ( token == NULL ) {
    return DF_NoToken;
  }    

	page = DataFlash_CheckToken(token, page);
	if ( page == 0xFFFF ) {
		return DF_BadPage;
	}

	Dataflash_SPI.CsOn();			// delay 250ns minimum till SCK (1.8T)
	Dataflash_SPI.Write(whichbuf==DATAFLASH_FLASHBUF1?MM_PAGE_TO_B1_XFER:MM_PAGE_TO_B2_XFER);	// Main Memory Page Read to Buffer 1 or 2
	Dataflash_SPI.Write(page>>6);
	Dataflash_SPI.Write((page<<2)+0);
	Dataflash_SPI.Write(0);
	Dataflash_SPI.CsOff();
  return DF_Good;
}

DF_Return_t DataFlash_WriteBufferToFlash(const DFToken_t *token, uint16_t page, uint8_t whichbuf)
{	/* do a Buffer to Main Memory Page Write with auto-erase
	 * 24 address bits, 32 don't care bits
	 * address = 2bits 0, 13bits page, 10bits 0
	 * 0ppp pppp pppp pp00 0000 0000
   */
  if ( token == NULL ) {
    return DF_NoToken;
  }    

	page = DataFlash_CheckToken(token, page);
	if ( page == 0xFFFF ) {
		return DF_BadPage;
	}

	Dataflash_SPI.CsOn();

	Dataflash_SPI.Write(whichbuf==DATAFLASH_FLASHBUF1?B1PROGWITHERASE:B2PROGWITHERASE);
	Dataflash_SPI.Write(page>>6);
	Dataflash_SPI.Write((page<<2)+0);
	Dataflash_SPI.Write(0);
	Dataflash_SPI.CsOff();
  return DF_Good;
}

/* ======================================= */
DF_Return_t DataFlash_BufferRead (const DFToken_t *token, uint16_t page, uint16_t offset, uint16_t size, uint8_t whichbuf, uint8_t *dest)
{	/* start a Read from Buffer
	 * 24 address bits, 8 don't care bits
	 * address = 14 bits 0, 10bits byte
	 * 0000 0000 0000 00bb bbbb bbbb
   */
  if ( token == NULL ) {
    return DF_NoToken;
  }    
  if ( dest == NULL ) {
    return DF_NoBuffer;
  }    
	page = DataFlash_CheckToken(token, page);
	if ( page == 0xFFFF ) {
		return DF_BadPage;
	}

	Dataflash_SPI.CsOn();
	Dataflash_SPI.Write(whichbuf==DATAFLASH_FLASHBUF1?BUFFER_1_READ:BUFFER_2_READ);
	Dataflash_SPI.Write(0);
	Dataflash_SPI.Write(offset>>8);
	Dataflash_SPI.Write((uint8_t)offset);
	Dataflash_SPI.Write(0);
	Dataflash_SPI.ReadWrite( dest, size);
	Dataflash_SPI.CsOff();
  return DF_Good;
}

DF_Return_t DataFlash_BufferWrite (const DFToken_t *token, uint16_t page, uint16_t offset, uint16_t size, uint8_t whichbuf, uint8_t *src)
{	/* do a Write to Buffer
   * 24 address bits, 8 don't care bits
   * address = 14 bits 0, 10 bits byte
   * 0000 0000 0000 00bb bbbb bbbb
   */
  if ( token == NULL ) {
    return DF_NoToken;
  }    
  if ( src == NULL ) {
    return DF_NoBuffer;
  }    
	page = DataFlash_CheckToken(token, page);
	if ( page == 0xFFFF ) {
		return DF_BadPage;
	}

	Dataflash_SPI.CsOn();
	Dataflash_SPI.Write(whichbuf==DATAFLASH_FLASHBUF1?BUFFER_1_WRITE:BUFFER_2_WRITE);
	Dataflash_SPI.Write(0);
	Dataflash_SPI.Write(offset>>8);
	Dataflash_SPI.Write((uint8_t)offset);
	Dataflash_SPI.ReadWrite( src, size);
	Dataflash_SPI.CsOff();
  return DF_Good;
}

/* ======================================= */
DataFlash_Chip_t DataFlash_ReadChipType(void)
{
	uint8_t data;

	while ( !IsFlashReady(0) )
		;
	Dataflash_SPI.CsOn();
	Dataflash_SPI.Write(DEVICE_ID_READ);
	Dataflash_SPI.Write(0);
	Dataflash_SPI.Read();
	Dataflash_SPI.Write(0);
	data = Dataflash_SPI.Read();
	Dataflash_SPI.Write(0);
	Dataflash_SPI.Read();
	Dataflash_SPI.Write(0);
	Dataflash_SPI.Read();
	Dataflash_SPI.CsOff();

  if ( data == 0x27 ) {
    return FLASH_DEVID_321;

  } else if ( data == 0x26 ) {
    return FLASH_DEVID_161;

  } else if ( data == 0x25 ) {
    return FLASH_DEVID_081;

  } else {
    return FLASH_DEVID_UNKNOWN;
  }
}

