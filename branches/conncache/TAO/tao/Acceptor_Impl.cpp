// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   Acceptor_Impl.cpp
//
// = AUTHOR
//   Carlos O'Ryan <coryan@cs.wustl.edu>
//   Ossama Othman <othman@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_ACCEPTOR_IMPL_C
#define TAO_ACCEPTOR_IMPL_C

#include "tao/Acceptor_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/IIOP_Connector.h"
#include "tao/UIOP_Connector.h"
#include "tao/Connector_Registry.h"

#if !defined(__ACE_INLINE__)
#include "tao/Acceptor_Impl.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Acceptor_Impl, "$Id$")

template <class SVC_HANDLER>
TAO_Creation_Strategy<SVC_HANDLER>::TAO_Creation_Strategy (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core)
{
}

template <class SVC_HANDLER> int
TAO_Creation_Strategy<SVC_HANDLER>::make_svc_handler (SVC_HANDLER *&sh)
{
  if (sh == 0)
    ACE_NEW_RETURN (sh,
                    SVC_HANDLER (this->orb_core_),
                    -1);

  return 0;
}

//---------------------------------------------------------------

template <class SVC_HANDLER>
TAO_Concurrency_Strategy<SVC_HANDLER>::TAO_Concurrency_Strategy (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core)
{
}


template <class SVC_HANDLER> int
TAO_Concurrency_Strategy<SVC_HANDLER>::activate_svc_handler (SVC_HANDLER *sh,
                                                             void *arg)
{
  if (this->ACE_Concurrency_Strategy<SVC_HANDLER>::activate_svc_handler (sh,
                                                                         arg) == -1)
    return -1;

  TAO_Server_Strategy_Factory *f =
    this->orb_core_->server_factory ();

  if (f->activate_server_connections ())
    return sh->activate (f->server_connection_thread_flags (),
                         f->server_connection_thread_count ());

  return this->orb_core_->reactor ()->register_handler
    (sh, ACE_Event_Handler::READ_MASK);
}

//---------------------------------------------------------------

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1, class TAO_PEER_CONNECTOR> 
TAO_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2, TAO_PEER_CONNECTOR>::TAO_Accept_Strategy (TAO_ORB_Core *orb_core,
                                                                                                const CORBA::ULong &tag)
  : ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2> (orb_core->reactor ()),
    orb_core_ (orb_core),
    tag_ (tag)
{
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1, class TAO_PEER_CONNECTOR> int 
TAO_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2, TAO_PEER_CONNECTOR>::open (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
                                                                                 int restart = 0)
{
  int result = ACCEPT_STRATEGY_BASE::open (local_addr,
                                           restart);

  if (result == 0)
    return result;

  // If the error occured due to the fact that the file descriptor
  // limit was exhausted, then purge the connection cache of some
  // entries.
  result = this->out_of_sockets_handler ();
  if (result == -1)
    return -1;

  // If we are able to purge, try again.
  return ACCEPT_STRATEGY_BASE::open (local_addr, restart);
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1, class TAO_PEER_CONNECTOR> int 
TAO_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2, TAO_PEER_CONNECTOR>::accept_svc_handler (SVC_HANDLER *svc_handler)
{
  // Try to find out if the implementation of the reactor that we are
  // using requires us to reset the event association for the newly
  // created handle. This is because the newly created handle will
  // inherit the properties of the listen handle, including its event
  // associations.
  int reset_new_handle = this->orb_core_->reactor ()->uses_event_associations ();

  int result = this->acceptor_.accept (svc_handler->peer (), // stream
                                   0, // remote address
                                   0, // timeout
                                   1, // restart
                                   reset_new_handle  // reset new handler
                                   );
  if (result == 0)
    return result;

  // If the error occured due to the fact that the file descriptor
  // limit was exhausted, then purge the connection cache of some
  // entries.
  result = this->out_of_sockets_handler ();

  // Note: SunOS5.5 as well as Linux crib when the accept is tried again after purging.
  // Close down handler to avoid memory leaks.
  svc_handler->close (0);
  return -1;
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1, class TAO_PEER_CONNECTOR> TAO_PEER_CONNECTOR *
TAO_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2, TAO_PEER_CONNECTOR>::get_connector (void)
{
  return ACE_dynamic_cast (TAO_PEER_CONNECTOR *,
                           this->orb_core_->connector_registry ()->get_connector (this->tag_));
  
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1, class TAO_PEER_CONNECTOR> int 
TAO_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2, TAO_PEER_CONNECTOR>::out_of_sockets_handler (void)
{
   // ENOBUFS had to be checked on NT while ENOENT check had to be
  // added for Solaris + Linux.
  if (ACE::out_of_handles (errno))
    {
      // Close connections which are cached by explicitly purging the
      // connection cache maintained by the connector.
      ACE_DEBUG ((LM_DEBUG, "Purging connections from Connection Cache...\n"));

      return this->get_connector ()->cached_connect_strategy ().purge_connections ();
    }

  return -1;
}

#endif /* TAO_ACCEPTOR_IMPL_C */
