#include "unity_fixture.h"

#include "Dataflash.h"
/*** 
 * Here we remap the data flash functions.
 * This makes it much easier to verify that the 
 * module is doing what it supposed to.
 */
#define DataFlash_PageRead           TEST_PageRead
#define DataFlash_WriteBufferToFlash TEST_WriteBufferToFlash
#define DataFlash_BufferWrite        TEST_BufferWrite

DF_Return_t TEST_PageRead(const DFToken_t *token, uint16_t page, uint16_t offset, uint16_t size, uint8_t *dest);
DF_Return_t TEST_WriteBufferToFlash(const DFToken_t *token, uint16_t page, uint8_t whichbuf);
DF_Return_t TEST_BufferWrite (const DFToken_t *token, uint16_t page, uint16_t offset, uint16_t size, uint8_t whichbuf, uint8_t *src);


#include "../BulkStorage.h"
#include "../BulkStorage.c"

TEST_GROUP(TestBulkStorage);

#define NUM_PAGES       10
#define UNWRITTEN_VALUE 0xAA
#define UNDERWRITE_SIZE 8

const DFToken_t BS_Token = {
  .start_page = 0,
  .number_pages = NUM_PAGES
};

struct DF_Page {
  uint8_t dirty;
  uint8_t data[DATAFLASH_PAGESIZE_EXT+UNDERWRITE_SIZE+UNDERWRITE_SIZE];
};

struct DF_Page PageArray[NUM_PAGES];
struct BulkStorage BulkWrite;
struct BulkStorage BulkRead;

struct {
  unsigned invalid_offset:1;
  unsigned invalid_page:1;
} ErrorBits;


TEST_SETUP(TestBulkStorage)
{
  for ( int i = 0; i < NUM_PAGES; i ++ ) {     
    memset(PageArray[i].data, UNWRITTEN_VALUE, (DATAFLASH_PAGESIZE_EXT+16));
    PageArray[i].dirty = 0;
  }
  BulkStorage_WriteInit(&BulkWrite, &BS_Token);
  BulkStorage_ReadInit (&BulkRead,  &BS_Token);
  
  memset(PageArray[0].data+8, UNWRITTEN_VALUE, BULK_NOTE_SIZE);
  PageArray[0].data[UNDERWRITE_SIZE+0] = 0x0F;
  PageArray[0].data[UNDERWRITE_SIZE+1] = 0x0F;
  PageArray[0].data[UNDERWRITE_SIZE+2] = 0x0F;
  PageArray[0].data[UNDERWRITE_SIZE+3] = 0x0F;
  PageArray[0].data[UNDERWRITE_SIZE+4] = 0xF0;
  PageArray[0].data[UNDERWRITE_SIZE+5] = 0xF0;
  PageArray[0].data[UNDERWRITE_SIZE+6] = 0xF0;
  PageArray[0].data[UNDERWRITE_SIZE+7] = 0xF0;
  memset(&ErrorBits, 0, sizeof(ErrorBits));
}

TEST_TEAR_DOWN(TestBulkStorage)
{
  TEST_ASSERT_EQUAL_INT_MESSAGE( 0, ErrorBits.invalid_offset, "Invalid Offset" );
  TEST_ASSERT_EQUAL_INT_MESSAGE( 0, ErrorBits.invalid_page, "Invalid Page" );
  BulkStorage_WriteClose(&BulkWrite);
  BulkStorage_ReadClose(&BulkRead);
}

static int CheckPage(int page)
{
  for ( int i = 0; i < (DATAFLASH_PAGESIZE_EXT+16); i ++ ) {     
    if ( PageArray[page].data[i] != UNWRITTEN_VALUE) {
      return 1;
    }
  }  
  return 0;
}

static int CheckAllPages(void)
{
  for ( int i = 1; i < NUM_PAGES; i ++ ) {   
    if ( CheckPage(i) != 0 )
      return 1;
  }
  return 0;
}

/* ======================== */
TEST(TestBulkStorage, BulkStorage_WriteNotes_NoPtr)
{
  uint8_t buffer[BULK_NOTE_SIZE];
  memset(buffer, UNWRITTEN_VALUE, sizeof(buffer));

  BulkStorage_WriteNotes(&BS_Token, NULL);
  TEST_ASSERT_EQUAL_INT8_ARRAY(buffer, PageArray[0].data+8+UNDERWRITE_SIZE, BULK_NOTE_SIZE);
}

