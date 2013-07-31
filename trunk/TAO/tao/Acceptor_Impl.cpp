// -*- C++ -*-

//=============================================================================
/**
 *  @file     Acceptor_Impl.cpp
 *
 *  $Id$
 *
 *  @author  Carlos O'Ryan <coryan@uci.edu>  Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_ACCEPTOR_IMPL_CPP
#define TAO_ACCEPTOR_IMPL_CPP

#include "tao/Acceptor_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Thread_Per_Connection_Handler.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/ORB_Core.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Transport.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

//////////////////////////////////////////////////////////////////////////////

template <class SVC_HANDLER>
TAO_Creation_Strategy<SVC_HANDLER>::TAO_Creation_Strategy (TAO_ORB_Core *orb_core)
  : ACE_Creation_Strategy<SVC_HANDLER> (0, orb_core->reactor()),
    orb_core_ (orb_core)
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
                      SVC_HANDLER (this->orb_core_),
                      -1);
    }

  return 0;
}

//////////////////////////////////////////////////////////////////////////////

template <class SVC_HANDLER>
TAO_Concurrency_Strategy<SVC_HANDLER>::TAO_Concurrency_Strategy (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core)
{
}

template <class SVC_HANDLER> int
TAO_Concurrency_Strategy<SVC_HANDLER>::activate_svc_handler (SVC_HANDLER *sh,
                                                             void *arg)
{
  sh->transport ()->opened_as (TAO::TAO_SERVER_ROLE);

  // Indicate that this transport was opened in the server role
  if (TAO_debug_level > 6)
    TAOLIB_DEBUG ((LM_DEBUG,
                "TAO (%P|%t) - Concurrency_Strategy::activate_svc_handler, "
                "opened as TAO_SERVER_ROLE\n"));

  // Here the service handler has been created and the new connection
  // has been accepted.  #REFCOUNT# is one at this point.

  if (this->ACE_Concurrency_Strategy<SVC_HANDLER>::activate_svc_handler (sh,
                                                                         arg) == -1)
    {
      // Activation fails, decrease reference.
      sh->transport ()->remove_reference ();

      // #REFCOUNT# is zero at this point.

      return -1;
    }

  // The service handler has been activated. Now cache the handler.
  if (sh->add_transport_to_cache () == -1)
    {
      // Adding to the cache fails, close the handler.
      sh->close ();

      // close() doesn't really decrease reference.
      sh->transport ()->remove_reference ();

      // #REFCOUNT# is zero at this point.

      if (TAO_debug_level > 0)
        {
          TAOLIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - Concurrency_Strategy::activate_svc_handler, ")
                      ACE_TEXT ("could not add the handler to cache\n")));
        }

      return -1;
    }

  // Registration with cache is successful, #REFCOUNT# is two at this
  // point.
  TAO_Server_Strategy_Factory *f = this->orb_core_->server_factory ();

  int result = 0;

  // Do we need to create threads?
  if (f->activate_server_connections ())
    {
      // Thread-per-connection concurrency model
      TAO_Thread_Per_Connection_Handler *tpch = 0;

      ACE_NEW_RETURN (tpch,
                      TAO_Thread_Per_Connection_Handler (sh,
                                                         this->orb_core_),
                      -1);

      result =
        tpch->activate (f->server_connection_thread_flags (),
                        f->server_connection_thread_count ());
    }
  else
    {
      // Otherwise, it is the reactive concurrency model. We may want
      // to register ourselves with the reactor. Call the register
      // handler on the transport.
      result = sh->transport ()->register_handler ();
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

      // close() doesn't really decrease reference.
      sh->transport ()->remove_reference ();

      // #REFCOUNT# is zero at this point.

      if (TAO_debug_level > 0)
         {
           const ACE_TCHAR *error_message = 0;
           if (f->activate_server_connections ())
             error_message = ACE_TEXT("could not activate new connection");
           else
             error_message = ACE_TEXT("could not register new connection in the reactor");

           TAOLIB_ERROR ((LM_ERROR,
                       ACE_TEXT("TAO (%P|%t) - Concurrency_Strategy::activate_svc_handler, ")
                       ACE_TEXT("%s\n"), error_message));
         }

      return -1;
    }

  // Success: #REFCOUNT# is two at this point.
  return result;
}

//////////////////////////////////////////////////////////////////////////////

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
TAO_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::TAO_Accept_Strategy (TAO_ORB_Core *orb_core)
  : ACCEPT_STRATEGY_BASE (orb_core->reactor ())
  , orb_core_ (orb_core)
{
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
TAO_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::open (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
                                                             bool restart)
{
  return ACCEPT_STRATEGY_BASE::open (local_addr, restart);
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
TAO_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::accept_svc_handler (SVC_HANDLER *svc_handler)
{
  int const result = ACCEPT_STRATEGY_BASE::accept_svc_handler (svc_handler);
  if (result == -1)
    {
      svc_handler->transport ()->remove_reference ();

      // #REFCOUNT# is zero at this point.
    }

  return result;
}


/////////////////////////////////////////////////////////////////////

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_ACCEPTOR_IMPL_CPP */
