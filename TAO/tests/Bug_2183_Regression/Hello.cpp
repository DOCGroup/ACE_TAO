//
// $Id$
//
#include "Hello.h"

ACE_RCSID(Hello, Hello, "Hello.cpp,v 1.3 2002/01/29 20:21:07 okellogg Exp")

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("Hello there!");
}

void
Hello::shutdown (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