TEST(TestBulkStorage, BulkStorage_WriteNotes_ClearData)
{
  uint8_t expected[BULK_NOTE_SIZE] = { 0 };
  uint8_t buffer[BULK_NOTE_SIZE];

  memset(buffer, 0, sizeof(buffer));
  BulkStorage_WriteNotes(&BS_Token, buffer);
  
  TEST_ASSERT_EQUAL_INT8_ARRAY(expected, PageArray[0].data+8+UNDERWRITE_SIZE, BULK_NOTE_SIZE);
}

TEST(TestBulkStorage, BulkStorage_WriteNotes_CheckBounds_Under)
{
  uint8_t empty[8] = { 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0 };
  uint8_t buffer[BULK_NOTE_SIZE];
  memset(buffer, 0, sizeof(buffer));
  BulkStorage_WriteNotes(&BS_Token, buffer);
  TEST_ASSERT_EQUAL_INT8_ARRAY(empty, PageArray[0].data+UNDERWRITE_SIZE, 8);
}

TEST(TestBulkStorage, BulkStorage_WriteNotes_CheckBounds_Over)
{
  uint8_t empty[8];
  uint8_t buffer[BULK_NOTE_SIZE];
  memset(empty, UNWRITTEN_VALUE, sizeof(empty));
  memset(buffer, 0, sizeof(buffer));
  BulkStorage_WriteNotes(&BS_Token, buffer);
  TEST_ASSERT_EQUAL_INT8_ARRAY(empty, PageArray[0].data+8+BULK_NOTE_SIZE+UNDERWRITE_SIZE, 8);
}

TEST(TestBulkStorage, BulkStorage_ReadNotes_Good)
{
  uint8_t exp[BULK_NOTE_SIZE];
  uint8_t buffer[BULK_NOTE_SIZE];
  
  memset(exp, 0x55, sizeof(exp));
  memset(buffer, 0x55, sizeof(buffer));
  BulkStorage_WriteNotes(&BS_Token, buffer);
  BulkStorage_ReadNotes(&BS_Token, buffer);
  TEST_ASSERT_EQUAL_INT8_ARRAY(exp, buffer, BULK_NOTE_SIZE);
}

/* ======================== */
/*
uint8_t BulkStorage_WriteInit(struct BulkStorage *b, const DFToken_t *token);
*/
/* ======================== */
TEST(TestBulkStorage, BulkStorage_WriteInit_NoPtr)
{
	TEST_ASSERT_EQUAL(1, BulkStorage_WriteInit(NULL, &BS_Token ));
}

TEST(TestBulkStorage, BulkStorage_WriteInit_NoToken)
{
  struct BulkStorage b;
	TEST_ASSERT_EQUAL(1, BulkStorage_WriteInit(&b, NULL ));
}

TEST(TestBulkStorage, BulkStorage_WriteInit_Good)
{
  struct BulkStorage b;
	TEST_ASSERT_EQUAL(0, BulkStorage_WriteInit(&b, &BS_Token ));
}

TEST(TestBulkStorage, BulkStorage_WriteInit_CheckHeader)
{
  struct BulkStorage b;
  BulkStorage_WriteInit(&b, &BS_Token );
  TEST_ASSERT_EQUAL(0x0F0F0F0F, b.head.hash);
  TEST_ASSERT_EQUAL(0xF0F0F0F0, b.head.timestamp);
}

TEST(TestBulkStorage, BulkStorage_WriteInit_CheckFlags)
{
  struct BulkStorage b;
  BulkStorage_WriteInit(&b, &BS_Token );
  TEST_ASSERT_EQUAL(0xFFFF, b.loaded_page);
  TEST_ASSERT_EQUAL(0, b.dirty);
  TEST_ASSERT_EQUAL(1, b.write);
}

TEST(TestBulkStorage, BulkStorage_WriteInit_CheckToken)
{
  struct BulkStorage b;
  BulkStorage_WriteInit(&b, &BS_Token );
  TEST_ASSERT_NOT_EQUAL(0, b.token);
}

