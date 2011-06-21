//
// $Id$
//
#include "Service.h"

Service::Service (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Service::run_test (Test::Callback_ptr callback)
{
  callback->test_oneway ();
}

void
Service::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Shuting down self (server)\n"));

  this->orb_->shutdown (0);
}
