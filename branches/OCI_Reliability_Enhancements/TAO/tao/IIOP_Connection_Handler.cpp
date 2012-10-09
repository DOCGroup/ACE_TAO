// -*- C++ -*-
// $Id$

#include "tao/IIOP_Connection_Handler.h"

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/IIOP_Transport.h"
#include "tao/IIOP_Endpoint.h"
#include "tao/IIOPC.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Protocols_Hooks.h"
#include "tao/Wait_Strategy.h"

#include "ace/os_include/netinet/os_tcp.h"
#include "ace/os_include/os_netdb.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


#ifdef TAO_LOG_CH_REF_COUNTS
ACE_Event_Handler::Reference_Count
TAO_IIOP_Connection_Handler::add_reference (void)
{
  Reference_Count rc = TAO_IIOP_SVC_HANDLER::add_reference ();
  if (TAO_debug_level > 9)
    {
      TAO_Transport *tport = this->transport ();
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - IIOP_Connection_Handler[%d]::")
                  ACE_TEXT("add_reference, up to %d\n"),
                  tport != 0 ? tport->id () : 0,
                  rc));
    }
  return rc;

}

ACE_Event_Handler::Reference_Count
TAO_IIOP_Connection_Handler::remove_reference (void)
{
  TAO_Transport *tport = this->transport ();
  Reference_Count rc = TAO_IIOP_SVC_HANDLER::remove_reference ();
  if (TAO_debug_level > 9)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - IIOP_Connection_Handler[%d]::")
                  ACE_TEXT("remove_reference, down to %d\n"),
                  tport != 0 ? tport->id () : 0,
                  rc));
    }
  return rc;
}
#endif /*TAO_LOG_CH_REF_COUNTS*/


TAO_IIOP_Connection_Handler::TAO_IIOP_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_IIOP_SVC_HANDLER (t, 0 , 0),
    TAO_Connection_Handler (0),
    dscp_codepoint_ (IPDSFIELD_DSCP_DEFAULT << 2)
{
  // This constructor should *never* get called, it is just here to
  // make the compiler happy: the default implementation of the
  // Creation_Strategy requires a constructor with that signature, we
  // don't use that implementation, but some (most?) compilers
  // instantiate it anyway.
  ACE_ASSERT (0);
}


TAO_IIOP_Connection_Handler::TAO_IIOP_Connection_Handler (
  TAO_ORB_Core *orb_core)
  : TAO_IIOP_SVC_HANDLER (orb_core->thr_mgr (), 0, 0),
    TAO_Connection_Handler (orb_core),
    dscp_codepoint_ (IPDSFIELD_DSCP_DEFAULT << 2)
{
  TAO_IIOP_Transport* specific_transport = 0;
  ACE_NEW (specific_transport,
           TAO_IIOP_Transport (this, orb_core));

  if (TAO_debug_level > 9)
    {
      TAO_Transport *tport = static_cast<TAO_Transport *> (specific_transport);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - IIOP_Connection_Handler[%d]::")
                  ACE_TEXT("IIOP_Connection_Handler, ")
                  ACE_TEXT("this=%@\n"),
                  tport != 0 ? tport->id () : 0,
                  this));
    }

  // store this pointer (indirectly increment ref count)
  this->transport (specific_transport);
}

TAO_IIOP_Connection_Handler::~TAO_IIOP_Connection_Handler (void)
{
  if (TAO_debug_level > 9)
    {
      TAO_Transport *tport = this->transport ();
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - IIOP_Connection_Handler[%d]::")
                  ACE_TEXT("~IIOP_Connection_Handler, ")
                  ACE_TEXT("this=%@, transport=%@\n"),
                  tport != 0 ? tport->id () : 0,
                  this,
                  tport));
    }
  delete this->transport ();
  int const result =
    this->release_os_resources ();

  if (result == -1 && TAO_debug_level)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("TAO (%P|%t) - IIOP_Connection_Handler::")
                  ACE_TEXT("~IIOP_Connection_Handler, ")
                  ACE_TEXT("release_os_resources() failed %m\n")));
    }
}

/* Copy hook that copies over the concrete methods from this class
 * to the base Connection_Handler class as a part of the specialization
 * process. Add all concrete and virtual
 * methods implemented in this class within this hook.
 */