/* ======================== */
/*
uint16_t BulkStorage_WriteData(struct BulkStorage *b, uint32_t addr, const uint8_t *buffer, uint16_t size);
*/
TEST(TestBulkStorage, BulkStorage_WriteData_WriteNothing_Return)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0x55, sizeof(buffer));
  
  uint16_t rv = BulkStorage_WriteData( &BulkWrite, 0, buffer, 0);
	TEST_ASSERT_EQUAL(0, rv);
}

TEST(TestBulkStorage, BulkStorage_WriteData_WriteNothing_PagesClear)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0x55, sizeof(buffer));
  
  BulkStorage_WriteData( &BulkWrite, 0, buffer, 0);
	TEST_ASSERT_EQUAL(0, CheckAllPages());
}

TEST(TestBulkStorage, BulkStorage_WriteData_WritePages_CheckP0)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0x0F, sizeof(buffer));
  
  BulkStorage_WriteData( &BulkWrite, 0, buffer, DATAFLASH_PAGESIZE_NORMAL);
  BulkStorage_WriteData( &BulkWrite, DATAFLASH_PAGESIZE_NORMAL, buffer, DATAFLASH_PAGESIZE_NORMAL);
	TEST_ASSERT_EQUAL(1, CheckPage(1));
}

TEST(TestBulkStorage, BulkStorage_WriteData_WritePages_CheckP2)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0x55, sizeof(buffer));
  
  BulkStorage_WriteData( &BulkWrite, 0, buffer, DATAFLASH_PAGESIZE_NORMAL);
	TEST_ASSERT_EQUAL(0, CheckPage(2));
}

TEST(TestBulkStorage, BulkStorage_WriteData_WritePages_CheckValid)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0x55, sizeof(buffer));
  
  uint8_t expected[DATAFLASH_PAGESIZE_NORMAL];
  memset(expected, 0x55, sizeof(expected));
  
  BulkStorage_WriteData( &BulkWrite, 0, buffer, DATAFLASH_PAGESIZE_NORMAL);
  TEST_ASSERT_EQUAL_INT8_ARRAY(expected, BulkWrite.data, sizeof(expected)); 
}

TEST(TestBulkStorage, BulkStorage_WriteData_WriteFractionalPage_CheckValid)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0x55, sizeof(buffer));
  
  uint8_t expected[DATAFLASH_PAGESIZE_NORMAL];
  memset(expected, UNWRITTEN_VALUE, sizeof(expected));
  memset(expected+256, 0x55, 256);
  
  BulkStorage_WriteData( &BulkWrite, 256, buffer, 256);
  TEST_ASSERT_EQUAL_INT8_ARRAY(expected, BulkWrite.data, sizeof(expected));
}

TEST(TestBulkStorage, BulkStorage_WriteData_CheckDirtyFlag)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0x55, sizeof(buffer));
  
  BulkStorage_WriteData( &BulkWrite, 256, buffer, 256);
  
  TEST_ASSERT_EQUAL(1, BulkWrite.dirty);
}

TEST(TestBulkStorage, BulkStorage_WriteData_WriteFullPage_CheckReturn)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0x55, sizeof(buffer));
  
  uint16_t rv = BulkStorage_WriteData( &BulkWrite, 0, buffer, DATAFLASH_PAGESIZE_NORMAL);
  TEST_ASSERT_EQUAL_UINT16(DATAFLASH_PAGESIZE_NORMAL, rv); 
}

TEST(TestBulkStorage, BulkStorage_WriteData_WriteGreaterDataThanPageSize_CheckReturn)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0x55, sizeof(buffer));
  
  uint16_t rv = BulkStorage_WriteData( &BulkWrite, 256, buffer, DATAFLASH_PAGESIZE_NORMAL);
  TEST_ASSERT_EQUAL_UINT16((DATAFLASH_PAGESIZE_NORMAL-256), rv); 
}

/* ======================== */
/*
 * Now we check what happens when we flush a page. 
 */
