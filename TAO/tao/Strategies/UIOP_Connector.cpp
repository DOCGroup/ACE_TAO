#include "UIOP_Connector.h"

#if TAO_HAS_UIOP == 1

#include "UIOP_Profile.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Environment.h"
#include "tao/Protocols_Hooks.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Invocation.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Connect_Strategy.h"
ACE_RCSID(Strategies,
          UIOP_Connector,
          "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Connect_Concurrency_Strategy<TAO_UIOP_Connection_Handler>;
template class TAO_Connect_Creation_Strategy<TAO_UIOP_Connection_Handler>;
template class ACE_Strategy_Connector<TAO_UIOP_Connection_Handler, ACE_LSOCK_CONNECTOR>;
template class ACE_Connect_Strategy<TAO_UIOP_Connection_Handler, ACE_LSOCK_CONNECTOR>;
template class ACE_Connector<TAO_UIOP_Connection_Handler, ACE_LSOCK_CONNECTOR>;
template class ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>;

template class ACE_Map_Manager<int, ACE_Svc_Tuple<TAO_UIOP_Connection_Handler> *, TAO_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator_Base<int, ACE_Svc_Tuple<TAO_UIOP_Connection_Handler> *, TAO_SYNCH_RW_MUTEX>;
template class ACE_Map_Entry<int,ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>*>;
template class ACE_Map_Iterator<int,ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>*,TAO_SYNCH_RW_MUTEX>;
template class ACE_Map_Reverse_Iterator<int,ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>*,TAO_SYNCH_RW_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<ACE_UNIX_Addr>
#pragma instantiate ACE_Unbounded_Stack<ACE_UNIX_Addr>

#pragma instantiate TAO_Connect_Concurrency_Strategy<TAO_UIOP_Connection_Handler>
#pragma instantiate TAO_Connect_Creation_Strategy<TAO_UIOP_Connection_Handler>
#pragma instantiate ACE_Strategy_Connector<TAO_UIOP_Connection_Handler, ACE_LSOCK_CONNECTOR>
#pragma instantiate ACE_Connect_Strategy<TAO_UIOP_Connection_Handler, ACE_LSOCK_CONNECTOR>
#pragma instantiate ACE_Connector<TAO_UIOP_Connection_Handler, ACE_LSOCK_CONNECTOR>
#pragma instantiate ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>

#pragma instantiate ACE_Map_Manager<int, ACE_Svc_Tuple<TAO_UIOP_Connection_Handler> *, TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<int, ACE_Svc_Tuple<TAO_UIOP_Connection_Handler> *, TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Entry<int,ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>*>
#pragma instantiate ACE_Map_Iterator<int,ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>*,TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<int,ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>*,TAO_SYNCH_RW_MUTEX>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

TAO_UIOP_Connector::TAO_UIOP_Connector (CORBA::Boolean flag)
  : TAO_Connector (TAO_TAG_UIOP_PROFILE),
    connect_strategy_ (),
    base_connector_ (),
    lite_flag_ (flag)
{
}

TAO_UIOP_Connector::~TAO_UIOP_Connector (void)
{
}

int
TAO_UIOP_Connector::open (TAO_ORB_Core *orb_core)
{
  this->orb_core (orb_core);

  // Create our connect strategy
  if (this->create_connect_strategy () == -1)
    return -1;

  if (this->init_uiop_properties () != 0)
    return -1;

  // Our connect creation strategy
  TAO_UIOP_CONNECT_CREATION_STRATEGY *connect_creation_strategy = 0;

  ACE_NEW_RETURN (connect_creation_strategy,
                  TAO_UIOP_CONNECT_CREATION_STRATEGY
                      (orb_core->thr_mgr (),
                       orb_core,
                       &(this->uiop_properties_),
                       this->lite_flag_),
                  -1);

  /// Our activation strategy
  TAO_UIOP_CONNECT_CONCURRENCY_STRATEGY *concurrency_strategy = 0;

  ACE_NEW_RETURN (concurrency_strategy,
                  TAO_UIOP_CONNECT_CONCURRENCY_STRATEGY (orb_core),
                  -1);

  return this->base_connector_.open (this->orb_core ()->reactor (),
                                     connect_creation_strategy,
                                     &this->connect_strategy_,
                                     concurrency_strategy);
}

int
TAO_UIOP_Connector::close (void)
{
  // Zap the creation strategy that we created earlier.
  delete this->base_connector_.creation_strategy ();
  delete this->base_connector_.concurrency_strategy ();

  return this->base_connector_.close ();
}


int
TAO_UIOP_Connector::set_validate_endpoint (TAO_Endpoint *endpoint)
{
  TAO_UIOP_Endpoint *uiop_endpoint =
    this->remote_endpoint (endpoint);

  if (uiop_endpoint == 0)
    return -1;

   const ACE_UNIX_Addr &remote_address =
     uiop_endpoint->object_addr ();

   // @@ Note, POSIX.1g renames AF_UNIX to AF_LOCAL.
   // Verify that the remote ACE_UNIX_Addr was initialized properly.
   // Failure can occur if hostname lookup failed when initializing the
   // remote ACE_INET_Addr.
   if (remote_address.get_type () != AF_UNIX)
     {
       if (TAO_debug_level > 0)
         {
           ACE_DEBUG ((LM_DEBUG,
                       ACE_LIB_TEXT ("TAO (%P|%t) UIOP failure.\n")
                       ACE_LIB_TEXT ("TAO (%P|%t) This is most likely ")
                       ACE_LIB_TEXT ("due to a hostname lookup ")
                       ACE_LIB_TEXT ("failure.\n")));
         }

       return -1;
     }

   return 0;
}

int
TAO_UIOP_Connector::make_connection (TAO_GIOP_Invocation *invocation,
                                     TAO_Transport_Descriptor_Interface *desc,
                                     ACE_Time_Value *max_wait_time)
{
  if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Connector::connect - ")
                  ACE_TEXT ("looking for UIOP connection.\n")));

  TAO_UIOP_Endpoint *uiop_endpoint =
    this->remote_endpoint (desc->endpoint ());

  if (uiop_endpoint == 0)
    return -1;

  const ACE_UNIX_Addr &remote_address =
    uiop_endpoint->object_addr ();

  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) UIOP_Connector::connect ")
                ACE_TEXT ("making a new connection \n")));

  ACE_Synch_Options synch_options;

  this->active_connect_strategy_->synch_options (max_wait_time,
                                                 synch_options);

  TAO_UIOP_Connection_Handler *svc_handler = 0;

  int result =
    this->base_connector_.connect (svc_handler,
                                   remote_address,
                                   synch_options);

  if (result == -1 && errno == EWOULDBLOCK)
    {
      if (TAO_debug_level > 2)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - UIOP_Connector::make_connection, "
                    "going to wait for connection completion on local"
                    "handle [%d]\n",
                    svc_handler->get_handle ()));
      result =
        this->active_connect_strategy_->wait (svc_handler,
                                              max_wait_time);

      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - UIOP_Connector::make_connection"
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
                ACE_TEXT ("(%P|%t) UIOP_Connector::connect ")
                ACE_TEXT ("The result is <%d> \n"), result));

  if (result == -1)
    {
      // Give users a clue to the problem.
      if (TAO_debug_level)
        {
          ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) %s:%u, connection to ")
                      ACE_TEXT ("%s failed (%p)\n"),
                      __FILE__,
                      __LINE__,
                      uiop_endpoint->rendezvous_point (),
                      ACE_TEXT ("errno")));
        }

      (void) this->active_connect_strategy_->post_failed_connect (svc_handler,
                                                                  status);
      return -1;
    }

  TAO_Transport *base_transport =
    TAO_Transport::_duplicate (svc_handler->transport ());

  // Add the handler to Cache
  int retval =
    this->orb_core ()->lane_resources ().transport_cache ().cache_transport (desc,
                                                                             base_transport);

  if (retval != 0 && TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) UIOP_Connector::connect ")
                  ACE_TEXT ("could not add the new connection to Cache \n")));
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



