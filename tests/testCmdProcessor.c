#include "unity_fixture.h"
#include "../CmdProcessor.h"
#include <string.h>


TEST_GROUP(CmdProcessor);

uint8_t Func_1_Called;
uint8_t Func_2_Called;
char    Func_Arg[64];

TEST_SETUP(CmdProcessor)
{
  Func_1_Called = 0;
  Func_2_Called = 0;
  memset(Func_Arg, 0, sizeof(Func_Arg));
}

TEST_TEAR_DOWN(CmdProcessor)
{
}

/* ======================== */
/* ======================== */
/*
    const char *CmdProc_SkipSpace(const char *buf);
 */

TEST(CmdProcessor, CmdProc_SkipSpace_NULL)
{
	TEST_ASSERT_EQUAL(NULL, CmdProc_SkipSpace(NULL));
}

TEST(CmdProcessor, CmdProc_SkipSpace_NoSpace)
{
  char buf[3];
  buf[0] = 'a';
  buf[1] = 'b';
  buf[2] = 0;
	TEST_ASSERT_EQUAL(&buf[0], CmdProc_SkipSpace(buf));
}

TEST(CmdProcessor, CmdProc_SkipSpace_Space)
{
  char buf[3];
  buf[0] = ' ';
  buf[1] = 'b';
  buf[2] = 0;
	TEST_ASSERT_EQUAL(&buf[1], CmdProc_SkipSpace(buf));
}

TEST(CmdProcessor, CmdProc_SkipSpace_NoChars)
{
  char buf[3];
  buf[0] = ' ';
  buf[1] = ' ';
  buf[2] = 0;
	TEST_ASSERT_EQUAL(&buf[2], CmdProc_SkipSpace(buf));
}

/* ======================== */
/*
    const char *CmdProc_SkipChars(const char *buf);
 */
TEST(CmdProcessor, CmdProc_SkipChars_NULL)
{
	TEST_ASSERT_EQUAL(NULL, CmdProc_SkipChars(NULL));
}

TEST(CmdProcessor, CmdProc_SkipChars_NoChar)
{
  char buf[3];
  buf[0] = ' ';
  buf[1] = 'b';
  buf[2] = 0;
	TEST_ASSERT_EQUAL(&buf[0], CmdProc_SkipChars(buf));
}

TEST(CmdProcessor, CmdProc_SkipChars_Char)
{
  char buf[3];
  buf[0] = 'a';
  buf[1] = ' ';
  buf[2] = 0;
	TEST_ASSERT_EQUAL(&buf[1], CmdProc_SkipChars(buf));
}

TEST(CmdProcessor, CmdProc_SkipChars_NoChars)
{
  char buf[3];
  buf[0] = 'a';
  buf[1] = 'a';
  buf[2] = '\0';
	TEST_ASSERT_EQUAL(&buf[2], CmdProc_SkipChars(buf));
}

/* ======================== */
/*
    void CmdProc_Clear(MsgBuff_t *ptr);
 */
TEST(CmdProcessor, CmdProc_Clear_ClearsCorrectly)
{
  char buf[3];
  MsgBuff_t mb;
  mb.buffer = buf;
  mb.size = sizeof(buf);
  buf[0] = 'a';
  buf[1] = 'b';
  buf[2] = '\0';
  CmdProc_Clear(&mb);
  
	TEST_ASSERT_EQUAL(0, buf[0]);
	TEST_ASSERT_NOT_EQUAL(0, buf[1]);
}

/* ======================== */
/*
    void CmdProc_Print (MsgBuff_t *ptr, const char *msg);
 */
TEST(CmdProcessor, CmdProc_Print_PrintString)
{
  char buf[64];
  MsgBuff_t mb;
  mb.buffer = buf;
  mb.size = sizeof(buf);

  CmdProc_Clear(&mb);
  CmdProc_Print(&mb, "Test123");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("Test123", buf, "CmdProc_Print_PrintString");
}

TEST(CmdProcessor, CmdProc_Print_NotBigEnough)
{
  char buf[64];
  MsgBuff_t mb;
  mb.buffer = buf;
  mb.size = 4;

  CmdProc_Clear(&mb);
  CmdProc_Print(&mb, "Test123");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("\0", buf, "CmdProc_Print_NotBigEnough");
}

TEST(CmdProcessor, CmdProc_Print_Concatenate)
{
  char buf[64];
  MsgBuff_t mb;
  mb.buffer = buf;
  mb.size = sizeof(buf);

  CmdProc_Clear(&mb);
  CmdProc_Print(&mb, "Test");
  CmdProc_Print(&mb, "123");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("Test123", buf, "CmdProc_Print_Concatenate");
}

/* ======================== */
/*
  void CmdProc_CRLF(MsgBuff_t *ptr);
 */