//@@ CONNECTION_HANDLER_SPL_COPY_HOOK_START

int
TAO_IIOP_Connection_Handler::open_handler (void *v)
{
  return this->open (v);
}

int
TAO_IIOP_Connection_Handler::open (void*)
{
  if (this->shared_open() == -1)
    return -1;

  TAO_IIOP_Protocol_Properties protocol_properties;

  // Initialize values from ORB params.
  protocol_properties.send_buffer_size_ =
    this->orb_core ()->orb_params ()->sock_sndbuf_size ();
  protocol_properties.recv_buffer_size_ =
    this->orb_core ()->orb_params ()->sock_rcvbuf_size ();
  protocol_properties.no_delay_ =
    this->orb_core ()->orb_params ()->nodelay ();
  protocol_properties.keep_alive_ =
    this->orb_core ()->orb_params ()->sock_keepalive ();
  protocol_properties.dont_route_ =
   this->orb_core ()->orb_params ()->sock_dontroute ();
  protocol_properties.hop_limit_ =
   this->orb_core ()->orb_params ()->ip_hoplimit ();

  TAO_Protocols_Hooks *tph = this->orb_core ()->get_protocols_hooks ();

  if (tph != 0)
    {
      try
        {
          if (this->transport ()->opened_as () == TAO::TAO_CLIENT_ROLE)
            {
              tph->client_protocol_properties_at_orb_level (protocol_properties);
            }
          else
            {
              tph->server_protocol_properties_at_orb_level (protocol_properties);
            }
        }
      catch (const ::CORBA::Exception&)
        {
          return -1;
        }
    }

  if (this->set_socket_option (this->peer (),
                               protocol_properties.send_buffer_size_,
                               protocol_properties.recv_buffer_size_) == -1)
    {
      return -1;
    }

#if !defined (ACE_LACKS_TCP_NODELAY)
  if (this->peer ().set_option (ACE_IPPROTO_TCP,
                                TCP_NODELAY,
                                (void *) &protocol_properties.no_delay_,
                                sizeof (protocol_properties.no_delay_)) == -1)
    return -1;
#endif /* ! ACE_LACKS_TCP_NODELAY */

  if (protocol_properties.keep_alive_)
    {
      if (this->peer ().
          set_option (SOL_SOCKET,
                      SO_KEEPALIVE,
                      (void *) &protocol_properties.keep_alive_,
                      sizeof (protocol_properties.keep_alive_)) == -1
          && errno != ENOTSUP)
        {
          return -1;
        }
    }

#if !defined (ACE_LACKS_SO_DONTROUTE)
  if (protocol_properties.dont_route_)
    {
      if (this->peer ().
          set_option (SOL_SOCKET,
                      SO_DONTROUTE,
                      (void *) &protocol_properties.dont_route_,
                      sizeof (protocol_properties.dont_route_)) == -1
          && errno != ENOTSUP)
        {
          return -1;
        }
    }
#endif /* ! ACE_LACKS_SO_DONTROUTE */

  if (protocol_properties.hop_limit_ >= 0)
    {
      int result = 0;
#if defined (ACE_HAS_IPV6)
      ACE_INET_Addr local_addr;
      if (this->peer ().get_local_addr (local_addr) == -1)
        {
          result = -1;
        }
      else if (local_addr.get_type () == AF_INET6)
        {
#if defined (ACE_WIN32)
          DWORD hop_limit =
            static_cast<DWORD> (protocol_properties.hop_limit_);
#else
          int hop_limit =
            static_cast<int> (protocol_properties.hop_limit_);
#endif
          result = this->peer ().set_option (
            IPPROTO_IPV6,
            IPV6_UNICAST_HOPS,
            (void *) &hop_limit,
            sizeof (hop_limit));
        }
      else
#endif /* ACE_HAS_IPV6 */
        {
#if defined (ACE_WIN32)
          DWORD hop_limit =
            static_cast<DWORD> (protocol_properties.hop_limit_);
#else
          int hop_limit =
            static_cast<int> (protocol_properties.hop_limit_);
#endif
          result = this->peer ().set_option (
            IPPROTO_IP,
            IP_TTL,
            (void *) &hop_limit,
            sizeof (hop_limit));
        }

      if (result != 0)
        {
          if (TAO_debug_level)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT("TAO (%P|%t) - IIOP_Connection_Handler::open, ")
                          ACE_TEXT("couldn't set hop limit\n\n")));
            }
          return -1;
        }
    }

  if (this->transport ()->wait_strategy ()->non_blocking ()
      || this->transport ()->opened_as () == TAO::TAO_SERVER_ROLE)
    {
      if (this->peer ().enable (ACE_NONBLOCK) == -1)
        return -1;
    }

  // Called by the <Strategy_Acceptor> when the handler is
  // completely connected.

  ACE_INET_Addr remote_addr;
  if (this->peer ().get_remote_addr (remote_addr) == -1)
    return -1;

  ACE_INET_Addr local_addr;
  if (this->peer ().get_local_addr (local_addr) == -1)
    return -1;

  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("TAO (%P|%t) - IIOP_Connection_Handler::open, ")
                ACE_TEXT("The local addr is <%C:%d>\n"),
                local_addr.get_host_addr (),
                local_addr.get_port_number()));

  if (local_addr == remote_addr)
    {
      if (TAO_debug_level > 0)
        {
          ACE_TCHAR remote_as_string[MAXHOSTNAMELEN + 16];
          ACE_TCHAR local_as_string[MAXHOSTNAMELEN + 16];

          (void) remote_addr.addr_to_string (remote_as_string,
                                             sizeof(remote_as_string));
          (void) local_addr.addr_to_string (local_as_string,
                                            sizeof(local_as_string));
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT("TAO(%P|%t) - IIOP_Connection_Handler::open, ")
                      ACE_TEXT("Holy Cow! The remote addr and ")
                      ACE_TEXT("local addr are identical (%s == %s)\n"),
                      remote_as_string, local_as_string));
        }
      return -1;
    }

