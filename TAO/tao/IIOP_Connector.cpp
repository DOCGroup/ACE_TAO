#include "IIOP_Connector.h"
#include "IIOP_Profile.h"
#include "debug.h"
#include "ORB_Core.h"
#include "Environment.h"
#include "Protocols_Hooks.h"
#include "Connect_Strategy.h"
#include "Thread_Lane_Resources.h"
#include "Profile_Transport_Resolver.h"
#include "Transport.h"
#include "Wait_Strategy.h"
#include "SystemException.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_string.h"


ACE_RCSID (tao,
           IIOP_Connector,
           "$Id$")


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_Connect_Concurrency_Strategy<TAO_IIOP_Connection_Handler>;
template class TAO_Connect_Creation_Strategy<TAO_IIOP_Connection_Handler>;
template class ACE_Strategy_Connector<TAO_IIOP_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Connect_Strategy<TAO_IIOP_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Connector_Base<TAO_IIOP_Connection_Handler>;
template class ACE_Connector<TAO_IIOP_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_NonBlocking_Connect_Handler<TAO_IIOP_Connection_Handler>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Connect_Concurrency_Strategy<TAO_IIOP_Connection_Handler>
#pragma instantiate TAO_Connect_Creation_Strategy<TAO_IIOP_Connection_Handler>
#pragma instantiate ACE_Strategy_Connector<TAO_IIOP_Connection_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Connect_Strategy<TAO_IIOP_Connection_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Connector_Base<TAO_IIOP_Connection_Handler>
#pragma instantiate ACE_Connector<TAO_IIOP_Connection_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_NonBlocking_Connect_Handler<TAO_IIOP_Connection_Handler>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

TAO_IIOP_Connector::TAO_IIOP_Connector (CORBA::Boolean flag)
  : TAO_Connector (IOP::TAG_INTERNET_IOP),
    lite_flag_ (flag),
    connect_strategy_ (),
    base_connector_ ()
{
}

TAO_IIOP_Connector::~TAO_IIOP_Connector (void)
{
}

int
TAO_IIOP_Connector::open (TAO_ORB_Core *orb_core)
{
  // @@todo: The functionality of the following two statements could
  // be done in the constructor, but that involves changing the
  // interface of the pluggable transport factory.

  // Set the ORB Core
  this->orb_core (orb_core);

  // Create our connect strategy
  if (this->create_connect_strategy () == -1)
    return -1;

  if (this->init_tcp_properties () != 0)
    return -1;

  /// Our connect creation strategy
  TAO_IIOP_CONNECT_CREATION_STRATEGY *connect_creation_strategy = 0;

  ACE_NEW_RETURN (connect_creation_strategy,
                  TAO_IIOP_CONNECT_CREATION_STRATEGY
                      (orb_core->thr_mgr (),
                       orb_core,
                       &(this->tcp_properties_),
                       this->lite_flag_),
                  -1);

  /// Our activation strategy
  TAO_IIOP_CONNECT_CONCURRENCY_STRATEGY *concurrency_strategy = 0;

  ACE_NEW_RETURN (concurrency_strategy,
                  TAO_IIOP_CONNECT_CONCURRENCY_STRATEGY (orb_core),
                  -1);

  return this->base_connector_.open (this->orb_core ()->reactor (),
                                     connect_creation_strategy,
                                     &this->connect_strategy_,
                                     concurrency_strategy);
}

int
TAO_IIOP_Connector::close (void)
{
  delete this->base_connector_.concurrency_strategy ();
  delete this->base_connector_.creation_strategy ();
  return this->base_connector_.close ();
}

int
TAO_IIOP_Connector::set_validate_endpoint (TAO_Endpoint *endpoint)
{
  TAO_IIOP_Endpoint *iiop_endpoint =
    this->remote_endpoint (endpoint);

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

TAO_Transport *
TAO_IIOP_Connector::make_connection (TAO::Profile_Transport_Resolver *r,
                                     TAO_Transport_Descriptor_Interface &desc,
                                     ACE_Time_Value *timeout)
{
  TAO_IIOP_Endpoint *iiop_endpoint =
    this->remote_endpoint (desc.endpoint ());

  if (iiop_endpoint == 0)
    return 0;

  const ACE_INET_Addr &remote_address =
    iiop_endpoint->object_addr ();

  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                "TAO (%P|%t) - IIOP_Connector::make_connection, "
                "to <%s:%d> which should %s\n",
                ACE_TEXT_CHAR_TO_TCHAR(iiop_endpoint->host()),
                iiop_endpoint->port(),
                r->blocked () ? ACE_TEXT("block") : ACE_TEXT("nonblock")));

  // Get the right synch options
  ACE_Synch_Options synch_options;

  this->active_connect_strategy_->synch_options (timeout,
                                                 synch_options);

  // If we don't need to block for a transport just set the timeout to
  // be zero.
  ACE_Time_Value tmp_zero (ACE_Time_Value::zero);
  if (!r->blocked ())
    {
      synch_options.timeout (ACE_Time_Value::zero);
      timeout = &tmp_zero;
    }

  TAO_IIOP_Connection_Handler *svc_handler = 0;

  // Connect.
  int result =
    this->base_connector_.connect (svc_handler,
                                   remote_address,
                                   synch_options);

  // The connect() method creates the service handler and bumps the
  // #REFCOUNT# up one extra.  There are four possibilities from
  // calling connect(): (a) connection succeeds immediately - in this
  // case, the #REFCOUNT# on the handler is two; (b) connection
  // completion is pending - in this case, the #REFCOUNT# on the
  // handler is also two; (c) connection fails immediately - in this
  // case, the #REFCOUNT# on the handler is one since close() gets
  // called on the handler; (d) the connect immediately returns when we
  // have specified that it shouldn't block.
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
                                                     timeout))
            {
              if (TAO_debug_level > 2)
                ACE_ERROR ((LM_ERROR, "TAO (%P|%t) - IIOP_Connector::"
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
      if (TAO_debug_level > 3)
          ACE_DEBUG ((LM_ERROR,
                      "TAO (%P|%t) - IIOP_Connector::make_connection, "
                      "connection to <%s:%d> failed (%p)\n",
                      iiop_endpoint->host (), iiop_endpoint->port (),
                      ACE_TEXT("errno")));

      return 0;
    }

  // At this point, the connection has be successfully created
  // connected or not connected, but we have a connection.
  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                "TAO (%P|%t) - IIOP_Connector::make_connection, "
                "new %s connection to <%s:%d> on Transport[%d]\n",
                transport->is_connected() ? "connected" : "not connected",
                iiop_endpoint->host (),
                iiop_endpoint->port (),
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
                      "TAO (%P|%t) - IIOP_Connector::make_connection, "
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
                    "TAO (%P|%t) - IIOP_Connector [%d]::make_connection, "
                    "could not register the transport "
                    "in the reactor.\n",
                    transport->id ()));

      return 0;
    }

  return transport;
}

