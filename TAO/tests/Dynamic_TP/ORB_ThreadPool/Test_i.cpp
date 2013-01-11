//
// $Id$
//
#include "Test_i.h"
#include "ace/OS_NS_unistd.h"

Test_i::Test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Test_i::delay(void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Test_i::delay called\n"));
  ACE_OS::sleep (1);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Test_i::delay returning\n"));
}

void
Test_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
