#include "testC.h"

#include "ace/OS_NS_stdlib.h"
#include "ace/OS_main.h"
#include <string>

int ACE_TMAIN(int, ACE_TCHAR *[])
{
  // No need to test the actual map works as it's just a typedef to std::map
  DataStruct testData;

  RecurseStruct rStruct;
  rStruct.iMapR[10] = rStruct;

  TestStruct testStruct;
  testStruct.id = 42;
  testStruct.msg = "Hello World!";

  TestStructMap testStructMap;
  testStructMap[42] = testStruct;

  TestIntStringMap testIntStringMap;
  testIntStringMap[42] = "Hello World!";

  TestStructSeq testStructSeq(1, 1, TestStructSeq::allocbuf(1), /* release = */ true);
  testStructSeq[0] = testStruct;

  testData.intIntMap[4] = 100;
  testData.stringStructsMap["test"] = testStruct;
  testData.stringSequenceMap["test"] = testStructSeq;
  testData.stringMapMap["test"] = testStructMap;
  testData.mapStringMap[testIntStringMap] = "Hello World";

  return EXIT_SUCCESS;
}
