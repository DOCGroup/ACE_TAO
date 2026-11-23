#ifndef ACE_APPS_GPERF_TESTS_TEST_DRIVER_H
#define ACE_APPS_GPERF_TESTS_TEST_DRIVER_H

#include "ace/OS_NS_string.h"

enum class Gperf_Test_Mode
{
  none,
  switch_prefix,
  duplicate_prefix
};

int gperf_test_main (int argc, ACE_TCHAR *argv[], Gperf_Test_Mode test_mode);

#endif /* ACE_APPS_GPERF_TESTS_TEST_DRIVER_H */
