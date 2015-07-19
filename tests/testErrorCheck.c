#include "unity_fixture.h"
#include "../ErrorChecking.h"

TEST_GROUP(ErrorCheck);

TEST_SETUP(ErrorCheck)
{
}

TEST_TEAR_DOWN(ErrorCheck)
{
}

/* ======================== */
/* ======================== */
TEST(ErrorCheck, CalcChecksum_NULL_ZeroSize)
{
	TEST_ASSERT_EQUAL(0, CalcChecksum(NULL, 0));
}

TEST(ErrorCheck, CalcChecksum_NULL_NonZeroSize)
{
	TEST_ASSERT_EQUAL(0, CalcChecksum(NULL, 10));
}

TEST(ErrorCheck, CalcChecksum_Data_Process1BytesSize)
{
  uint8_t buffer[2];
  buffer[0] = 1;
  buffer[1] = 2;
	TEST_ASSERT_EQUAL(1, CalcChecksum(buffer, 1));
}

TEST(ErrorCheck, CalcChecksum_Data_ProcessMultiBytesSize)
{
  uint8_t buffer[2];
  buffer[0] = 1;
  buffer[1] = 2;
	TEST_ASSERT_EQUAL(3, CalcChecksum(buffer, 2));
}

/* ======================== */
TEST(ErrorCheck, CompareChecksum_NULL_ZeroSize_ZeroCmp)
{
	TEST_ASSERT_EQUAL(0, CompareChecksum(NULL, 0, 0));
}

TEST(ErrorCheck, CompareChecksum_NULL_NonZeroSize_ZeroCmp)
{
	TEST_ASSERT_EQUAL(0, CompareChecksum(NULL, 10, 0));
}

TEST(ErrorCheck, CompareChecksum_NULL_NonZeroSize_NonZeroCmp)
{
	TEST_ASSERT_EQUAL(0, CompareChecksum(NULL, 10, 0));
}

TEST(ErrorCheck, CompareChecksum_Data_Process1BytesSize_GoodMatch)
{
  uint8_t buffer[2];
  buffer[0] = 1;
  buffer[1] = 2;
	TEST_ASSERT_NOT_EQUAL(0, CompareChecksum(buffer, 1, 1));
}

TEST(ErrorCheck, CompareChecksum_Data_Process1BytesSize_BadMatch)
{
  uint8_t buffer[2];
  buffer[0] = 1;
  buffer[1] = 2;
	TEST_ASSERT_EQUAL(0, CompareChecksum(buffer, 1, 10));
}

TEST(ErrorCheck, CompareChecksum_Data_ProcessMultiBytesSize_GoodMatch)
{
  uint8_t buffer[2];
  buffer[0] = 1;
  buffer[1] = 2;
	TEST_ASSERT_NOT_EQUAL(0, CompareChecksum(buffer, 2, 3));
}

TEST(ErrorCheck, CompareChecksum_Data_ProcessMultiBytesSize_BadMatch)
{
  uint8_t buffer[2];
  buffer[0] = 1;
  buffer[1] = 2;
	TEST_ASSERT_EQUAL(0, CompareChecksum(buffer, 2, 5));
}

/* ======================== */
/* ======================== */
TEST(ErrorCheck, CalcCRC_NULL_ZeroSize)
{
	TEST_ASSERT_EQUAL(0, CalcCRC(NULL, 0));
}

TEST(ErrorCheck, CalcCRC_NULL_NonZeroSize)
{
	TEST_ASSERT_EQUAL(0, CalcCRC(NULL, 10));
}

TEST(ErrorCheck, CalcCRC_Data_Process1BytesSize)
{
  uint8_t buffer[2];
  buffer[0] = 1;
  buffer[1] = 2;
	TEST_ASSERT_EQUAL(4129, CalcCRC(buffer, 1));
}

TEST(ErrorCheck, CalcCRC_Data_ProcessMultiBytesSize)
{
  uint8_t buffer[2];
  buffer[0] = 1;
  buffer[1] = 2;
	TEST_ASSERT_EQUAL(4979, CalcCRC(buffer, 2));
}

/* ======================== */
TEST(ErrorCheck, CompareCalcCRC_NULL_ZeroSize_ZeroCmp)
{
	TEST_ASSERT_EQUAL(0, CompareCalcCRC(NULL, 0, 0));
}

