// -*- C++ -*-

#include "test_i.h"

ACE_RCSID (IORInterceptors,
           test_i,
           "$Id$")

test_i::test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
test_i::shutdown (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "\"test_i\" is shutting down.\n"));
  this->orb_->shutdown (0 TAO_ENV_ARG_PARAMETER);
}
