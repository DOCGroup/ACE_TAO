// -*- C++ -*-
// $Id$

#include "test_i.h"

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
test_i::number (void)
{
  ++this->number_;
  if (this->number_ == 3)
    {
#if 1
      // Either throw any of these 4 exceptions with completion status
      // COMPLETED_NO and see how it's handled in receive_other() point.
      throw ::CORBA::TRANSIENT (0, CORBA::COMPLETED_NO);
      // throw ::CORBA::NO_RESPONSE (0, CORBA::COMPLETED_NO);
      // throw ::CORBA::COMM_FAILURE (0, CORBA::COMPLETED_NO);
      // throw ::CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO);
#else
      // Or throw any other exception and see how it's handled in
      // receive_exception() point on the client side.
      throw ::CORBA::INTERNAL (0, CORBA::COMPLETED_NO);
#endif
    }
  return this->number_;
}

void
test_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "SERVER: Server is shutting down at %d.\n",
              this->number_));
  this->orb_->shutdown (0);
}
