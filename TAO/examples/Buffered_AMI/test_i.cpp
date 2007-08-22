// $Id$

#include "test_i.h"

ACE_RCSID(Buffered_AMI, test_i, "$Id$")

test_i::test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
test_i::method (CORBA::ULong request_number,
                CORBA::ULong_out reply_number)
{
  ACE_DEBUG ((LM_DEBUG,
              "server: Iteration %d @ %T\n",
              request_number));

  reply_number = request_number;
}

void
test_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
