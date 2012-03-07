//
// $Id$
//
#include "Test_i.h"

test_i::test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
test_i::send_stuff (const char* string)
{
  ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - %s\n", string));
}

void
test_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) About to invoke shudown...\n"));
  this->orb_->shutdown (0);
}
