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

CORBA::Short
test_i::number (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->number_;
}

void
test_i::shutdown (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Server is shutting down via object %d.\n",
              this->number_));
  this->orb_->shutdown (0, ACE_TRY_ENV);
}
