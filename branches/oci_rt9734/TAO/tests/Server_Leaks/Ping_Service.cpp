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
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Ping_Service::shutdown (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0);
}
