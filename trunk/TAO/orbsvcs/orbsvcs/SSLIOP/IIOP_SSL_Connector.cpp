// -*- C++ -*-
//
// $Id$

#include "IIOP_SSL_Connector.h"
#include "SSLIOP_Endpoint.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Environment.h"
#include "tao/Base_Connection_Property.h"
#include "tao/IIOP_Endpoint.h"

#include "ace/Strategies_T.h"

ACE_RCSID(TAO_SSLIOP, IIOP_SSL_Connector, "$Id$")


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

  if (this->init_tcp_properties () != 0)
    return -1;

  /// Our connect creation strategy
  TAO_IIOP_SSL_CONNECT_CREATION_STRATEGY *connect_creation_strategy = 0;

  ACE_NEW_RETURN (connect_creation_strategy,
                  TAO_IIOP_SSL_CONNECT_CREATION_STRATEGY
                      (orb_core->thr_mgr (),
                       orb_core,
                       &(this->tcp_properties_),
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
TAO_IIOP_SSL_Connector::connect (TAO_Connection_Descriptor_Interface *desc,
                                 TAO_Transport *&transport,
                                 ACE_Time_Value *max_wait_time,
                                 CORBA::Environment &)
{
  if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Connector::connect - ")
                  ACE_TEXT ("looking for IIOP connection.\n")));

  TAO_Endpoint *endpoint = desc->endpoint ();

  if (endpoint->tag () != TAO_TAG_IIOP_PROFILE)
    return -1;

  TAO_SSLIOP_Endpoint *ssliop_endpoint =
    ACE_dynamic_cast (TAO_SSLIOP_Endpoint *,
                      endpoint );

  if (ssliop_endpoint == 0)
    return -1;

  TAO_IIOP_Endpoint *iiop_endpoint = ssliop_endpoint->iiop_endpoint ();

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

  int result = 0;
  TAO_IIOP_SSL_Connection_Handler *svc_handler = 0;
  TAO_Connection_Handler *conn_handler = 0;

  // Check the Cache first for connections
  if (this->orb_core ()->connection_cache ().find_handler (desc,
                                                           conn_handler) == 0)
    {
      if (TAO_debug_level > 5)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) IIOP_SSL_Connector::connect ")
                    ACE_TEXT ("got an existing connection \n")));
      // We have found a connection and a handler
      svc_handler =
        ACE_dynamic_cast (TAO_IIOP_SSL_Connection_Handler *,
                          conn_handler);
    }
  else
    {
      if (TAO_debug_level > 4)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) IIOP_SSL_Connector::connect ")
                    ACE_TEXT ("making a new connection \n")));

      // @@ This needs to change in the next round when we implement a
      // policy that will not allow new connections when a connection
      // is busy.
      if (max_wait_time != 0)
        {
          ACE_Synch_Options synch_options (ACE_Synch_Options::USE_TIMEOUT,
                                           *max_wait_time);

          // We obtain the transport in the <svc_handler> variable. As
          // we know now that the connection is not available in Cache
          // we can make a new connection
          result = this->base_connector_.connect (svc_handler,
                                                  remote_address,
                                                  synch_options);
        }
      else
        {
          // We obtain the transport in the <svc_handler> variable. As
          // we know now that the connection is not available in Cache
          // we can make a new connection
          result = this->base_connector_.connect (svc_handler,
                                                  remote_address);
        }

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
          return -1;
        }

      // Add the handler to Cache
      int retval =
        this->orb_core ()->connection_cache ().cache_handler (desc,
                                                              svc_handler);

      if (retval != 0 && TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) IIOP_SSL_Connector::connect ")
                      ACE_TEXT ("could not add the new connection to ")
                      ACE_TEXT ("Cache \n")));
        }
    }

  transport = svc_handler->transport ();
  return 0;
}

int
TAO_IIOP_SSL_Connector::preconnect (const char *)
{
  // Deprecated.  Use _validate_connection() at run-time, instead.

  ACE_NOTSUP_RETURN (0);
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Connect_Concurrency_Strategy<TAO_IIOP_SSL_Connection_Handler>;
template class TAO_Connect_Creation_Strategy<TAO_IIOP_SSL_Connection_Handler>;
template class ACE_Strategy_Connector<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Connect_Strategy<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Connector<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler>;

template class ACE_Map_Manager<int, ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator_Base<int, ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Entry<int,ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler>*>;
template class ACE_Map_Iterator<int,ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler>*,ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Reverse_Iterator<int,ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler>*,ACE_SYNCH_RW_MUTEX>;

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
#pragma instantiate ACE_Map_Manager<int, ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<int, ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Entry<int,ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler>*>
#pragma instantiate ACE_Map_Iterator<int,ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler>*,ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<int,ACE_Svc_Tuple<TAO_IIOP_SSL_Connection_Handler>*,ACE_SYNCH_RW_MUTEX>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
