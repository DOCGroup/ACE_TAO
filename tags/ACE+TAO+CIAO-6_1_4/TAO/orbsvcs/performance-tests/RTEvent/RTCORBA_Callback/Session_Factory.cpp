// $Id$

#include "Session_Factory.h"
#include "Session.h"
#include "Servant_var.h"

Session_Factory::Session_Factory (CORBA::ORB_ptr orb,
                                  PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , poa_ (PortableServer::POA::_duplicate (poa))
{
}

Test::Session_ptr
Session_Factory::create_new_session (Test::Callback_ptr cb)
{
  Servant_var<Session> session_impl (new Session (cb,
                                                  this->poa_.in ()));

  return session_impl->_this ();
}

void
Session_Factory::shutdown (void)
{
  this->orb_->shutdown (0);
}

PortableServer::POA_ptr
Session_Factory::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