#if defined (ACE_HAS_IPV6) && !defined (ACE_HAS_IPV6_V6ONLY)
  // Check if we need to invalidate accepted connections
  // from IPv4 mapped IPv6 addresses
  if (this->orb_core ()->orb_params ()->connect_ipv6_only () &&
      remote_addr.is_ipv4_mapped_ipv6 ())
    {
      if (TAO_debug_level > 0)
        {
          ACE_TCHAR remote_as_string[MAXHOSTNAMELEN + 16];

          (void) remote_addr.addr_to_string (remote_as_string,
                                             sizeof(remote_as_string));

          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT("TAO (%P|%t) - IIOP_Connection_Handler::open, ")
                      ACE_TEXT("invalid connection from IPv4 mapped IPv6 interface <%s>!\n"),
                      remote_as_string));
        }
      return -1;
    }
#endif /* ACE_HAS_IPV6 && ACE_HAS_IPV6_V6ONLY */

  if (TAO_debug_level > 0)
    {
      ACE_TCHAR client_addr[MAXHOSTNAMELEN + 16];

      // Verify that we can resolve the peer hostname.
      if (remote_addr.addr_to_string (client_addr, sizeof (client_addr)) == -1)
        return -1;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - IIOP_Connection_Handler::open, IIOP ")
                  ACE_TEXT ("connection to peer <%s> on %d\n"),
                  client_addr, this->peer ().get_handle ()));
    }

  // Set that the transport is now connected, if fails we return -1
  // Use C-style cast b/c otherwise we get warnings on lots of
  // compilers
  if (!this->transport ()->post_open ((size_t) this->get_handle ()))
    return -1;
  this->state_changed (TAO_LF_Event::LFS_SUCCESS,
                       this->orb_core ()->leader_follower ());

  return 0;
}

int
TAO_IIOP_Connection_Handler::resume_handler (void)
{
  return ACE_Event_Handler::ACE_APPLICATION_RESUMES_HANDLER;
}

int
TAO_IIOP_Connection_Handler::close_connection (void)
{
  // To maintain maximum compatibility, we only set this socket option
  // if the user has provided a linger timeout.
  int const linger = this->orb_core()->orb_params()->linger ();
  if (linger != -1)
    {
      struct linger lval;
      lval.l_onoff = 1;
      lval.l_linger = (u_short)linger;
      if (this->peer ().set_option(SOL_SOCKET,
                                   SO_LINGER,
                                   (void*) &lval,
                                   sizeof (lval)) == -1)
        {
          if (TAO_debug_level)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) Unable to set ")
                          ACE_TEXT ("SO_LINGER on %d\n"),
                          this->peer ().get_handle ()));
            }
        }
    }

  return this->close_connection_eh (this);
}

