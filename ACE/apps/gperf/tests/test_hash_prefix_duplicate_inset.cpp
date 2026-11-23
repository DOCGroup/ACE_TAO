#include "test_driver.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return gperf_test_main (argc, argv, Gperf_Test_Mode::duplicate_prefix);
}
