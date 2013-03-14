// $Id$

#include <stdio.h>
__declspec(dllimport) int test_entry(void);

// This is plain Windows code, not ACE. Therefore we disable
// the check for ACE_OS
// FUZZ: disable check_for_lack_ACE_OS
// FUZZ: disable check_for_improper_main_declaration

int main (int, char *[])
{
  char line[80];
  test_entry();
  puts ("Ok, go... hit return to stop.");
  gets (line);
  return 0;
}
