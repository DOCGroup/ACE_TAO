// $Id$

#include "orbsvcs/SSLIOP/IIOP_SSL_Connector.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/IIOP_Endpoint.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Connect_Strategy.h"
#include "tao/Wait_Strategy.h"
#include "tao/Profile_Transport_Resolver.h"
#include "tao/Transport.h"
#include "tao/Transport_Descriptor_Interface.h"
#include "ace/Strategies_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::IIOP_SSL_Connector::IIOP_SSL_Connector (void)
  : TAO_IIOP_Connector (),
    connect_strategy_ (),
    base_connector_ ()
{
}

TAO::IIOP_SSL_Connector::~IIOP_SSL_Connector (void)
{
}

int
TAO::IIOP_SSL_Connector::open (TAO_ORB_Core *orb_core)
{
  this->orb_core (orb_core);

  // Create our connect strategy
  if (this->create_connect_strategy () == -1)
    return -1;

  // Our connect creation strategy
  CONNECT_CREATION_STRATEGY *connect_creation_strategy = 0;

  ACE_NEW_RETURN (connect_creation_strategy,
                  CONNECT_CREATION_STRATEGY (orb_core->thr_mgr (),
                                             orb_core),
                  -1);

  // Our activation strategy
  CONNECT_CONCURRENCY_STRATEGY *concurrency_strategy = 0;

  ACE_NEW_RETURN (concurrency_strategy,
                  CONNECT_CONCURRENCY_STRATEGY (orb_core),
                  -1);


  return this->base_connector_.open (this->orb_core ()->reactor (),
                                     connect_creation_strategy,
                                     &this->connect_strategy_,
                                     concurrency_strategy);
}

int
TAO::IIOP_SSL_Connector::close (void)
{
  delete this->base_connector_.creation_strategy ();
  delete this->base_connector_.concurrency_strategy ();
  return this->base_connector_.close ();
}

int
TAO::IIOP_SSL_Connector::set_validate_endpoint (TAO_Endpoint *endpoint)
{
  if (endpoint->tag () != IOP::TAG_INTERNET_IOP)
    return -1;

  TAO_IIOP_Endpoint *iiop_endpoint =
    dynamic_cast<TAO_IIOP_Endpoint *> (endpoint);

  if (iiop_endpoint == 0)
    return -1;

  const ACE_INET_Addr &remote_address = iiop_endpoint->object_addr ();

  // Verify that the remote ACE_INET_Addr was initialized properly.
  // Failure can occur if hostname lookup failed when initializing the
  // remote ACE_INET_Addr.
  if (remote_address.get_type () != AF_INET
#if defined (ACE_HAS_IPV6)
      && remote_address.get_type () != AF_INET6
#endif /* ACE HAS_IPV6 */
     )
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - IIOP_SSL connection failed.\n")
                      ACE_TEXT ("TAO (%P|%t) - This is most likely ")
                      ACE_TEXT ("due to a hostname lookup failure.\n")));
        }

      return -1;
    }

  return 0;
}

TAO_Transport *
TAO::IIOP_SSL_Connector::make_connection (
  TAO::Profile_Transport_Resolver *r,
  TAO_Transport_Descriptor_Interface &desc,
  ACE_Time_Value *max_wait_time)
{
  TAO_IIOP_Endpoint *iiop_endpoint =
    dynamic_cast<TAO_IIOP_Endpoint *> (desc.endpoint ());

  if (iiop_endpoint == 0)
    return 0;

  const ACE_INET_Addr &remote_address =
    iiop_endpoint->object_addr ();

  if (TAO_debug_level > 4)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) IIOP_SSL_Connector::connect ")
                ACE_TEXT ("making a new connection\n")));

  // Get the right synch options
  ACE_Synch_Options synch_options;

  this->active_connect_strategy_->synch_options (max_wait_time, synch_options);

  // If we don't need to block for a transport just set the timeout to
  // be zero.
  ACE_Time_Value tmp_zero (ACE_Time_Value::zero);
  if (!r->blocked_connect ())
    {
      synch_options.timeout (ACE_Time_Value::zero);
      max_wait_time = &tmp_zero;
    }


  IIOP_SSL_Connection_Handler *svc_handler = 0;

  // Connect.
  int result =
    this->base_connector_.connect (svc_handler, remote_address, synch_options);

  // Make sure that we always do a remove_reference
  ACE_Event_Handler_var svc_handler_auto_ptr (svc_handler);

  TAO_Transport *transport =
    svc_handler->transport ();

  if (result == -1)
    {
      // No immediate result, wait for completion
      if (errno == EWOULDBLOCK)
        {
          // Try to wait until connection completion. Incase we block, then we
          // get a connected transport or not. In case of non block we get
          // a connected or not connected transport
          if (!this->wait_for_connection_completion (r,
                                                     desc,
                                                     transport,
                                                     max_wait_time))
            {
              if (TAO_debug_level > 2)
                ACE_ERROR ((LM_ERROR, "TAO (%P|%t) - IIOP_SSL_Connector::"
                                      "make_connection, "
                                      "wait for completion failed\n"));
            }
        }
      else
        {
          // Transport is not usable
          transport = 0;
        }
    }

  // In case of errors transport is zero
  if (transport == 0)
    {
      // Give users a clue to the problem.
      if (TAO_debug_level)
        {
          ACE_DEBUG ((LM_ERROR,
                      "TAO (%P|%t) - IIOP_SSL_Connector::make_connection, "
                      "connection to <%s:%d> failed (%p)\n",
                      iiop_endpoint->host (), iiop_endpoint->port (),
                      "errno"));
        }

      return 0;
    }

  if (svc_handler->keep_waiting ())
    {
      svc_handler->connection_pending ();
    }

  // At this point, the connection has be successfully connected.
  // #REFCOUNT# is one.
  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                "TAO (%P|%t) - IIOP_SSL_Connector::make_connection, "
                "new connection to <%s:%d> on Transport[%d]\n",
                iiop_endpoint->host (), iiop_endpoint->port (),
                svc_handler->peer ().get_handle ()));

  // Add the handler to Cache
  int retval =
    this->orb_core ()->lane_resources ().transport_cache ().cache_transport (
      &desc,
      transport);

  // Failure in adding to cache.
  if (retval == -1)
    {
      // Close the handler.
      svc_handler->close ();

      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "TAO (%P|%t) - IIOP_SSL_Connector::make_connection, "
                      "could not add the new connection to cache\n"));
        }

      return 0;
    }

  if (transport->is_connected () &&
      transport->wait_strategy ()->register_handler () != 0)
    {
      // Registration failures.

      // Purge from the connection cache, if we are not in the cache, this
      // just does nothing.
      (void) transport->purge_entry ();

      // Close the handler.
      (void) transport->close_connection ();

      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "TAO (%P|%t) - IIOP_SSL_Connector [%d]::make_connection, "
                    "could not register the transport "
                    "in the reactor.\n",
                    transport->id ()));

      return 0;
    }

  svc_handler_auto_ptr.release ();
  return transport;
}

int
TAO::IIOP_SSL_Connector::cancel_svc_handler (
  TAO_Connection_Handler * svc_handler)
{
  IIOP_SSL_Connection_Handler* handler=
    dynamic_cast<IIOP_SSL_Connection_Handler*> (svc_handler);

  if (handler)
    // Cancel from the connector
    return this->base_connector_.cancel (handler);

  return -1;
}

TAO_END_VERSIONED_NAMESPACE_DECL