int
TAO_IIOP_Connection_Handler::handle_input (ACE_HANDLE h)
{
  return this->handle_input_eh (h, this);
}

int
TAO_IIOP_Connection_Handler::handle_output (ACE_HANDLE handle)
{
  int const result = this->handle_output_eh (handle, this);

  if (result == -1)
    {
      this->close_connection ();
      return 0;
    }

  return result;
}


int
TAO_IIOP_Connection_Handler::handle_timeout (const ACE_Time_Value &,
                                             const void *)
{
  // Using this to ensure this instance will be deleted (if necessary)
  // only after reset_state(). Without this, when this refcount==1 -
  // the call to close() will cause a call to remove_reference() which
  // will delete this. At that point this->reset_state() is in no
  // man's territory and that causes SEGV on some platforms (Windows!)

  TAO_Auto_Reference<TAO_IIOP_Connection_Handler> safeguard (*this);

  // NOTE: Perhaps not the best solution, as it feels like the upper
  // layers should be responsible for this?

  // We don't use this upcall for I/O.  This is only used by the
  // Connector to indicate that the connection timedout.  Therefore,
  // we should call close()

  int const ret = this->close ();
  this->reset_state (TAO_LF_Event::LFS_TIMEOUT);
  if (TAO_debug_level > 9)
    {
      ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - TAO_IIOP_Connection_Handler[%d]::"
                  "handle_timeout reset state to LFS_TIMEOUT\n",
                  this->transport ()-> id()));
    }
  return ret;
}

int
TAO_IIOP_Connection_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_ASSERT (0);
  return 0;
}

int
TAO_IIOP_Connection_Handler::close (u_long flags)
{
  return this->close_handler (flags);
}

int
TAO_IIOP_Connection_Handler::release_os_resources (void)
{
  return this->peer ().close ();
}

int
TAO_IIOP_Connection_Handler::add_transport_to_cache (void)
{
  ACE_INET_Addr addr;

  // Get the peername.
  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  // Construct an  IIOP_Endpoint object
  TAO_IIOP_Endpoint endpoint (
      addr,
      this->orb_core()->orb_params()->cache_incoming_by_dotted_decimal_address ());

  // Construct a property object
  TAO_Base_Transport_Property prop (&endpoint);

  TAO::Transport_Cache_Manager &cache =
    this->orb_core ()->lane_resources ().transport_cache ();

  // Idle the transport..
  return cache.cache_transport (&prop, this->transport ());
}

int
TAO_IIOP_Connection_Handler::process_listen_point_list (
    IIOP::ListenPointList &listen_list)
{
  // Get the size of the list
  CORBA::ULong len = listen_list.length ();

  if (TAO_debug_level > 0 && len == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("TAO (%P|%t) - IIOP_Connection_Handler::")
                  ACE_TEXT("process_listen_point_list, ")
                  ACE_TEXT("Received list of size 0, check client config.\n")));
    }

  // @@ We can only handle a single endpoint for now because the
  // transport is recached for each endpoint, loosing older
  // information. This means that when more than one listen_point is
  // sent, the cache will end up only being associated with the last
  // address.  This poses a problem at invocation time because the
  // default endpoint selector steps through the profiles/endpoints
  // sequentially and will try and possibly succeed in connecting to
  // one of the earlier endpoints.  My assumption is that when this
  // method is called, it is before the first attempt to connect to
  // any object in the target process, thus the first listen point in
  // the list will correspond with the first profile and thus that
  // should match the cache.
  //
  // Probably what needs to be done to make this model work well is to
  // allow the transport cache to have multiple keys reference the
  // same transport so that rather than recaching, we simply add a
  // key.
  len = 1;
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      IIOP::ListenPoint listen_point = listen_list[i];

      // since the supplied host/port could be unresolvable, the assigning
      // constructor of the INET addr should not be as it will emit an error
      // if the underlying set fails. An unresolvable address in this case
      // is OK, as it will only be used to find an already cached transport.
      ACE_INET_Addr addr;
      (void)addr.set(listen_point.port, listen_point.host.in ());

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - IIOP_Connection_Handler::")
                      ACE_TEXT("process_listen_point_list, ")
                      ACE_TEXT("Listening port [%d] on [%C]\n"),
                      listen_point.port,
                      listen_point.host.in ()));
        }

      // Construct an  IIOP_Endpoint object using the host as provided
      // in the listen point list.  We must use host in that form because
      // that's also how the ORB on the other side will advertise the host
      // in an IOR.
      TAO_IIOP_Endpoint endpoint (listen_point.host.in (),
                                  listen_point.port, addr);

      // Construct a property object
      TAO_Base_Transport_Property prop (&endpoint);

      // Mark the connection as bidirectional
      prop.set_bidir_flag (1);

      // The property for this handler has changed. Recache the
      // handler with this property
      if (this->transport ()->recache_transport (&prop) == -1)
        return -1;

      // Make the handler idle and ready for use
      this->transport ()->make_idle ();
    }

  return 0;
}