TEST(CmdProcessor, CmdProc_CRLF_WorksCorrectly)
{
  char buf[64];
  MsgBuff_t mb;
  mb.buffer = buf;
  mb.size = sizeof(buf);

  CmdProc_Clear(&mb);
  CmdProc_CRLF(&mb);
  TEST_ASSERT_EQUAL_STRING_MESSAGE("\r\n", buf, "CmdProc_CRLF");
}

TEST(CmdProcessor, CmdProc_CRLF_BadSize)
{
  char buf[64];
  MsgBuff_t mb;
  mb.buffer = buf;
  mb.size = 2;

  CmdProc_Clear(&mb);
  CmdProc_CRLF(&mb);
  TEST_ASSERT_EQUAL_STRING_MESSAGE("\0", buf, "CmdProc_CRLF_BadSize");
}

/* ======================== */
/*
    uint32_t CmdProc_BuildLine(MsgBuff_t *line, char c);
 */
TEST(CmdProcessor, CmdProc_BuildLine_ReturnValue)
{
  char buf[64];
  MsgBuff_t mb;
  mb.buffer = buf;
  mb.size = sizeof(buf);

  CmdProc_Clear(&mb);
	TEST_ASSERT_EQUAL(0, CmdProc_BuildLine(&mb, 'a'));
}
 
TEST(CmdProcessor, CmdProc_BuildLine_BufferValue)
{
  char buf[64];
  MsgBuff_t mb;
  mb.buffer = buf;
  mb.size = sizeof(buf);

  CmdProc_Clear(&mb);
  CmdProc_BuildLine(&mb, 'a');
  TEST_ASSERT_EQUAL_STRING_MESSAGE("a", buf, "CmdProc_BuildLine_BufferValue");
}

TEST(CmdProcessor, CmdProc_BuildLine_ReturnStringSize_CR)
{
  char buf[64];
  MsgBuff_t mb;
  mb.buffer = buf;
  mb.size = sizeof(buf);

  CmdProc_Clear(&mb);
  CmdProc_BuildLine(&mb, 'a');
	TEST_ASSERT_EQUAL(1, CmdProc_BuildLine(&mb, '\r'));
}
 
TEST(CmdProcessor, CmdProc_BuildLine_ReturnStringSize_LF)
{
  char buf[64];
  MsgBuff_t mb;
  mb.buffer = buf;
  mb.size = sizeof(buf);

  CmdProc_Clear(&mb);
  CmdProc_BuildLine(&mb, 'a');
	TEST_ASSERT_EQUAL(1, CmdProc_BuildLine(&mb, '\n'));
}
 
TEST(CmdProcessor, CmdProc_BuildLine_ReturnStringSize_Empty_CR)
{
  char buf[64];
  MsgBuff_t mb;
  mb.buffer = buf;
  mb.size = sizeof(buf);

  CmdProc_Clear(&mb);
	TEST_ASSERT_EQUAL(0, CmdProc_BuildLine(&mb, '\r'));
}
 
TEST(CmdProcessor, CmdProc_BuildLine_ReturnStringSize_Empty_LF)
{
  char buf[64];
  MsgBuff_t mb;
  mb.buffer = buf;
  mb.size = sizeof(buf);

  CmdProc_Clear(&mb);
	TEST_ASSERT_EQUAL(0, CmdProc_BuildLine(&mb, '\n'));
}
 
/* ======================== */
/*
    int8_t CmdProc_Lookup(const CmdTable_t *table, const char *line, MsgBuff_t resp);
 */
int8_t Func_1(const char *line, MsgBuff_t *resp)
{
  Func_1_Called = 1;
  strcpy(Func_Arg, line);
  return 0;
}
 
int8_t Func_2(const char *line, MsgBuff_t *resp)
{
  Func_2_Called = 1;
  strcpy(Func_Arg, line);
  return 0;
}
 
const CmdTable_t Table[] =
{
  { "123", Func_1 },
  { "abc", Func_2 },
  { NULL,  NULL }
};
 
TEST(CmdProcessor, CmdProc_Lookup_Match)
{
  char line[64];
  char resp[64];
  MsgBuff_t mb;
  mb.buffer = resp;
  mb.size = sizeof(resp);

  strcpy(line,"123");
	TEST_ASSERT_NOT_EQUAL(-1, CmdProc_Lookup(Table, line, &mb));
}

TEST(CmdProcessor, CmdProc_Lookup_Match_Func)
{
  char line[64];
  char resp[64];
  MsgBuff_t mb;
  mb.buffer = resp;
  mb.size = sizeof(resp);

  strcpy(line,"123");
  CmdProc_Lookup(Table, line, &mb);
	TEST_ASSERT_EQUAL(1, Func_1_Called);
	TEST_ASSERT_EQUAL(0, Func_2_Called);
}

