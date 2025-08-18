#include "testC.h"

#include "ace/OS_NS_stdlib.h"
#include "ace/OS_main.h"
#include <string>

template <typename T>
int check (T const &map)
{
  typedef typename T::const_iterator iter_t;
  iter_t const i = map.find ("abc");
  if (i != map.begin ())
    {
      ACE_DEBUG ((LM_DEBUG, "String-keyed map didn't have 'abc' as first entry\n"));
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}

int ACE_TMAIN (int, ACE_TCHAR *[])
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

  TestStructSeq testStructSeq;
  testStructSeq.length (1);
  testStructSeq[0] = testStruct;

  testData.intIntMap[4] = 100;

  testData.stringStructsMap["test"] = testStruct;
  testData.stringStructsMap["abc"] = testStruct;
  int ret = check (testData.stringStructsMap);

  testData.stringSequenceMap["test"] = testStructSeq;
  testData.stringSequenceMap["abc"] = testStructSeq;
  ret += check (testData.stringSequenceMap);

  testData.stringMapMap["test"] = testStructMap;
  testData.stringMapMap["abc"] = testStructMap;
  ret += check (testData.stringMapMap);

  testData.mapStringMap[testIntStringMap] = "Hello World";

  testData.testStructMapArray[0][2] = testStruct;

  testData.testStructMapSeq.length (1);
  testData.testStructMapSeq[0][1024] = testStruct;

  UnionWithMap union_with_map;
  union_with_map.named_map (testStructMap);
  union_with_map.anon_map (testData.stringMapMap);

  return ret;
}
