// $Id$

#include "ace/OS_NS_sys_stat.h"
#include "ace/OS_NS_unistd.h"

void poll (const char* filename)
{
  int r;
  do
  {
    ACE_stat st;
    r = ACE_OS::stat (filename, &st);
    ACE_OS::sleep (1); // Allows time between waiting scans and for contents to be written
  } while (r != 0);
}
