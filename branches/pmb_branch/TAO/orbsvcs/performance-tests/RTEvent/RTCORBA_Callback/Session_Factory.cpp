//
// $Id$
//
#include "Session_Factory.h"
#include "Session.h"
#include "Servant_var.h"

ACE_RCSID(TAO_RTEC_Perf_RTCORBA_Callback, Session_Factory, "$Id$")

Session_Factory::Session_Factory (CORBA::ORB_ptr orb,
                                  PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , poa_ (PortableServer::POA::_duplicate (poa))
{
}

Test::Session_ptr
Session_Factory::create_new_session (Test::Callback_ptr cb
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Servant_var<Session> session_impl (new Session (cb,
                                                  this->poa_.in ()));

  return session_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
Session_Factory::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}

PortableServer::POA_ptr
Session_Factory::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