TEST(TestBulkStorage, BulkStorage_WriteData_WriteNextPage_NeighbouringPages)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0x55, sizeof(buffer));
  
  uint8_t expected[DATAFLASH_PAGESIZE_EXT+16];
  memset(expected, UNWRITTEN_VALUE, sizeof(expected));
  memset(expected+8, 0x55, DATAFLASH_PAGESIZE_NORMAL);
  
  BulkStorage_WriteData( &BulkWrite, 0, buffer, DATAFLASH_PAGESIZE_NORMAL);
  BulkStorage_WriteData( &BulkWrite, DATAFLASH_PAGESIZE_NORMAL, buffer, DATAFLASH_PAGESIZE_NORMAL);
	TEST_ASSERT_EQUAL(0, CheckPage(2));
}

TEST(TestBulkStorage, BulkStorage_WriteData_WriteNextPage)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0x55, sizeof(buffer));
  
  uint8_t expected[DATAFLASH_PAGESIZE_EXT+16];
  memset(expected, UNWRITTEN_VALUE, sizeof(expected));
  memset(expected+8, 0x55, DATAFLASH_PAGESIZE_NORMAL);
  
  BulkStorage_WriteData( &BulkWrite, 0, buffer, DATAFLASH_PAGESIZE_NORMAL);
  BulkStorage_WriteData( &BulkWrite, DATAFLASH_PAGESIZE_NORMAL, buffer, DATAFLASH_PAGESIZE_NORMAL);

  TEST_ASSERT_EQUAL_INT8_ARRAY(expected, PageArray[1].data, DATAFLASH_PAGESIZE_NORMAL+8);
}

TEST(TestBulkStorage, BulkStorage_WriteData_NoPermission_RV)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0x55, sizeof(buffer));
  
  uint16_t rv = BulkStorage_WriteData( &BulkRead, 0, buffer, DATAFLASH_PAGESIZE_NORMAL);
	TEST_ASSERT_EQUAL(0, rv);
}

TEST(TestBulkStorage, BulkStorage_WriteData_NoPermission_Data)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0x55, sizeof(buffer));
  
  uint8_t expected[DATAFLASH_PAGESIZE_EXT];
  memset(expected, UNWRITTEN_VALUE, DATAFLASH_PAGESIZE_NORMAL);
  
  BulkStorage_WriteData( &BulkRead, 0, buffer, DATAFLASH_PAGESIZE_NORMAL);
  TEST_ASSERT_EQUAL_INT8_ARRAY(expected, PageArray[1].data+UNDERWRITE_SIZE, DATAFLASH_PAGESIZE_NORMAL);
}

/* ======================== */
/*
void BulkStorage_WriteClose(struct BulkStorage *b);
*/
TEST(TestBulkStorage, BulkStorage_WriteClose_FlushPageFlag)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0x55, sizeof(buffer));
  
  BulkStorage_WriteData( &BulkWrite, 0, buffer, DATAFLASH_PAGESIZE_NORMAL);
  BulkStorage_WriteClose(&BulkWrite);
  TEST_ASSERT_EQUAL(0, BulkWrite.dirty);
}

TEST(TestBulkStorage, BulkStorage_WriteClose_FlushPageData)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0x55, sizeof(buffer));
  uint8_t expected[DATAFLASH_PAGESIZE_EXT+16];
  memset(expected, UNWRITTEN_VALUE, sizeof(expected));
  memset(expected+8, 0x55, DATAFLASH_PAGESIZE_NORMAL);
  
  BulkStorage_WriteData( &BulkWrite, 0, buffer, DATAFLASH_PAGESIZE_NORMAL);
  BulkStorage_WriteClose(&BulkWrite);
  TEST_ASSERT_EQUAL_INT8_ARRAY(expected, PageArray[1].data, DATAFLASH_PAGESIZE_NORMAL+8);
}

TEST(TestBulkStorage, BulkStorage_WriteClose_ResetPageVariable)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0x55, sizeof(buffer));
  
  BulkStorage_WriteData( &BulkWrite, 0, buffer, DATAFLASH_PAGESIZE_NORMAL);
  BulkStorage_WriteClose(&BulkWrite);
  TEST_ASSERT_EQUAL(0xFFFF, BulkWrite.loaded_page);
}

