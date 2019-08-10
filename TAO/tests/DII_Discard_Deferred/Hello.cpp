// -*- C++ -*-
#include "Hello.h"
#include "ace/OS_NS_unistd.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Hello::get_string called\n"));
  ACE_OS::sleep (2);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Hello::get_string returning\n"));
  return CORBA::string_dup ("Hello there!");
}

void
Hello::shutdown (void)
{
  this->orb_->shutdown (0);
}
