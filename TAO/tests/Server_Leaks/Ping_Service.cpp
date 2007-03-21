//
// $Id$
//
#include "Ping_Service.h"

ACE_RCSID(Server_Leaks, Ping_Service, "$Id$")

Ping_Service::Ping_Service (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Ping_Service::ping (void)
{
}

void
Ping_Service::shutdown (void)
{
  this->orb_->shutdown (0);
}