TAO_Profile *
TAO_IIOP_Connector::create_profile (TAO_InputCDR& cdr)
{
  TAO_Profile *pfile = 0;
  ACE_NEW_RETURN (pfile,
                  TAO_IIOP_Profile (this->orb_core ()),
                  0);

  const int r = pfile->decode (cdr);
  if (r == -1)
    {
      pfile->_decr_refcnt ();
      pfile = 0;
    }

  return pfile;
}

TAO_Profile *
TAO_IIOP_Connector::make_profile (ACE_ENV_SINGLE_ARG_DECL)
{
  // The endpoint should be of the form:
  //    N.n@host:port/object_key
  // or:
  //    host:port/object_key

  TAO_Profile *profile = 0;
  ACE_NEW_THROW_EX (profile,
                    TAO_IIOP_Profile (this->orb_core ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  return profile;
}

int
TAO_IIOP_Connector::check_prefix (const char *endpoint)
{
  // Check for a valid string
  if (!endpoint || !*endpoint)
    return -1;  // Failure

  static const char *protocol[] = { "iiop", "iioploc" };

  const size_t slot = ACE_OS::strchr (endpoint, ':') - endpoint;
  if (slot == 0) // an empty string is valid for corbaloc.
    return 0;
                                                                                                                  
  const size_t len0 = ACE_OS::strlen (protocol[0]);
  const size_t len1 = ACE_OS::strlen (protocol[1]);

  // Check for the proper prefix in the IOR.  If the proper prefix
  // isn't in the IOR then it is not an IOR we can use.
  if (slot == len0
      && ACE_OS::strncasecmp (endpoint, protocol[0], len0) == 0)
    return 0;
  else if (slot == len1
           && ACE_OS::strncasecmp (endpoint, protocol[1], len1) == 0)
    return 0;

  return -1;
  // Failure: not an IIOP IOR
  // DO NOT throw an exception here.
}

char
TAO_IIOP_Connector::object_key_delimiter (void) const
{
  return TAO_IIOP_Profile::object_key_delimiter_;
}

int
TAO_IIOP_Connector::init_tcp_properties (void)
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

  // Initialize the settings to the ORB defaults.  If RT CORBA is enabled,
  // it may override these.
  int send_buffer_size = this->orb_core ()->orb_params ()->sock_sndbuf_size ();
  int recv_buffer_size = this->orb_core ()->orb_params ()->sock_rcvbuf_size ();
  int no_delay = this->orb_core ()->orb_params ()->nodelay ();
  int enable_network_priority = 0;

  TAO_Protocols_Hooks *tph =
    this->orb_core ()->get_protocols_hooks (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (tph != 0)
    {
      static const char protocol[] = "iiop";
      const char *protocol_type = protocol;

      const int hook_result =
        tph->call_client_protocols_hook (send_buffer_size,
                                         recv_buffer_size,
                                         no_delay,
                                         enable_network_priority,
                                         protocol_type);

      if (hook_result == -1)
        return -1;
    }

  // Extract and locally store properties of interest.
  this->tcp_properties_.send_buffer_size =
    send_buffer_size;
  this->tcp_properties_.recv_buffer_size =
    recv_buffer_size;
  this->tcp_properties_.no_delay =
    no_delay;
  this->tcp_properties_.enable_network_priority  =
    enable_network_priority;

  return 0;
}


TAO_IIOP_Endpoint *
TAO_IIOP_Connector::remote_endpoint (TAO_Endpoint *endpoint)
{
  if (endpoint->tag () != IOP::TAG_INTERNET_IOP)
    return 0;

  TAO_IIOP_Endpoint *iiop_endpoint =
    dynamic_cast<TAO_IIOP_Endpoint *> (endpoint );

  if (iiop_endpoint == 0)
    return 0;

  return iiop_endpoint;
}

int
TAO_IIOP_Connector::cancel_svc_handler (
  TAO_Connection_Handler * svc_handler)
{
  TAO_IIOP_Connection_Handler* handler=
    dynamic_cast<TAO_IIOP_Connection_Handler*>(svc_handler);

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
