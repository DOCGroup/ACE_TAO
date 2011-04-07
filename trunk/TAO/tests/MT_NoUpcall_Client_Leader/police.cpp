// $Id$

#include "ace/OS_NS_sys_stat.h"
#include "ace/OS_NS_unistd.h"

void poll (const char* filename)
{
  ACE_stat st;
  for (int r=1; r != 0; r = ACE_OS::stat (filename, &st))
  {
    ACE_OS::sleep (1);
  }
}
