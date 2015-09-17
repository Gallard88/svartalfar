
#include "BulkStorage.h"
#include "ErrorChecking.h"


#include <stdio.h>
#include <string.h>

/*  Implementation details:
 *  Each page has a field stating how much data is stored.
 *  Should be DATAFLASH_PAGESIZE_NORMAL, except potentially 
 * for the last page.
 * 
 * Error detection.
 * Each page has a checksum. This is simply the sum of *all* bytes in that page.
 * The header contains a crude hash to verify the entire image.
 * This is the checksum of each page containing data XOR'd together.
 * It aims to be quick to detect major problems, i.e. entire pages corrupted,
 * rather than subtle bit wise errors. It is expected that the stored data will
 * contain it's own error checking mechanism.
 *
 * Header.
 * The first page contain
 */
/* ========================================================= */
/* ========================================================= */
struct PageBuf
{
  uint8_t buf[DATAFLASH_PAGESIZE_NORMAL];
  uint32_t hash;
};

/* ========================================================= */
/* Static Functions */
static void ReadHeader(const DFToken_t *token, struct BulkStorage_Header* h)
{
  DataFlash_PageRead(token, 0, 0, sizeof(struct BulkStorage_Header), (uint8_t*)h);
}

static void WriteHeader(const DFToken_t *token, struct BulkStorage_Header* h)
{
  DataFlash_BufferWrite(token, 0, 0, sizeof(struct BulkStorage_Header), DATAFLASH_FLASHBUF1, (uint8_t*)h); 
  DataFlash_WriteBufferToFlash(token, 0, DATAFLASH_FLASHBUF1);
}

static void ReadPage(struct BulkStorage *b,  uint16_t page)
{
  DataFlash_PageRead(b->token, page, 0, DATAFLASH_PAGESIZE_NORMAL, b->data);
  b->loaded_page = page;
}

static void WritePage(struct BulkStorage *b)
{
  struct PageBuf p;
  memcpy(p.buf, b->data, DATAFLASH_PAGESIZE_NORMAL);
  p.hash = CalcCRC(p.buf, DATAFLASH_PAGESIZE_NORMAL);
  DataFlash_BufferWrite(b->token, b->loaded_page, 0, sizeof(struct PageBuf), DATAFLASH_FLASHBUF1, (uint8_t*) &p); 
  DataFlash_WriteBufferToFlash(b->token, b->loaded_page, DATAFLASH_FLASHBUF1);
}

static uint16_t CalcPage(uint32_t addr)
{
  return (addr / DATAFLASH_PAGESIZE_NORMAL) + 1;
}

static uint16_t CalcPageAddr(uint32_t addr)
{
  return addr % DATAFLASH_PAGESIZE_NORMAL;
}

/* ========================================================= */
/* ========================================================= */
/* ========================================================= */
uint8_t BulkStorage_ReadTimeStamp(const DFToken_t *token)
{
  struct BulkStorage_Header h;
  ReadHeader(token, &h);
  return h.timestamp;
}

uint8_t BulkStorage_VerifyImage(const DFToken_t *token)
{
  struct PageBuf p;
  uint32_t i;
  uint32_t page_hash;
  uint32_t image_hash = 0;

  struct BulkStorage_Header h;
  ReadHeader(token, &h);
  
  for ( i = 1; i < token->number_pages; i ++ ) {
    DataFlash_PageRead(token, i, 0, sizeof(struct PageBuf), (uint8_t*)&p);
    page_hash = CalcCRC(p.buf, DATAFLASH_PAGESIZE_NORMAL);   
    if ( page_hash != p.hash ) {
      return 0;
    }
    image_hash ^= page_hash;   
  }
  return ( image_hash == h.hash )? 1: 0;
}

void BulkStorage_EraseImage(const DFToken_t *token)
{
  uint32_t i;

  uint8_t buf[DATAFLASH_PAGESIZE_EXT];
  memset(buf, 0xFF, DATAFLASH_PAGESIZE_EXT);

  for ( i = 0; i < token->number_pages; i ++ ) {
    DataFlash_BufferWrite(token, i, 0, DATAFLASH_PAGESIZE_EXT, DATAFLASH_FLASHBUF1, buf); 
    DataFlash_WriteBufferToFlash(token, i, DATAFLASH_FLASHBUF1);
  }
}

