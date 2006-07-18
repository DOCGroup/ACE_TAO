//
// $Id$
//
#include "Hello.h"

ACE_RCSID(Hello, Hello, "$Id$")

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Hello::op (
        ::Test::Fls_out fstruct,
        ::Test::Vls_out vstruct
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        ::CORBA::SystemException
      ))
{
  Test::Fls a;
  fstruct = a;
  vstruct = new Test::Vls;
}

void
Hello::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
