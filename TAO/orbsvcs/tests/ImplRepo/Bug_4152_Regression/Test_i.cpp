/* -*- C++ -*-  */

#include "Test_i.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"

CORBA::Short
Test_i::get_server_num (void)
{
  return 1;
}

void
Test_i::terminate (void)
{
  printf ("---------------------------------------> server terminate %d\n", ACE_OS::getpid());
  exit (0);
}
