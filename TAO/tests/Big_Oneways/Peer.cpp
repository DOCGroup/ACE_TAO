//
// $Id$
//
#include "Peer.h"
#include "Session.h"

ACE_RCSID(Big_Oneways, Peer, "$Id$")

Peer::Peer (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Peer::~Peer (void)
{
}

Test::Session_ptr
Peer::create_session (Test::Session_Control_ptr control,
                      CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Session *session_impl;
  ACE_NEW_THROW_EX (session_impl,
                    Session (control),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (Test::Session::_nil ());
  PortableServer::ServantBase_var transfer_ownership (session_impl);

  return session_impl->_this (ACE_TRY_ENV);
}

void
Peer::shutdown (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Peer::shutdown, waiting for threads\n"));

  // Wait for all the threads.
  ACE_Thread_Manager::instance ()->wait ();

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Peer::shutdown, shutting down ORB\n"));
  this->orb_->shutdown (0, ACE_TRY_ENV);
}
