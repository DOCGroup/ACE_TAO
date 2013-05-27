//
// $Id$
//
#include "Peer.h"
#include "Session.h"

Peer::Peer (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Peer::~Peer (void)
{
}

Test::Session_ptr
Peer::create_session (Test::Session_Control_ptr control,
                      CORBA::ULong payload_size,
                      CORBA::ULong thread_count,
                      CORBA::ULong message_count,
                      CORBA::ULong peer_count)
{
  Session *session_impl = 0;
  ACE_NEW_THROW_EX (session_impl,
                    Session (control,
                             payload_size,
                             thread_count,
                             message_count,
                             peer_count),
                    CORBA::NO_MEMORY ());
  PortableServer::ServantBase_var transfer_ownership (session_impl);
  Test::Session_var session;

#if defined (CORBA_E_COMPACT) || defined (CORBA_E_MICRO)
  PortableServer::POA_var poa = this->_default_POA ();
  PortableServer::ObjectId_var id = poa->activate_object (transfer_ownership.in ());
  CORBA::Object_var object = poa->id_to_reference (id.in ());
  session = Test::Session::_unchecked_narrow (object.in());
#else
  session = session_impl->_this ();
#endif /* CORBA_E_COMPACT || CORBA_E_MICRO */

  return session._retn ();
}

void
Peer::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Peer::shutdown, waiting for threads\n"));

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Peer::shutdown, shutting down ORB\n"));
  this->orb_->shutdown (0);
}