int
TAO_IIOP_Connection_Handler::set_tos (int tos)
{
  if (tos != this->dscp_codepoint_)
    {
      int result = 0;
#if defined (ACE_HAS_IPV6)
      ACE_INET_Addr local_addr;
      if (this->peer ().get_local_addr (local_addr) == -1)
        return -1;
      else if (local_addr.get_type () == AF_INET6)
# if !defined (IPV6_TCLASS)
      // IPv6 defines option IPV6_TCLASS for specifying traffic class/priority
      // but not many implementations yet (very new;-).
        {
          if (TAO_debug_level)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT("TAO (%P|%t) - IIOP_Connection_Handler::")
                          ACE_TEXT("set_dscp_codepoint -> IPV6_TCLASS not supported yet\n")));
            }
          return 0;
        }
# else /* !IPV6_TCLASS */
        result = this->peer ().set_option (IPPROTO_IPV6,
                                           IPV6_TCLASS,
                                           (int *) &tos ,
                                           (int) sizeof (tos));
      else
# endif /* IPV6_TCLASS */
#endif /* ACE_HAS_IPV6 */
      result = this->peer ().set_option (IPPROTO_IP,
                                         IP_TOS,
                                         (int *) &tos ,
                                         (int) sizeof (tos));

      if (TAO_debug_level)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - IIOP_Connection_Handler::")
                      ACE_TEXT("set_dscp_codepoint -> dscp: %x; result: %d; %C\n"),
                      tos,
                      result,
                      result == -1 ? "try running as superuser" : ""));
        }

      // On successful setting of TOS field.
      if (result == 0)
        this->dscp_codepoint_ = tos;
    }
  return 0;
}

int
TAO_IIOP_Connection_Handler::set_dscp_codepoint (CORBA::Long dscp_codepoint)
{
  int tos = IPDSFIELD_DSCP_DEFAULT << 2;

  CORBA::Long codepoint = dscp_codepoint;

  tos = static_cast<int> (codepoint) << 2;

  this->set_tos (tos);

  return 0;
}

int
TAO_IIOP_Connection_Handler::set_dscp_codepoint (CORBA::Boolean set_network_priority)
{
  int tos = IPDSFIELD_DSCP_DEFAULT << 2;

  if (set_network_priority)
    {
      TAO_Protocols_Hooks *tph = this->orb_core ()->get_protocols_hooks ();

      if (tph != 0)
        {
          CORBA::Long codepoint = tph->get_dscp_codepoint ();

          tos = static_cast<int> (codepoint) << 2;
          this->set_tos (tos);
        }
    }

  return 0;
}

void
TAO_IIOP_Connection_Handler::abort (void)
{
  struct linger lval;
  lval.l_onoff = 1;
  lval.l_linger = 0;

  if (this->peer ().set_option(SOL_SOCKET,
                               SO_LINGER,
                               (void*) &lval,
                               sizeof (lval)) == -1)
    {
      if (TAO_debug_level)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) Unable to set ")
                      ACE_TEXT ("SO_LINGER on %d\n"),
                      this->peer ().get_handle ()));
        }
    }
}

int
TAO_IIOP_Connection_Handler::handle_write_ready (const ACE_Time_Value *timeout)
{
  return ACE::handle_write_ready (this->peer ().get_handle (), timeout);
}


//@@ CONNECTION_HANDLER_SPL_COPY_HOOK_END
/*
 * End copy hook
 */

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_IIOP && TAO_HAS_IIOP != 0 */
