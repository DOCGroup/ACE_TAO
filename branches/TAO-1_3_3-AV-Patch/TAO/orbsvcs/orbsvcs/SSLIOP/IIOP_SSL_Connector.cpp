// -*- C++ -*-
//
// $Id$

#include "IIOP_SSL_Connector.h"

#include "SSLIOP_Util.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Environment.h"
#include "tao/IIOP_Endpoint.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Invocation.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Connect_Strategy.h"
#include "tao/Wait_Strategy.h"

#include "ace/Strategies_T.h"


ACE_RCSID (TAO_SSLIOP,
           IIOP_SSL_Connector,
           "$Id$")


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Connect_Concurrency_Strategy<TAO_IIOP_SSL_Connection_Handler>;
template class TAO_Connect_Creation_Strategy<TAO_IIOP_SSL_Connection_Handler>;
template class ACE_Strategy_Connector<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Connect_Strategy<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Connector_Base<TAO_IIOP_SSL_Connection_Handler>;
template class ACE_Connector<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_NonBlocking_Connect_Handler<TAO_IIOP_SSL_Connection_Handler>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Connect_Concurrency_Strategy<TAO_IIOP_SSL_Connection_Handler>
#pragma instantiate TAO_Connect_Creation_Strategy<TAO_IIOP_SSL_Connection_Handler>
#pragma instantiate ACE_Strategy_Connector<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Connect_Strategy<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Connector_Base<TAO_IIOP_SSL_Connection_Handler>
#pragma instantiate ACE_Connector<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_Connector>
#pragma instantiate ACE_NonBlocking_Connect_Handler<TAO_IIOP_SSL_Connection_Handler>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


TAO_IIOP_SSL_Connector::TAO_IIOP_SSL_Connector (CORBA::Boolean flag)
  : TAO_IIOP_Connector (flag),
    connect_strategy_ (),
    base_connector_ ()
{
}

TAO_IIOP_SSL_Connector::~TAO_IIOP_SSL_Connector (void)
{
}

int
TAO_IIOP_SSL_Connector::open (TAO_ORB_Core *orb_core)
{
  this->orb_core (orb_core);

  // Create our connect strategy
  if (this->create_connect_strategy () == -1)
    return -1;

  if (this->init_tcp_properties () != 0)
    return -1;

  if (TAO_SSLIOP_Util::setup_handler_state (orb_core,
                                            &(this->tcp_properties_),
                                            this->handler_state_) != 0)
      return -1;

  /// Our connect creation strategy
  TAO_IIOP_SSL_CONNECT_CREATION_STRATEGY *connect_creation_strategy = 0;

  ACE_NEW_RETURN (connect_creation_strategy,
                  TAO_IIOP_SSL_CONNECT_CREATION_STRATEGY
                      (orb_core->thr_mgr (),
                       orb_core,
                       &(this->handler_state_),
                       this->lite_flag_),
                  -1);

  /// Our activation strategy
  TAO_IIOP_SSL_CONNECT_CONCURRENCY_STRATEGY *concurrency_strategy = 0;

  ACE_NEW_RETURN (concurrency_strategy,
                  TAO_IIOP_SSL_CONNECT_CONCURRENCY_STRATEGY (orb_core),
                  -1);


  return this->base_connector_.open (this->orb_core ()->reactor (),
                                     connect_creation_strategy,
                                     &this->connect_strategy_,
                                     concurrency_strategy);
}

int
TAO_IIOP_SSL_Connector::close (void)
{
  delete this->base_connector_.creation_strategy ();
  delete this->base_connector_.concurrency_strategy ();
  return this->base_connector_.close ();
}

int
TAO_IIOP_SSL_Connector::set_validate_endpoint (TAO_Endpoint *endpoint)
{
  if (endpoint->tag () != IOP::TAG_INTERNET_IOP)
    return -1;

  TAO_IIOP_Endpoint *iiop_endpoint =
    ACE_dynamic_cast (TAO_IIOP_Endpoint *,
                      endpoint);

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
                      ACE_TEXT ("TAO (%P|%t) IIOP connection failed.\n")
                      ACE_TEXT ("TAO (%P|%t) This is most likely ")
                      ACE_TEXT ("due to a hostname lookup ")
                      ACE_TEXT ("failure.\n")));
        }

      return -1;
    }

  return 0;
}