TEST(TestBulkStorage, BulkStorage_WriteClose_Timestamp)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0x55, sizeof(buffer));
  
  BulkStorage_WriteData( &BulkWrite, 0, buffer, DATAFLASH_PAGESIZE_NORMAL);
  BulkStorage_WriteClose(&BulkWrite);
  TEST_ASSERT_NOT_EQUAL(0xF0F0F0F0, BulkWrite.head.timestamp);
}

TEST(TestBulkStorage, BulkStorage_WriteClose_Hash)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0x55, sizeof(buffer));
  
  BulkStorage_WriteData( &BulkWrite, 0, buffer, DATAFLASH_PAGESIZE_NORMAL);
  BulkStorage_WriteClose(&BulkWrite);
  TEST_ASSERT_NOT_EQUAL(0x0F0F0F0F, BulkWrite.head.hash);
}


/* ======================== */
/* ======================== */
TEST(TestBulkStorage, BulkStorage_ReadInit_NoPtr)
{
	TEST_ASSERT_EQUAL(1, BulkStorage_ReadInit(NULL, &BS_Token ));
}

TEST(TestBulkStorage, BulkStorage_ReadInit_NoToken)
{
  struct BulkStorage b;
	TEST_ASSERT_EQUAL(1, BulkStorage_ReadInit(&b, NULL ));
}

TEST(TestBulkStorage, BulkStorage_ReadInit_Good)
{
  struct BulkStorage b;
	TEST_ASSERT_EQUAL(0, BulkStorage_ReadInit(&b, &BS_Token ));
}

TEST(TestBulkStorage, BulkStorage_ReadInit_CheckHeader)
{
  struct BulkStorage b;
  BulkStorage_ReadInit(&b, &BS_Token );
  TEST_ASSERT_EQUAL(0x0F0F0F0F, b.head.hash);
  TEST_ASSERT_EQUAL(0xF0F0F0F0, b.head.timestamp);
}

TEST(TestBulkStorage, BulkStorage_ReadInit_CheckFlags)
{
  struct BulkStorage b;
  BulkStorage_ReadInit(&b, &BS_Token );
  TEST_ASSERT_EQUAL(0xFFFF, b.loaded_page);
  TEST_ASSERT_EQUAL(0, b.dirty);
  TEST_ASSERT_EQUAL(0, b.write);
}

TEST(TestBulkStorage, BulkStorage_ReadInit_CheckToken)
{
  struct BulkStorage b;
  BulkStorage_ReadInit(&b, &BS_Token );
  TEST_ASSERT_NOT_EQUAL(0, b.token);
}

/* ======================== */
/*
uint16_t  BulkStorage_ReadData(struct BulkStorage *b, uint32_t addr, uint8_t *buffer, uint16_t size)
*/
TEST(TestBulkStorage, BulkStorage_ReadData_ReadNothing_RV)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  
  uint16_t rv = BulkStorage_ReadData( &BulkRead, 0, buffer, 0);
	TEST_ASSERT_EQUAL(0, rv);
}

TEST(TestBulkStorage, BulkStorage_ReadData_ReadNothing_Buff)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  uint8_t expected[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0, sizeof(expected));
  memset(expected, 0, sizeof(expected));
  
  BulkStorage_ReadData( &BulkRead, 0, buffer, 0);
  TEST_ASSERT_EQUAL_INT8_ARRAY(expected, buffer, DATAFLASH_PAGESIZE_NORMAL);
}

TEST(TestBulkStorage, BulkStorage_ReadData_ReadData_RV)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  
  uint16_t rv = BulkStorage_ReadData( &BulkRead, 0, buffer, 128);
	TEST_ASSERT_EQUAL(128, rv);
}

TEST(TestBulkStorage, BulkStorage_ReadData_ReadData_Buff)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  uint8_t expected[DATAFLASH_PAGESIZE_NORMAL];
  memset(expected, UNWRITTEN_VALUE, sizeof(expected));
  
  BulkStorage_ReadData( &BulkRead, 0, buffer, 128);
  TEST_ASSERT_EQUAL_INT8_ARRAY(expected, buffer, 128);
}

TEST(TestBulkStorage, BulkStorage_ReadData_ReadData_CheckPage_Addr0)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  BulkStorage_ReadData( &BulkRead, 0, buffer, 128);
  TEST_ASSERT_EQUAL(1, BulkRead.loaded_page);
}

