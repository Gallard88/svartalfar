#include "unity_fixture.h"
#include "../Dataflash.h"

TEST_GROUP(Dataflash);

uint8_t Unity_SPI_Driver_Read(void)
{
  return 0xFF;
}

void Unity_SPI_Driver_Write(uint8_t data)
{
}

void Unity_SPI_Driver_ReadWrite(uint8_t *buffer, uint16_t length)
{
}

void Unity_SPI_Driver_InitCsPin(void)
{
}

void Unity_SPI_Driver_CsOn(void)
{
}

void Unity_SPI_Driver_CsOff(void)
{
}

const SPI_Driver_t Dataflash_SPI = {
  Unity_SPI_Driver_Read,
  Unity_SPI_Driver_Write,
  Unity_SPI_Driver_ReadWrite,
  Unity_SPI_Driver_InitCsPin,
  Unity_SPI_Driver_CsOn,
  Unity_SPI_Driver_CsOff
};

TEST_SETUP(Dataflash)
{
}

TEST_TEAR_DOWN(Dataflash)
{
}

/* ======================== */
TEST(Dataflash, DataflashDriver_CheckToken_InvalidPage_ZeroStart)
{
  DFToken_t token;
  token.start_page = 0;
  token.number_pages = 10;
  
	TEST_ASSERT_EQUAL(0xFFFF, DataFlash_CheckToken(&token, 10));
}

TEST(Dataflash, DataflashDriver_CheckToken_ValidPage_ZeroStart)
{
  DFToken_t token;
  token.start_page = 0;
  token.number_pages = 10;
  
	TEST_ASSERT_EQUAL(5, DataFlash_CheckToken(&token, 5));
}

TEST(Dataflash, DataflashDriver_CheckToken_InvalidPage_NonStart)
{
  DFToken_t token;
  token.start_page = 100;
  token.number_pages = 10;
  
	TEST_ASSERT_EQUAL(0xFFFF, DataFlash_CheckToken(&token, 10));
}

TEST(Dataflash, DataflashDriver_CheckToken_ValidPage_NonStart)
{
  DFToken_t token;
  token.start_page = 100;
  token.number_pages = 10;
  
	TEST_ASSERT_EQUAL(105, DataFlash_CheckToken(&token, 5));
}

/* ======================== */
/* ======================== */
TEST_GROUP_RUNNER(Dataflash)
{
	RUN_TEST_CASE(Dataflash, DataflashDriver_CheckToken_InvalidPage_ZeroStart);
	RUN_TEST_CASE(Dataflash, DataflashDriver_CheckToken_ValidPage_ZeroStart);
  
	RUN_TEST_CASE(Dataflash, DataflashDriver_CheckToken_InvalidPage_NonStart);
	RUN_TEST_CASE(Dataflash, DataflashDriver_CheckToken_ValidPage_NonStart);
    
}