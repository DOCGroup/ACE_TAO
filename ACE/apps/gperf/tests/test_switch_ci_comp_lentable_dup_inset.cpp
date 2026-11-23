#include "test_driver.h"
#include "ace/OS_main.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return gperf_test_main (argc, argv, Gperf_Test_Mode::switch_prefix);
}
