#include <ace/OS.h>

void poll (const char* filename)
{
  ACE_stat st;
  for (int r=1; r != 0; r = ACE_OS::stat (filename, &st))
  {
    ACE_OS::sleep (1);
  }
}
