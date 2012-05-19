// $Id$

#include "test_i.h"

test_i::test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

CORBA::Long
test_i::test_method ()
{
  ACE_DEBUG ((LM_DEBUG, "client called us.\n"));

  return 0;
}

void
test_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
