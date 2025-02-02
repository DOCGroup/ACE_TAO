// -*- C++ -*-
#include "Hello.h"
#include "ace/OS_NS_unistd.h"

void
Hello::crash ()
{
  ACE::terminate_process (ACE_OS::getpid ());
}

char *
Hello::get_string ()
{
//  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Hello::get_string called\n"));
  return CORBA::string_dup ("Hello there!");
}
