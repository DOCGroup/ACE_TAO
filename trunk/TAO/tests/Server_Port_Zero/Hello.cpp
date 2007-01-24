//
// $Id$
//
#include "Hello.h"

ACE_RCSID(Server_Port_Zero,
          Hello,
          "$Id$")

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
