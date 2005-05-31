// $Id$
// A simple test for performance of the ACEXML_escape_string() function

#include "ACEXML/common/XML_Util.h"

#include "ace/OS_main.h"
#include "ace/Time_Value.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Log_Msg.h"

const int MAX_ITERATIONS = 100 * 1000;
const int NUM_TEST_STRS = 6;

static bool is_escaped(const ACEXML_String& s)
{
  if (s[0] != ACE_TEXT('&'))
    return false;
  if (s[s.length() - 1] != ACE_TEXT(';'))
    return false;
  return true;
}

static int run_tests(ACEXML_String test_strings[NUM_TEST_STRS], int iterations)
{
  // Test 1 - Escape the strings using a new temporary string each iteration.
  ACE_Time_Value start = ACE_OS::gettimeofday();
  int i = 0;
  for (i = 0; i < iterations; ++i)
  {
    ACEXML_String tmp = ACEXML_escape_string(test_strings[i % NUM_TEST_STRS]);
    if (! is_escaped(tmp))
    {
      ACE_ERROR((LM_ERROR, "Error: Failed to escape string\n"));
      return 1;
    }
  }
  ACE_DEBUG((LM_DEBUG, "Test1 took %dms\n", (ACE_OS::gettimeofday() - start).msec()));

  // Test 2 - Escape the strings using a shared temporary string. This shouldn't
  // be any faster than Test 1 as long as the compiler has return value optimization.
  ACEXML_String tmp;
  start = ACE_OS::gettimeofday();
  for (i = 0; i < iterations; ++i)
  {
    tmp = ACEXML_escape_string(test_strings[i % NUM_TEST_STRS]);
    if (! is_escaped(tmp))
    {
      ACE_ERROR((LM_ERROR, "Error: Failed to escape string\n"));
      return 1;
    }
  }
  ACE_DEBUG((LM_DEBUG, "Test2 took %dms\n", (ACE_OS::gettimeofday() - start).msec()));

  // Test 3 - Escape the strings using a shared temporary string. This time, we use
  // the alternate form of ACEXML_escape_string() so that our temporary buffer is reused.
  tmp.clear(1);
  start = ACE_OS::gettimeofday();
  for (i = 0; i < iterations; ++i)
  {
    ACEXML_escape_string(test_strings[i % NUM_TEST_STRS], tmp);
    if (! is_escaped(tmp))
    {
      ACE_ERROR((LM_ERROR, "Error: Failed to escape string\n"));
      return 1;
    }
  }
  ACE_DEBUG((LM_DEBUG, "Test3 took %dms\n", (ACE_OS::gettimeofday() - start).msec()));

  // Test 4 - Same as Test 3, except that the tmp buffer shouldn't have to resize.
  start = ACE_OS::gettimeofday();
  for (i = 0; i < iterations; ++i)
  {
    ACEXML_escape_string(test_strings[i % NUM_TEST_STRS], tmp);
    if (! is_escaped(tmp))
    {
      ACE_ERROR((LM_ERROR, "Error: Failed to escape string\n"));
      return 1;
    }
  }
  ACE_DEBUG((LM_DEBUG, "Test4 took %dms\n", (ACE_OS::gettimeofday() - start).msec()));
  return 0;
}

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACEXML_String test_strings[NUM_TEST_STRS] = {
    ACE_TEXT("\"xxxxx\"xxxxxxxx xx\"xxxxxx xxxxxx\"xxxxxxxxxx xxxxxxxx\"xxxxxx\""),
    ACE_TEXT("'xxxxx\'xxxxxxxx' xxxxxxxx xx'xxxxxxxx'xxxxxx xxxxxxx'xxxxxxx'"),
    ACE_TEXT("&xxxx&xxxxxxxxx &xxxxxxxx xxxxx&xxxxxxxxxxx xxxx&xxxxxxxxxx&"),
    ACE_TEXT(">xx>xxxxxxxxxxx >xxxxxxxx xxxxx>xxxxxxxxxxx xxxxx>xxxxxxxxx>"),
    ACE_TEXT("<xxxxx<xxxxxxxx xxxxxxxx <xxxxxxxxxxxxxxx<x xxxxxxxxxxxxxx<"),
    ACE_TEXT("&xxxx\"xxxxxxx&xx xxx'xxxxx xx<xxxxxxx>xxxxxxx xx\"xxxxxxxxxxxx>"),
  };

  if (run_tests(test_strings, MAX_ITERATIONS) != 0)
    return 1;

  ACE_DEBUG((LM_DEBUG, "Rerun tests with larger strings\n"));
  for (int i = 0; i < NUM_TEST_STRS; ++i)
  {
    for (int j = 0; j < 5; ++j)
    {
      test_strings[i] += test_strings[i];
    }
  }

  if (run_tests(test_strings, MAX_ITERATIONS / 10) != 0)
    return 1;

  return 0;
}
