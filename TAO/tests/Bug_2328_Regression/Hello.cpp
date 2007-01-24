//
// $Id$
//
#include "Hello.h"

ACE_RCSID(Hello, Hello, "$Id$")

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string (const char * A, const char * B, const char * C)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Received: <%s>, <%s>, <%s>\n", A, B, C));
  return CORBA::string_dup ("Hello there!");
}

void
Hello::shutdown (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