TAO_Profile *
TAO_UIOP_Connector::create_profile (TAO_InputCDR& cdr)
{
  TAO_Profile *pfile;
  ACE_NEW_RETURN (pfile,
                  TAO_UIOP_Profile (this->orb_core ()),
                  0);

  int r = pfile->decode (cdr);
  if (r == -1)
    {
      pfile->_decr_refcnt ();
      pfile = 0;
    }

  return pfile;
}

TAO_Profile *
TAO_UIOP_Connector::make_profile (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Profile *profile = 0;
  ACE_NEW_THROW_EX (profile,
                    TAO_UIOP_Profile (this->orb_core ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  ACE_CHECK_RETURN (0);

  return profile;
}

int
TAO_UIOP_Connector::check_prefix (const char *endpoint)
{
  // Check for a valid string
  if (!endpoint || !*endpoint)
    return -1;  // Failure

  const char *protocol[] = { "uiop", "uioploc" };

  size_t slot = ACE_OS::strchr (endpoint, ':') - endpoint;

  size_t len0 = ACE_OS::strlen (protocol[0]);
  size_t len1 = ACE_OS::strlen (protocol[1]);

  // Check for the proper prefix in the IOR.  If the proper prefix
  // isn't in the IOR then it is not an IOR we can use.
  if (slot == len0
      && ACE_OS::strncasecmp (endpoint,
                              protocol[0],
                              len0) == 0)
    return 0;
  else if (slot == len1
           && ACE_OS::strncasecmp (endpoint,
                                   protocol[1],
                                   len1) == 0)
    return 0;

  return -1;
  // Failure: not an UIOP IOR DO NOT throw an exception here.
}

char
TAO_UIOP_Connector::object_key_delimiter (void) const
{
  return TAO_UIOP_Profile::object_key_delimiter_;
}

int
TAO_UIOP_Connector::init_uiop_properties (void)
{
  // Connector protocol properties are obtained from ORB-level
  // RTCORBA::ClientProtocolProperties policy override.
  // If the override doesn't exist or doesn't contain the
  // properties, we use ORB default.
  //
  // Currently, we do not use Object-level and Current-level policy
  // overrides for protocol configuration because connection
  // lookup and caching are not done based on protocol
  // properties.

  ACE_DECLARE_NEW_CORBA_ENV;

  int send_buffer_size = this->orb_core ()->orb_params ()->sock_sndbuf_size ();
  int recv_buffer_size = this->orb_core ()->orb_params ()->sock_rcvbuf_size ();
  int no_delay = 0;
  int enable_network_priority = 0;

  TAO_Protocols_Hooks *tph = this->orb_core ()->get_protocols_hooks (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (tph != 0)
    {
      const char protocol [] = "uiop";
      const char *protocol_type = protocol;
      int hook_result =
        tph->call_client_protocols_hook (send_buffer_size,
                                         recv_buffer_size,
                                         no_delay,
                                         enable_network_priority,
                                         protocol_type);

      if(hook_result == -1)
        return -1;
    }

    // Extract and locally store properties of interest.
    this->uiop_properties_.send_buffer_size =
      send_buffer_size;
    this->uiop_properties_.recv_buffer_size =
      recv_buffer_size;

  return 0;
}

TAO_UIOP_Endpoint *
TAO_UIOP_Connector::remote_endpoint (TAO_Endpoint *endpoint)
{
  if (endpoint->tag () != TAO_TAG_UIOP_PROFILE)
    return 0;

  TAO_UIOP_Endpoint *uiop_endpoint =
    ACE_dynamic_cast (TAO_UIOP_Endpoint *,
                      endpoint );
  if (uiop_endpoint == 0)
    return 0;

  return uiop_endpoint;
}

#endif /* TAO_HAS_UIOP == 1 */
