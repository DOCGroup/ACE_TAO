#include "IIOP_SSL_Connector.h"

#include "SSLIOP_Util.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Environment.h"
#include "tao/IIOP_Endpoint.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Connect_Strategy.h"
#include "tao/Wait_Strategy.h"
#include "tao/Profile_Transport_Resolver.h"

#include "ace/Strategies_T.h"


ACE_RCSID (SSLIOP,
           IIOP_SSL_Connector,
           "$Id$")


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Connect_Concurrency_Strategy<TAO::IIOP_SSL_Connection_Handler>;
template class TAO_Connect_Creation_Strategy<TAO::IIOP_SSL_Connection_Handler>;
template class ACE_Strategy_Connector<TAO::IIOP_SSL_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Connect_Strategy<TAO::IIOP_SSL_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Connector_Base<TAO::IIOP_SSL_Connection_Handler>;
template class ACE_Connector<TAO::IIOP_SSL_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_NonBlocking_Connect_Handler<TAO::IIOP_SSL_Connection_Handler>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Connect_Concurrency_Strategy<TAO::IIOP_SSL_Connection_Handler>
#pragma instantiate TAO_Connect_Creation_Strategy<TAO::IIOP_SSL_Connection_Handler>
#pragma instantiate ACE_Strategy_Connector<TAO::IIOP_SSL_Connection_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Connect_Strategy<TAO::IIOP_SSL_Connection_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Connector_Base<TAO::IIOP_SSL_Connection_Handler>
#pragma instantiate ACE_Connector<TAO::IIOP_SSL_Connection_Handler, ACE_SOCK_Connector>
#pragma instantiate ACE_NonBlocking_Connect_Handler<TAO::IIOP_SSL_Connection_Handler>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


TAO::IIOP_SSL_Connector::IIOP_SSL_Connector (CORBA::Boolean flag)
  : TAO_IIOP_Connector (flag),
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

  if (this->init_tcp_properties () != 0)
    return -1;

  if (TAO::SSLIOP::Util::setup_handler_state (orb_core,
                                              &(this->tcp_properties_),
                                              this->handler_state_) != 0)
      return -1;

  // Our connect creation strategy
  CONNECT_CREATION_STRATEGY *connect_creation_strategy = 0;

  ACE_NEW_RETURN (connect_creation_strategy,
                  CONNECT_CREATION_STRATEGY (orb_core->thr_mgr (),
                                             orb_core,
                                             &(this->handler_state_),
                                             this->lite_flag_),
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

  const ACE_INET_Addr &remote_address =
    iiop_endpoint->object_addr ();

  // Verify that the remote ACE_INET_Addr was initialized properly.
  // Failure can occur if hostname lookup failed when initializing the
  // remote ACE_INET_Addr.
  if (remote_address.get_type () != AF_INET)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) IIOP_SSL connection failed.\n")
                      ACE_TEXT ("TAO (%P|%t) This is most likely ")
                      ACE_TEXT ("due to a hostname lookup ")
                      ACE_TEXT ("failure.\n")));
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
                ACE_TEXT ("making a new connection \n")));

  // Get the right synch options
  ACE_Synch_Options synch_options;

  this->active_connect_strategy_->synch_options (max_wait_time,
                                                 synch_options);

  // If we don't need to block for a transport just set the timeout to
  // be zero.
  ACE_Time_Value tmp_zero (ACE_Time_Value::zero);
  if (!r->blocked ())
    {
      synch_options.timeout (ACE_Time_Value::zero);
      max_wait_time = &tmp_zero;
    }


  IIOP_SSL_Connection_Handler *svc_handler = 0;

  // Connect.
  int result =
    this->base_connector_.connect (svc_handler,
                                   remote_address,
                                   synch_options);

  // The connect() method creates the service handler and bumps the
  // #REFCOUNT# up one extra.  There are three possibilities from
  // calling connect(): (a) connection succeeds immediately - in this
  // case, the #REFCOUNT# on the handler is two; (b) connection
  // completion is pending - in this case, the #REFCOUNT# on the
  // handler is also two; (c) connection fails immediately - in this
  // case, the #REFCOUNT# on the handler is one since close() gets
  // called on the handler.
  //
  // The extra reference count in
  // TAO_Connect_Creation_Strategy::make_svc_handler() is needed in
  // the case when connection completion is pending and we are going
  // to wait on a variable in the handler to changes, signifying
  // success or failure.  Note, that this increment cannot be done
  // once the connect() returns since this might be too late if
  // another thread pick up the completion and potentially deletes the
  // handler before we get a chance to increment the reference count.

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
  if (retval != 0)
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

  return transport;
}

int
TAO::IIOP_SSL_Connector::cancel_svc_handler (
  TAO_Connection_Handler * svc_handler)
{
  IIOP_SSL_Connection_Handler* handler=
    dynamic_cast<IIOP_SSL_Connection_Handler*>(svc_handler);

  if (handler)
    {
      // Cancel from the connector
      this->base_connector_.cancel (handler);

      return 0;
    }
  else
    {
      return -1;
    }
}
