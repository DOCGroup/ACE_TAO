/* -*- C++ -*- $Id$ */

#include "Test_i.h"

CORBA::Long
Test_i::server_pid (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P) server pid called\n"));
  return ACE_OS::getpid ();
}



