// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     Acceptor_Impl.cpp
//
// = AUTHOR
//     Carlos O'Ryan <coryan@uci.edu>
//     Ossama Othman <othman@uci.edu>
//
// ============================================================================

#ifndef TAO_ACCEPTOR_IMPL_CPP
#define TAO_ACCEPTOR_IMPL_CPP

#include "tao/Acceptor_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB_Core.h"
#include "tao/ORB_Table.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/Connector_Registry.h"
#include "tao/debug.h"
#include "ace/Object_Manager.h"

#if !defined(__ACE_INLINE__)
#include "tao/Acceptor_Impl.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Acceptor_Impl, "$Id$")

////////////////////////////////////////////////////////////////////////////////

template <class SVC_HANDLER>
TAO_Creation_Strategy<SVC_HANDLER>::TAO_Creation_Strategy (TAO_ORB_Core *orb_core,
                                                           void *arg,
                                                           CORBA::Boolean flag)
  : orb_core_ (orb_core),
    arg_ (arg),
    lite_flag_ (flag)
{
}

template <class SVC_HANDLER> int
TAO_Creation_Strategy<SVC_HANDLER>::make_svc_handler (SVC_HANDLER *&sh)
{
  if (sh == 0)
    ACE_NEW_RETURN (sh,
                    SVC_HANDLER (this->orb_core_,
                                 this->lite_flag_,
                                 this->arg_),
                    -1);

  return 0;
}

////////////////////////////////////////////////////////////////////////////////

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

  // thread-per-connection concurrency model

  if (f->activate_server_connections ())
    return sh->activate (f->server_connection_thread_flags (),
                         f->server_connection_thread_count ());

  // reactive concurrency model

  // Bump the reference count. This helps keeping track of the number
  // of places the handler is registered with. This is particularly
  // helpful when it is needed to explicitly remove them from the
  // reactor prior to shutting down the ORB.  This is particularly
  // important for dynamically loaded ORBs where an application level
  // reactor, such as the Singleton reactor, is used instead of an ORB
  // created one.
  sh->incr_ref_count ();

  return this->orb_core_->reactor ()->register_handler
    (sh, ACE_Event_Handler::READ_MASK);
}

////////////////////////////////////////////////////////////////////////////////

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
TAO_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::TAO_Accept_Strategy (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core)
{
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
TAO_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::open (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
                                                             int restart)
{

  return ACCEPT_STRATEGY_BASE::open (local_addr,
                                     restart);
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
TAO_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::accept_svc_handler (SVC_HANDLER *svc_handler)
{
  return ACCEPT_STRATEGY_BASE::accept_svc_handler (svc_handler);
}


/////////////////////////////////////////////////////////////////////

#endif /* TAO_ACCEPTOR_IMPL_CPP */
