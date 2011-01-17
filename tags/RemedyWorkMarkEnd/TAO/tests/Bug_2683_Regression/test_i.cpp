//
// $Id$
//
#include "test_i.h"
#include "ace/OS_NS_unistd.h"

test_i::test_i (ORB_Killer *k)
  : killer_ (k)
{
}

void
test_i::ping (void)
{

}

void
test_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) requesting shutdown\n"));
  this->killer_->activate ();
}


ORB_Killer::ORB_Killer (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
ORB_Killer::svc ()
{
  ACE_OS::sleep (1);
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) doing shutdown\n"));
  orb_->shutdown(1);
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) shutdown complete\n"));
  return 0;
}
