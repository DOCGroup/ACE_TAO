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
template class ACE_Connector<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler>;

template class ACE_Map_Manager<int, ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler> *, TAO_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator_Base<int, ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler> *, TAO_SYNCH_RW_MUTEX>;
template class ACE_Map_Entry<int,ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler>*>;
template class ACE_Map_Iterator<int,ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler>*,TAO_SYNCH_RW_MUTEX>;
template class ACE_Map_Reverse_Iterator<int,ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler>*,TAO_SYNCH_RW_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Connect_Concurrency_Strategy<TAO_IIOP_SSL_Connection_Handler>
#pragma instantiate TAO_Connect_Creation_Strategy<TAO_IIOP_SSL_Connection_Handler>
#pragma instantiate ACE_Strategy_Connector<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Connect_Strategy<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Connector<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_Connector>
#pragma instantiate ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler>


#pragma instantiate ACE_Connector<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_Connector>
#pragma instantiate ACE_Creation_Strategy<TAO_IIOP_SSL_Connection_Handler>
#pragma instantiate ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler>
#pragma instantiate ACE_Map_Manager<int, ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler> *, TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<int, ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler> *, TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Entry<int,ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler>*>
#pragma instantiate ACE_Map_Iterator<int,ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler>*,TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<int,ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler>*,TAO_SYNCH_RW_MUTEX>

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
  if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Connector::connect - ")
                  ACE_TEXT ("looking for IIOP connection.\n")));



  TAO_IIOP_Endpoint *iiop_endpoint =
    ACE_dynamic_cast (TAO_IIOP_Endpoint *,
                      desc->endpoint ());

  if (TAO_debug_level > 4)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) IIOP_SSL_Connector::connect ")
                ACE_TEXT ("making a new connection \n")));

  const ACE_INET_Addr &remote_address =
    iiop_endpoint->object_addr ();

  // Get the right synch options
  ACE_Synch_Options synch_options;

  this->active_connect_strategy_->synch_options (max_wait_time,
                                                 synch_options);

  TAO_IIOP_SSL_Connection_Handler *svc_handler = 0;

   // Active connect
   int result = this->base_connector_.connect (svc_handler,
                                               remote_address,
                                               synch_options);

   if (result == -1 && errno == EWOULDBLOCK)
     {
       if (TAO_debug_level)
         ACE_DEBUG ((LM_DEBUG,
                     "TAO (%P|%t) - IIOP_SSL_Connector::make_connection(), "
                     "going to wait for connection completion on local"
                     "handle [%d]\n",
                        svc_handler->get_handle ()));
       result =
         this->active_connect_strategy_->wait (svc_handler,
                                               max_wait_time);

       if (TAO_debug_level > 2)
         {
           ACE_DEBUG ((LM_DEBUG,
                       "TAO (%P|%t) - IIOP_SSL_Connector::make_connection(), "
                       "wait done for handle[%d], result = %d\n",
                       svc_handler->get_handle (), result));
         }
     }

   int status =
     svc_handler->is_finalized ();

   // Reduce the refcount to the svc_handler that we have. The
   // increment to the handler is done in make_svc_handler (). Now
   // that we dont need the reference to it anymore we can decrement
   // the refcount whether the connection is successful ot not.
   long refcount = svc_handler->decr_refcount ();

   ACE_ASSERT (refcount >= 0);
   ACE_UNUSED_ARG (refcount);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) IIOP_SSL_Connector::connect ")
                ACE_TEXT ("The result is <%d> \n"), result));

  if (result == -1)
    {
      // Give users a clue to the problem.
      if (TAO_debug_level)
        {
          ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) %s:%u, connection to ")
                      ACE_TEXT ("%s:%d failed (%p)\n"),
                      __FILE__,
                      __LINE__,
                      iiop_endpoint->host (),
                      iiop_endpoint->port (),
                      ACE_TEXT ("errno")));
        }

      (void) this->active_connect_strategy_->post_failed_connect (svc_handler,
                                                                  status);

      return -1;
    }

  TAO_Transport *base_transport =
    TAO_Transport::_duplicate (svc_handler->transport ());

  // Add the handler to Cache.
  //
  // Note that the IIOP-only transport descriptor is used!
  int retval =
    this->orb_core ()->lane_resources ().transport_cache ().cache_transport (desc,
                                                                             base_transport);

  if (retval != 0 && TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) IIOP_SSL_Connector::connect ")
                  ACE_TEXT ("could not add the new connection to ")
                  ACE_TEXT ("Cache \n")));
    }

  // If the wait strategy wants us to be registered with the reactor
  // then we do so.
  retval =  base_transport->wait_strategy ()->register_handler ();

  if (retval != 0 && TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT ("(%P|%t) IIOP_Connector::connect ")
                  ACE_LIB_TEXT ("could not add the new connection to reactor \n")));
    }

  // Handover the transport pointer to the Invocation class.
  TAO_Transport *&transport = invocation->transport ();

  transport = base_transport;

  return 0;
}
