#include <stdio.h>
__declspec(dllimport) int test_entry(void);

int main (int, char *[])
{
  char line[80];
  test_entry();
  puts ("Ok, go... hit return to stop.");
  gets (line);
  return 0;
}