TEST(TestBulkStorage, BulkStorage_ReadData_ReadData_CheckPage_Addr1)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  BulkStorage_ReadData( &BulkRead, DATAFLASH_PAGESIZE_NORMAL+2, buffer, 128);
  TEST_ASSERT_EQUAL(2, BulkRead.loaded_page);
}

TEST(TestBulkStorage, BulkStorage_ReadData_ReadCap)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL*2];
  
  uint16_t rv = BulkStorage_ReadData( &BulkRead, 0, buffer, 2*DATAFLASH_PAGESIZE_NORMAL);
	TEST_ASSERT_EQUAL(DATAFLASH_PAGESIZE_NORMAL, rv);
}

/* ======================== */
/*
void BulkStorage_ReadClose(struct BulkStorage *b)
*/
TEST(TestBulkStorage, BulkStorage_ReadClose_FlushPageFlag)
{
  BulkStorage_ReadClose(&BulkRead);
  TEST_ASSERT_EQUAL(0, BulkRead.dirty);
}

TEST(TestBulkStorage, BulkStorage_ReadClose_ResetPageVariable)
{
  BulkStorage_ReadClose(&BulkRead);
  TEST_ASSERT_EQUAL(0xFFFF, BulkRead.loaded_page);
}

/* ======================== */
/*
uint8_t BulkStorage_VerifyImage(const DFToken_t *token)
*/
TEST(TestBulkStorage, BulkStorage_VerifyImage_Good)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0, sizeof(buffer));
  
  for ( int i = 0; i < NUM_PAGES; i ++ ) {
    BulkStorage_WriteData(&BulkWrite, DATAFLASH_PAGESIZE_NORMAL*i, buffer, DATAFLASH_PAGESIZE_NORMAL);
  }
  BulkStorage_WriteClose(&BulkWrite);
  TEST_ASSERT_EQUAL(1, BulkStorage_VerifyImage(&BS_Token));
}

TEST(TestBulkStorage, BulkStorage_VerifyImage_Bad)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0, sizeof(buffer));
  
  for ( int i = 0; i < NUM_PAGES; i ++ ) {
    BulkStorage_WriteData(&BulkWrite, DATAFLASH_PAGESIZE_NORMAL*i, buffer, DATAFLASH_PAGESIZE_NORMAL);
  }
  BulkStorage_WriteClose(&BulkWrite);
  
  PageArray[2].data[128] = 0xFF;
  
  TEST_ASSERT_EQUAL(0, BulkStorage_VerifyImage(&BS_Token));
}

/* ======================== */
/*
void BulkStorage_EraseImage(const DFToken_t *token)
*/
TEST(TestBulkStorage, BulkStorage_EraseImage_Data)
{
  uint32_t i;
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0, sizeof(buffer));
  
  for ( i = 0; i < NUM_PAGES; i ++ ) {
    BulkStorage_WriteData(&BulkWrite, DATAFLASH_PAGESIZE_NORMAL*i, buffer, DATAFLASH_PAGESIZE_NORMAL);
  }
  BulkStorage_WriteClose(&BulkWrite);
  BulkStorage_EraseImage(&BS_Token);

  memset(buffer, 0xFF, sizeof(buffer));
  for ( i = 0; i < NUM_PAGES; i ++ ) {
    if ( memcmp(buffer, PageArray[i].data+UNDERWRITE_SIZE, DATAFLASH_PAGESIZE_NORMAL) != 0 ) {
      break;
    }
  }
  TEST_ASSERT_EQUAL(NUM_PAGES, i);
}

TEST(TestBulkStorage, BulkStorage_EraseImage_Hash)
{
  uint8_t buffer[DATAFLASH_PAGESIZE_NORMAL];
  memset(buffer, 0, sizeof(buffer));
  
  for ( int i = 0; i < NUM_PAGES; i ++ ) {
    BulkStorage_WriteData(&BulkWrite, DATAFLASH_PAGESIZE_NORMAL*i, buffer, DATAFLASH_PAGESIZE_NORMAL);
  }
  BulkStorage_WriteClose(&BulkWrite);
  BulkStorage_EraseImage(&BS_Token);
  
  BulkStorage_WriteInit(&BulkWrite, &BS_Token);
  TEST_ASSERT_EQUAL(0xFFFFFFFF, BulkWrite.head.hash);
  BulkStorage_WriteClose(&BulkWrite);
}