int
TAO_IIOP_SSL_Connector::make_connection (
  TAO_GIOP_Invocation *invocation,
  TAO_Transport_Descriptor_Interface *desc,
  ACE_Time_Value *max_wait_time)
{
  TAO_IIOP_Endpoint *iiop_endpoint =
    ACE_dynamic_cast (TAO_IIOP_Endpoint *,
                      desc->endpoint ());

  if (iiop_endpoint == 0)
    return -1;

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

  TAO_IIOP_SSL_Connection_Handler *svc_handler = 0;

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

  // No immediate result.  Wait for completion.
  if (result == -1 && errno == EWOULDBLOCK)
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - IIOP_SSL_Connector::make_connection(), "
                    "going to wait for connection completion on local"
                    "handle [%d]\n",
                    svc_handler->get_handle ()));

      // Wait for connection completion.  No need to specify timeout
      // to wait() since the correct timeout was passed to the
      // Connector. The Connector will close the handler in the case
      // of timeouts, so the event will complete (either success or
      // failure) within timeout.
      result =
        this->active_connect_strategy_->wait (svc_handler,
                                              0);

      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - IIOP_SSL_Connector::make_connection(), "
                      "wait done for handle[%d], result = %d\n",
                      svc_handler->get_handle (), result));
        }

      // There are three possibilities when wait() returns: (a)
      // connection succeeded; (b) connection failed; (c) wait()
      // failed because of some other error.  It is easy to deal with
      // (a) and (b).  (c) is tricky since the connection is still
      // pending and may get completed by some other thread.  The
      // following code deals with (c).

      // Check if the handler has been closed.
      int closed =
        svc_handler->is_closed ();

      // In case of failures and close() has not be called.
      if (result == -1 &&
          !closed)
        {
          // First, cancel from connector.
          this->base_connector_.cancel (svc_handler);

          // Double check to make sure the handler has not been closed
          // yet.  This double check is required to ensure that the
          // connection handler was not closed yet by some other
          // thread since it was still registered with the connector.
          // Once connector.cancel() has been processed, we are
          // assured that the connector will no longer open/close this
          // handler.
          closed =
            svc_handler->is_closed ();

          // If closed, there is nothing to do here.  If not closed,
          // it was either opened or is still pending.
          if (!closed)
            {
              // Check if the handler has been opened.
              int open =
                svc_handler->is_open ();

              // Some other thread was able to open the handler even
              // though wait failed for this thread.
              if (open)
                // Overwrite <result>.
                result = 0;
              else
                {
                  // Assert that it is still connecting.
                  ACE_ASSERT (svc_handler->is_connecting ());

                  // Force close the handler now.
                  svc_handler->close ();
                }
            }
        }
    }

  // Irrespective of success or failure, remove the extra #REFCOUNT#.
  svc_handler->remove_reference ();

  // In case of errors.
  if (result == -1)
    {
      // Give users a clue to the problem.
      if (TAO_debug_level)
        {
          ACE_DEBUG ((LM_ERROR,
                      "TAO (%P|%t) - IIOP_Connector::make_connection, "
                      "connection to <%s:%d> failed (%p)\n",
                      iiop_endpoint->host (), iiop_endpoint->port (),
                      "errno"));
        }

      return -1;
    }

  // At this point, the connection has be successfully connected.
  // #REFCOUNT# is one.
  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                "TAO (%P|%t) - IIOP_Connector::make_connection, "
                "new connection to <%s:%d> on Transport[%d]\n",
                iiop_endpoint->host (), iiop_endpoint->port (),
                svc_handler->peer ().get_handle ()));

  TAO_Transport *transport =
    svc_handler->transport ();

  // Add the handler to Cache
  int retval =
    this->orb_core ()->lane_resources ().transport_cache ().cache_transport (desc,
                                                                             transport);

  // Failure in adding to cache.
  if (retval != 0)
    {
      // Close the handler.
      svc_handler->close ();

      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "TAO (%P|%t) - IIOP_Connector::make_connection, "
                      "could not add the new connection to cache\n"));
        }

      return -1;
    }

  // If the wait strategy wants us to be registered with the reactor
  // then we do so. If registeration is required and it succeeds,
  // #REFCOUNT# becomes two.
  retval =  transport->wait_strategy ()->register_handler ();

  // Registration failures.
  if (retval != 0)
    {
      // Purge from the connection cache.
      transport->purge_entry ();

      // Close the handler.
      svc_handler->close ();

      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "TAO (%P|%t) - IIOP_Connector::make_connection, "
                      "could not register the new connection in the reactor\n"));
        }

      return -1;
    }

  // Handover the transport pointer to the Invocation class.
  TAO_Transport *&invocation_transport =
    invocation->transport ();
  invocation_transport = transport;

  return 0;
}
