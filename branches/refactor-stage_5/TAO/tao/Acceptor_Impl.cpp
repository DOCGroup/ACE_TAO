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

#include "ORB_Core.h"
// #include "ORB_Table.h"
#include "Server_Strategy_Factory.h"
// #include "Connector_Registry.h"
#include "Transport_Cache_Manager.h"
#include "Thread_Lane_Resources.h"
#include "Transport.h"
#include "Thread_Per_Connection_Handler.h"

#include "ace/Object_Manager.h"

#if !defined(__ACE_INLINE__)
#include "tao/Acceptor_Impl.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Acceptor_Impl,
           "$Id$")


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
    {
      // Purge connections (if necessary)
      this->orb_core_->lane_resources ().transport_cache ().purge ();

      ACE_NEW_RETURN (sh,
                      SVC_HANDLER (this->orb_core_,
                                   this->lite_flag_,
                                   this->arg_),
                      -1);
    }

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
  // Here the service handler has been created and the new connection
  // has been accepted.  #REFCOUNT# is one at this point.

  if (this->ACE_Concurrency_Strategy<SVC_HANDLER>::activate_svc_handler (sh,
                                                                         arg) == -1)
    return -1;

  // The service handler has been activated. Now cache the handler.
  if (sh->add_transport_to_cache () == -1)
    {
      // Adding to the cache fails, close the handler.
      sh->close ();

      // #REFCOUNT# is zero at this point.

      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) Could not add the handler to Cache \n")));
        }

      return -1;
    }

  // Registration with cache is successful, #REFCOUNT# is two at this
  // point.

  TAO_Server_Strategy_Factory *f =
    this->orb_core_->server_factory ();

  int result = 0;

  // Do we need to create threads?
  if (f->activate_server_connections ())
    {
      // Thread-per-connection concurrency model
      TAO_Thread_Per_Connection_Handler *tpch = 0;

      ACE_NEW_RETURN (tpch,
                      TAO_Thread_Per_Connection_Handler (sh),
                      -1);

      result = tpch->activate (f->server_connection_thread_flags (),
                               f->server_connection_thread_count ());
    }
  else
    {
      // Otherwise, it is the reactive concurrency model. We may want
      // to register ourselves with the reactor. Call the register
      // handler on the transport.
      result =
        sh->transport ()->register_handler ();
    }

  if (result != -1)
    {
      // Activation/registration successful: the handler has been
      // registered with either the Reactor or the
      // Thread-per-Connection_Handler, and the Transport Cache.
      // #REFCOUNT# is three at this point.

      // We can let go of our reference.
      sh->transport ()->remove_reference ();
    }
  else
    {
      // Activation/registration failure. #REFCOUNT# is two at this
      // point.

      // Remove from cache.
      sh->transport ()->purge_entry ();

      // #REFCOUNT# is one at this point.

      // Close handler.
      sh->close ();

      // #REFCOUNT# is zero at this point.

      if (TAO_debug_level > 0)
         {
           const char *error = 0;
           if (f->activate_server_connections ())
             error = "could not activate new connection";
           else
             error = "could not register new connection in the reactor";

           ACE_ERROR ((LM_ERROR,
                       "TAO (%P|%t) - Concurrency_Strategy::activate_svc_handler, "
                       "%s\n", error));
         }

      return -1;
    }

  // Success: #REFCOUNT# is two at this point.
  return result;
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