TEST(CmdProcessor, CmdProc_Lookup_NoMatch)
{
  char line[64];
  char resp[64];
  MsgBuff_t mb;
  mb.buffer = resp;
  mb.size = sizeof(resp);

  strcpy(line,"ABC");
	TEST_ASSERT_EQUAL(-1, CmdProc_Lookup(Table, line, &mb));
}

TEST(CmdProcessor, CmdProc_Lookup_UnderMatch)
{
  char line[64];
  char resp[64];
  MsgBuff_t mb;
  mb.buffer = resp;
  mb.size = sizeof(resp);

  strcpy(line,"12");
	TEST_ASSERT_EQUAL(-1, CmdProc_Lookup(Table, line, &mb));
}

TEST(CmdProcessor, CmdProc_Lookup_OverMatch)
{
  char line[64];
  char resp[64];
  MsgBuff_t mb;
  mb.buffer = resp;
  mb.size = sizeof(resp);

  strcpy(line,"1234");
	TEST_ASSERT_EQUAL(-1, CmdProc_Lookup(Table, line, &mb));
}

/* ======================== */
TEST(CmdProcessor, CmdProc_Lookup_NoArg)
{
  char resp[64];
  MsgBuff_t mb;
  mb.buffer = resp;
  mb.size = sizeof(resp);

  CmdProc_Lookup(Table, "123", &mb);
  TEST_ASSERT_EQUAL_STRING_MESSAGE("\0", Func_Arg, "CmdProc_Lookup_NoArg");
}

TEST(CmdProcessor, CmdProc_Lookup_Arg)
{
  char resp[64];
  MsgBuff_t mb;
  mb.buffer = resp;
  mb.size = sizeof(resp);

  CmdProc_Lookup(Table, "123 abc", &mb);
  TEST_ASSERT_EQUAL_STRING_MESSAGE("abc", Func_Arg, "CmdProc_Lookup_Arg");
}


/* ======================== */
/* ======================== */
TEST_GROUP_RUNNER(CmdProcessor)
{
	RUN_TEST_CASE(CmdProcessor, CmdProc_SkipSpace_NULL);
  RUN_TEST_CASE(CmdProcessor, CmdProc_SkipSpace_NoSpace);
  RUN_TEST_CASE(CmdProcessor, CmdProc_SkipSpace_Space);
  RUN_TEST_CASE(CmdProcessor, CmdProc_SkipSpace_NoChars);

  RUN_TEST_CASE(CmdProcessor, CmdProc_SkipChars_NULL);
  RUN_TEST_CASE(CmdProcessor, CmdProc_SkipChars_NoChar);
  RUN_TEST_CASE(CmdProcessor, CmdProc_SkipChars_Char);
  RUN_TEST_CASE(CmdProcessor, CmdProc_SkipChars_NoChars);

  RUN_TEST_CASE(CmdProcessor, CmdProc_Clear_ClearsCorrectly);
  
  RUN_TEST_CASE(CmdProcessor, CmdProc_CRLF_WorksCorrectly); 
  RUN_TEST_CASE(CmdProcessor, CmdProc_CRLF_BadSize); 
  
  RUN_TEST_CASE(CmdProcessor, CmdProc_Print_PrintString); 
  RUN_TEST_CASE(CmdProcessor, CmdProc_Print_NotBigEnough);
  RUN_TEST_CASE(CmdProcessor, CmdProc_Print_Concatenate);
  
  RUN_TEST_CASE(CmdProcessor, CmdProc_BuildLine_ReturnValue);
  RUN_TEST_CASE(CmdProcessor, CmdProc_BuildLine_BufferValue);
  RUN_TEST_CASE(CmdProcessor, CmdProc_BuildLine_ReturnStringSize_CR);
  RUN_TEST_CASE(CmdProcessor, CmdProc_BuildLine_ReturnStringSize_LF);
  RUN_TEST_CASE(CmdProcessor, CmdProc_BuildLine_ReturnStringSize_Empty_CR);
  RUN_TEST_CASE(CmdProcessor, CmdProc_BuildLine_ReturnStringSize_Empty_LF);

  RUN_TEST_CASE(CmdProcessor, CmdProc_Lookup_Match);
  RUN_TEST_CASE(CmdProcessor, CmdProc_Lookup_Match_Func);
  RUN_TEST_CASE(CmdProcessor, CmdProc_Lookup_NoMatch);
  RUN_TEST_CASE(CmdProcessor, CmdProc_Lookup_UnderMatch);
  RUN_TEST_CASE(CmdProcessor, CmdProc_Lookup_OverMatch);
  RUN_TEST_CASE(CmdProcessor, CmdProc_Lookup_NoArg);
  RUN_TEST_CASE(CmdProcessor, CmdProc_Lookup_Arg);
  
/*  RUN_TEST_CASE(CmdProcessor, ); */
  
}