/* ======================== */
/* ======================== */
TEST_GROUP_RUNNER(TestBulkStorage)
{
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteNotes_NoPtr);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteNotes_ClearData);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteNotes_CheckBounds_Under);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteNotes_CheckBounds_Over);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_ReadNotes_Good);
  
  /* ======================== */
	RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteInit_NoPtr);
	RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteInit_NoToken);
	RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteInit_Good);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteInit_CheckHeader);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteInit_CheckFlags);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteInit_CheckToken);
  
	RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteData_WriteNothing_Return);
	RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteData_WriteNothing_PagesClear);
	RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteData_WritePages_CheckP0);
	RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteData_WritePages_CheckP2);
	RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteData_WritePages_CheckValid);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteData_WriteFractionalPage_CheckValid);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteData_CheckDirtyFlag);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteData_WriteFullPage_CheckReturn);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteData_WriteGreaterDataThanPageSize_CheckReturn);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteData_WriteNextPage_NeighbouringPages);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteData_WriteNextPage);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteData_NoPermission_RV);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteData_NoPermission_Data);
  
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteClose_FlushPageFlag);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteClose_FlushPageData);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteClose_ResetPageVariable);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteClose_Timestamp);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_WriteClose_Hash);

  /* ======================== */
	RUN_TEST_CASE(TestBulkStorage, BulkStorage_ReadInit_NoPtr);
	RUN_TEST_CASE(TestBulkStorage, BulkStorage_ReadInit_NoToken);
	RUN_TEST_CASE(TestBulkStorage, BulkStorage_ReadInit_Good);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_ReadInit_CheckHeader);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_ReadInit_CheckFlags);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_ReadInit_CheckToken);
  
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_ReadData_ReadNothing_RV);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_ReadData_ReadNothing_Buff);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_ReadData_ReadData_RV);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_ReadData_ReadData_Buff);

  RUN_TEST_CASE(TestBulkStorage, BulkStorage_ReadData_ReadData_CheckPage_Addr0);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_ReadData_ReadData_CheckPage_Addr1);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_ReadData_ReadCap);
  
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_ReadClose_FlushPageFlag);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_ReadClose_ResetPageVariable);

  /* ======================== */
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_VerifyImage_Good);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_VerifyImage_Bad);
  
  /* ======================== */
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_EraseImage_Data);
  RUN_TEST_CASE(TestBulkStorage, BulkStorage_EraseImage_Hash);

}

/* ======================== */
/* ======================== */
DF_Return_t TEST_PageRead(const DFToken_t *token, uint16_t page, uint16_t offset, uint16_t size, uint8_t *dest)
{
  if ( dest == NULL ) {
    return DF_NoBuffer;
  }
  if ( token == NULL ) {
    return DF_NoToken;
  }
	if ( page >= NUM_PAGES ) {
		return DF_BadPage;
	}
  if ( offset > DATAFLASH_PAGESIZE_EXT ) {
    return DF_BadOffset;
  }
  if (( offset+size) > DATAFLASH_PAGESIZE_EXT ) {
    return DF_BadSize;
  }
  memcpy(dest, PageArray[page].data+offset+8, size); 
  return DF_Good;
}

DF_Return_t TEST_WriteBufferToFlash(const DFToken_t *token, uint16_t page, uint8_t whichbuf)
{
  return DF_Good;
}

DF_Return_t TEST_BufferWrite (const DFToken_t *token, uint16_t page, uint16_t offset, uint16_t size, uint8_t whichbuf, uint8_t *src)
{
  if ( src == NULL ) {
    return DF_NoBuffer;
  }
  if ( token == NULL ) {
    return DF_NoToken;
  }
	if ( page >= NUM_PAGES ) {
		return DF_BadPage;
	}
  if ( offset > DATAFLASH_PAGESIZE_EXT ) {
    return DF_BadOffset;
  }
  if (( offset+size) > DATAFLASH_PAGESIZE_EXT ) {
    return DF_BadSize;
  }
  memcpy(PageArray[page].data+offset+8, src, size); 
  return DF_Good;
}
