// -*- C++ -*-

#include "test_i.h"

ACE_RCSID (ForwardRequest,
           test_i,
           "$Id$")

test_i::test_i (CORBA::Short num,
                CORBA::ORB_ptr orb)
  : number_ (num),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

test_i::~test_i (void)
{
}

CORBA::Short
test_i::number (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->number_;
}

void
test_i::shutdown (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Server is shutting down via object %d.\n",
              this->number_));
  this->orb_->shutdown (0 TAO_ENV_ARG_PARAMETER);
}