void BulkStorage_WriteNotes(const DFToken_t *token, const uint8_t *buffer)
{
  if ( buffer == NULL ) {
    return;
  }
  struct BulkStorage_Header h;
  ReadHeader(token, &h);
  memcpy(h.notes, buffer, BULK_NOTE_SIZE);
  WriteHeader(token, &h);
}

void BulkStorage_ReadNotes(const DFToken_t *token, uint8_t *buffer)
{
  if ( buffer == NULL ) {
    return;
  }
  struct BulkStorage_Header h;
  ReadHeader(token, &h);
  memcpy(buffer, h.notes, BULK_NOTE_SIZE);
  WriteHeader(token, &h);
}

/* ========================================================= */
/* ========================================================= */
/* ========================================================= */
uint8_t BulkStorage_WriteInit(struct BulkStorage *b, const DFToken_t *token)
{
  if (( b == NULL ) || ( token == NULL )) {
    return 1;
  }
  b->token = token;
  ReadHeader(token, &b->head);
  b->loaded_page = 0xFFFF;
  b->dirty = 0;
  b->marked = 0;
  b->write = 1;
  return 0;
}

uint16_t BulkStorage_WriteData(struct BulkStorage *b, uint32_t addr, const uint8_t *buffer, uint16_t size)
{
  if (( b == NULL ) || 
      ( buffer == NULL ) ||
      ( b->write == 0 )) {
    return 0;
  }  
  uint16_t page = CalcPage(addr);
  if ( page != b->loaded_page ) {
    if ( b->dirty != 0 ) {
      WritePage(b);
      b->dirty = 0;
    }
    ReadPage(b, page);
  }
  uint16_t offset = CalcPageAddr(addr);
  uint16_t length = size;
  if (( offset+size) > DATAFLASH_PAGESIZE_NORMAL ) {
    length = DATAFLASH_PAGESIZE_NORMAL - offset;
  }
  if ( memcmp(b->data+offset, buffer, length) != 0 ) {
    b->dirty = 1; /* only mark as dirty if we actually change data... */
    b->marked = 1;  
    memcpy(b->data+offset, buffer, length); 
  }
  return length;
}

void BulkStorage_WriteClose(struct BulkStorage *b)
{
  if ( b == NULL ) {
    return;
  }
  if ( b->dirty != 0 ) {
    WritePage(b);
  }
  if ( b->marked != 0 ) {
    uint32_t image_hash = 0;
    uint32_t page_hash ;
    for ( uint32_t i = 1; i < b->token->number_pages ; i ++ ) {
      ReadPage(b,  i);
      page_hash = CalcCRC(b->data, DATAFLASH_PAGESIZE_NORMAL);
      image_hash ^= page_hash;      
    }
    b->head.hash = image_hash;
    b->head.timestamp = 0;
    b->marked = 0;
    WriteHeader(b->token, &b->head);
  }
  
  b->token = NULL;
  b->loaded_page = 0xFFFF;
  b->dirty = 0;
  b->write = 0;
}

/* ========================================================= */
/* ========================================================= */
/* ========================================================= */
uint8_t BulkStorage_ReadInit(struct BulkStorage *b, const DFToken_t *token)
{
  if (( b == NULL ) || ( token == NULL )) {
    return 1;
  }
  b->token = token;
  ReadHeader(token, &b->head);
  b->loaded_page = 0xFFFF;
  b->dirty = 0;
  b->write = 0;
  b->marked = 0;
  return 0;
}

uint16_t  BulkStorage_ReadData(struct BulkStorage *b, uint32_t addr, uint8_t *buffer, uint16_t size)
{
  if (( b == NULL ) || ( buffer == NULL )) {
    return 0;
  }  
  uint16_t page = CalcPage(addr);
  if ( page != b->loaded_page ) {
    if ( b->dirty != 0 ) {
      WritePage(b);
      b->dirty = 0;
    }
    ReadPage(b, page);
  }
  uint16_t offset = CalcPageAddr(addr);
  uint16_t length = size;
  if (( offset+size) > DATAFLASH_PAGESIZE_NORMAL ) {
    length = DATAFLASH_PAGESIZE_NORMAL - offset;
  }
  memcpy(buffer, b->data+offset, length); 
  return length;
}

void BulkStorage_ReadClose(struct BulkStorage *b)
{
  if ( b == NULL ) {
    return;
  }
  b->token = NULL;
  b->loaded_page = 0xFFFF;
  b->dirty = 0;
  b->write = 0;
}

/* ========================================================= */
/* ========================================================= */
/* ========================================================= */