TEST(ErrorCheck, CompareCalcCRC_NULL_NonZeroSize_ZeroCmp)
{
	TEST_ASSERT_EQUAL(0, CompareCalcCRC(NULL, 10, 0));
}

TEST(ErrorCheck, CompareCalcCRC_NULL_NonZeroSize_NonZeroCmp)
{
	TEST_ASSERT_EQUAL(0, CompareCalcCRC(NULL, 10, 0));
}

TEST(ErrorCheck, CompareCalcCRC_Data_Process1BytesSize_GoodMatch)
{
  uint8_t buffer[2];
  buffer[0] = 1;
  buffer[1] = 2;
	TEST_ASSERT_NOT_EQUAL(0, CompareCalcCRC(buffer, 1, 4129));
}

TEST(ErrorCheck, CompareCalcCRC_Data_Process1BytesSize_BadMatch)
{
  uint8_t buffer[2];
  buffer[0] = 1;
  buffer[1] = 2;
	TEST_ASSERT_EQUAL(0, CompareCalcCRC(buffer, 1, 10));
}

TEST(ErrorCheck, CompareCalcCRC_Data_ProcessMultiBytesSize_GoodMatch)
{
  uint8_t buffer[2];
  buffer[0] = 1;
  buffer[1] = 2;
	TEST_ASSERT_NOT_EQUAL(0, CompareCalcCRC(buffer, 2, 4979));
}

TEST(ErrorCheck, CompareCalcCRC_Data_ProcessMultiBytesSize_BadMatch)
{
  uint8_t buffer[2];
  buffer[0] = 1;
  buffer[1] = 2;
	TEST_ASSERT_EQUAL(0, CompareCalcCRC(buffer, 2, 5));
}

/* ======================== */
/* ======================== */
TEST_GROUP_RUNNER(ErrorCheck)
{
	RUN_TEST_CASE(ErrorCheck, CalcChecksum_NULL_ZeroSize);
	RUN_TEST_CASE(ErrorCheck, CalcChecksum_NULL_NonZeroSize);
	RUN_TEST_CASE(ErrorCheck, CalcChecksum_Data_Process1BytesSize);
	RUN_TEST_CASE(ErrorCheck, CalcChecksum_Data_ProcessMultiBytesSize);

	RUN_TEST_CASE(ErrorCheck, CompareChecksum_NULL_ZeroSize_ZeroCmp);
	RUN_TEST_CASE(ErrorCheck, CompareChecksum_NULL_NonZeroSize_ZeroCmp);
	RUN_TEST_CASE(ErrorCheck, CompareChecksum_NULL_NonZeroSize_NonZeroCmp);
	RUN_TEST_CASE(ErrorCheck, CompareChecksum_Data_Process1BytesSize_GoodMatch);
	RUN_TEST_CASE(ErrorCheck, CompareChecksum_Data_Process1BytesSize_BadMatch);
	RUN_TEST_CASE(ErrorCheck, CompareChecksum_Data_ProcessMultiBytesSize_GoodMatch);
	RUN_TEST_CASE(ErrorCheck, CompareChecksum_Data_ProcessMultiBytesSize_BadMatch);
  
	RUN_TEST_CASE(ErrorCheck, CalcCRC_NULL_ZeroSize);
	RUN_TEST_CASE(ErrorCheck, CalcCRC_NULL_NonZeroSize);
	RUN_TEST_CASE(ErrorCheck, CalcCRC_Data_Process1BytesSize);
	RUN_TEST_CASE(ErrorCheck, CalcCRC_Data_ProcessMultiBytesSize);

	RUN_TEST_CASE(ErrorCheck, CompareCalcCRC_NULL_ZeroSize_ZeroCmp);
	RUN_TEST_CASE(ErrorCheck, CompareCalcCRC_NULL_NonZeroSize_ZeroCmp);
	RUN_TEST_CASE(ErrorCheck, CompareCalcCRC_NULL_NonZeroSize_NonZeroCmp);
	RUN_TEST_CASE(ErrorCheck, CompareCalcCRC_Data_Process1BytesSize_GoodMatch);
	RUN_TEST_CASE(ErrorCheck, CompareCalcCRC_Data_Process1BytesSize_BadMatch);
	RUN_TEST_CASE(ErrorCheck, CompareCalcCRC_Data_ProcessMultiBytesSize_GoodMatch);
	RUN_TEST_CASE(ErrorCheck, CompareCalcCRC_Data_ProcessMultiBytesSize_BadMatch);
}

