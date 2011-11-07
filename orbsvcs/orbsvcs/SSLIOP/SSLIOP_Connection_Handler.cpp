// $Id$

#include "orbsvcs/SSLIOP/SSLIOP_Connection_Handler.h"
#include "orbsvcs/SSLIOP/SSLIOP_Endpoint.h"
#include "orbsvcs/SSLIOP/SSLIOP_Util.h"

#include "tao/debug.h"
#include "tao/Base_Transport_Property.h"
#include "tao/ORB_Core.h"
#include "tao/IIOP_Endpoint.h"
#include "tao/IIOP_Connection_Handler.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Wait_Strategy.h"
#include "tao/Protocols_Hooks.h"
#include "ace/os_include/netinet/os_tcp.h"
#include "ace/os_include/os_netdb.h"

#if !defined (__ACE_INLINE__)
# include "orbsvcs/SSLIOP/SSLIOP_Connection_Handler.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::SSLIOP::Connection_Handler::Connection_Handler (
    ACE_Thread_Manager *t)
  : SVC_HANDLER (t, 0 , 0),
    TAO_Connection_Handler (0),
    current_ ()
{
  // This constructor should *never* get called, it is just here to
  // make the compiler happy: the default implementation of the
  // Creation_Strategy requires a constructor with that signature, we
  // don't use that implementation, but some (most?) compilers
  // instantiate it anyway.
  ACE_ASSERT (0);
}

TAO::SSLIOP::Connection_Handler::Connection_Handler (TAO_ORB_Core *orb_core)
  : SVC_HANDLER (orb_core->thr_mgr (), 0, 0),
    TAO_Connection_Handler (orb_core),
    current_ ()
{
  this->current_ = TAO::SSLIOP::Util::current (orb_core);

  TAO::SSLIOP::Transport* specific_transport = 0;
  ACE_NEW (specific_transport,
          TAO::SSLIOP::Transport (this, orb_core));

  // store this pointer (indirectly increment ref count)
  this->transport (specific_transport);
}

TAO::SSLIOP::Connection_Handler::~Connection_Handler (void)
{
  delete this->transport ();
  int const result =
    this->release_os_resources ();

  if (result == -1 && TAO_debug_level)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("TAO (%P|%t) - SSLIOP_Connection_Handler::")
                  ACE_TEXT("~SSLIOP_Connection_Handler, ")
                  ACE_TEXT("release_os_resources() failed %m\n")));
    }
}

int
TAO::SSLIOP::Connection_Handler::open_handler (void *v)
{
  return this->open (v);
}

int
TAO::SSLIOP::Connection_Handler::open (void *)
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
      catch (const CORBA::Exception&)
        {
          return -1;
        }
    }

  if (this->set_socket_option (this->peer (),
                               protocol_properties.send_buffer_size_,
                               protocol_properties.recv_buffer_size_) == -1)
    return -1;

#if !defined (ACE_LACKS_TCP_NODELAY)
  if (this->peer ().set_option (ACE_IPPROTO_TCP,
                                TCP_NODELAY,
                                (void *) &protocol_properties.no_delay_,
                                sizeof (protocol_properties.no_delay_)) == -1)
    return -1;
#endif /* ! ACE_LACKS_TCP_NODELAY */

  //support ORBKeepalive in SSL mode
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

  if (this->transport ()->wait_strategy ()->non_blocking ())
    {
      if (this->peer ().enable (ACE_NONBLOCK) == -1)
        return -1;

      // Enable partial SSL writes.
      //
      // By default, OpenSSL attempts to send the entire chunk of
      // data.  This is fine for relatively small chunks of data.
      // However, if SSL_write() returns with an SSL_ERROR_WANT_WRITE
      // (basically an EWOULDBLOCK) when using non-blocking I/O, TAO
      // may attempt to resend the same data with a potentially
      // different buffer address.  Such a scenario is prone to happen
      // when sending large chunks of data that cause flow control to
      // occur.  For most protocol implementations this is fine.
      // OpenSSL, on the other hand, requires that the same arguments
      // be passed to SSL_write() if an SSL_ERROR_WANT_WRITE error
      // occured on a previous SSL_write() attempt, which cannot be
      // guaranteed by TAO's current message queuing/construction
      // code, often resulting in a "bad write retry" OpenSSL error.
      // To work around this issue, we enable partial SSL_write()s in
      // SSL/TLS connections created by TAO's SSLIOP pluggable
      // protocol.  Doing so makes SSL_write() behave like write(2).
      //
      // This isn't an issue when using blocking I/O.
      (void) ::SSL_set_mode (this->peer ().ssl (),
                             SSL_MODE_ENABLE_PARTIAL_WRITE);
      (void) ::SSL_set_mode (this->peer ().ssl (),
                             SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);
    }

  // Called by the <Strategy_Acceptor> when the handler is
  // completely connected.
  ACE_INET_Addr remote_addr;
  if (this->peer ().get_remote_addr (remote_addr) == -1)
    return -1;

  ACE_INET_Addr local_addr;
  if (this->peer ().get_local_addr (local_addr) == -1)
    return -1;

  if (local_addr == remote_addr)
    {
      if (TAO_debug_level > 0)
        {
          ACE_TCHAR remote_as_string[MAXHOSTNAMELEN + 16];
          ACE_TCHAR local_as_string[MAXHOSTNAMELEN + 16];

          (void) remote_addr.addr_to_string (remote_as_string,
                                             sizeof (remote_as_string) / sizeof (ACE_TCHAR));
          (void) local_addr.addr_to_string (local_as_string,
                                            sizeof (local_as_string) / sizeof (ACE_TCHAR));
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT("TAO(%P|%t) - TAO::SSLIOP::Connection_Handler::open, ")
                      ACE_TEXT("Holy Cow! The remote addr and ")
                      ACE_TEXT("local addr are identical (%s == %s)\n"),
                      remote_as_string, local_as_string));
        }

      return -1;
    }

  if (TAO_debug_level > 0)
    {
      ACE_TCHAR client[MAXHOSTNAMELEN + 16];

      // Verify that we can resolve the peer hostname.
      if (remote_addr.addr_to_string (client,
                                      sizeof (client) / sizeof (ACE_TCHAR)) == -1)
      {
        ACE_OS::strcpy (client, ACE_TEXT("*unable to obtain*"));
      }

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) SSLIOP connection from ")
                  ACE_TEXT ("client <%s> on [%d]\n"),
                  client,
                  this->peer ().get_handle ()));

      // Verify that we can resolve our hostname.
      if (local_addr.addr_to_string (client,
                                      sizeof (client) / sizeof (ACE_TCHAR)) == -1)
      {
        ACE_OS::strcpy (client, ACE_TEXT("*unable to obtain*"));
      }

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) SSLIOP connection accepted from ")
                  ACE_TEXT ("server <%s> on [%d]\n"),
                  client,
                  this->get_handle ()));
    }

  // Set that the transport is now connected, if fails we return -1
  // Use C-style cast b/c otherwise we get warnings on lots of
  // compilers
  if (!this->transport ()->post_open ((size_t) this->get_handle ()))
    return -1;

  // @@ Not needed
  this->state_changed (TAO_LF_Event::LFS_SUCCESS,
           this->orb_core ()->leader_follower ());

  return 0;
}

int
TAO::SSLIOP::Connection_Handler::resume_handler (void)
{
  return ACE_Event_Handler::ACE_APPLICATION_RESUMES_HANDLER;
}

int
TAO::SSLIOP::Connection_Handler::close_connection (void)
{
  return this->close_connection_eh (this);
}

int
TAO::SSLIOP::Connection_Handler::handle_input (ACE_HANDLE h)
{
  return this->handle_input_eh (h, this);
}

int
TAO::SSLIOP::Connection_Handler::handle_output (ACE_HANDLE handle)
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
TAO::SSLIOP::Connection_Handler::handle_timeout (const ACE_Time_Value &,
                                                 const void *)
{
  // Using this to ensure this instance will be deleted (if necessary)
  // only after reset_state(). Without this, when this refcount==1 -
  // the call to close() will cause a call to remove_reference() which
  // will delete this. At that point this->reset_state() is in no
  // man's territory and that causes SEGV on some platforms (Windows!)

  TAO_Auto_Reference<TAO::SSLIOP::Connection_Handler> safeguard (*this);

  // NOTE: Perhaps not the best solution, as it feels like the upper
  // layers should be responsible for this?

  // We don't use this upcall for I/O.  This is only used by the
  // Connector to indicate that the connection timedout.  Therefore,
  // we should call close().
  int const ret = this->close ();
  this->reset_state (TAO_LF_Event::LFS_TIMEOUT);
  return ret;
}

int
TAO::SSLIOP::Connection_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_ASSERT (0);
  return 0;
}

int
TAO::SSLIOP::Connection_Handler::close (u_long flags)
{
  return this->close_handler (flags);
}

int
TAO::SSLIOP::Connection_Handler::release_os_resources (void)
{
  return this->peer().close ();
}

void
TAO::SSLIOP::Connection_Handler::pos_io_hook (int & return_value)
{
  if (return_value == 0 && ::SSL_pending (this->peer ().ssl ()))
    return_value = 1;
}

int
TAO::SSLIOP::Connection_Handler::add_transport_to_cache (void)
{
  ACE_INET_Addr addr;

  // Get the peername.
  //
  // Note that the port set in the ACE_INET_Addr is actually the SSL
  // port!
  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  // Construct an  IIOP_Endpoint object
  TAO_IIOP_Endpoint tmpoint (
      addr,
      this->orb_core()->orb_params()->use_dotted_decimal_addresses());

  // @@ This is broken.  We need to include the SecurityAssociation
  //    options to be able to truly distinguish cached SSLIOP
  //    transports.
  const ::SSLIOP::SSL ssl =
    {
      0,                        // target_supports
      0,                        // target_requires
      addr.get_port_number ()   // port
    };

  TAO_SSLIOP_Endpoint endpoint (&ssl, &tmpoint);

  // Construct a property object
  TAO_Base_Transport_Property prop (&endpoint);

  TAO::Transport_Cache_Manager &cache =
    this->orb_core ()->lane_resources ().transport_cache ();

  // Add the handler to Cache
  return cache.cache_transport (&prop, this->transport ());
}

int
TAO::SSLIOP::Connection_Handler::process_listen_point_list (
  IIOP::ListenPointList &listen_list)
{
  // Get the size of the list
  CORBA::ULong const len = listen_list.length ();

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      IIOP::ListenPoint listen_point = listen_list[i];
      ACE_INET_Addr addr (listen_point.port,
                          listen_point.host.in ());


      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) Listening port [%d] on [%C]\n",
                      listen_point.port,
                      listen_point.host.in ()));
        }

      // Construct an IIOP_Endpoint object using the host as provided
      // in the listen point list.  We must use the host in that form
      // because that's also how the ORB on the other side will
      // advertise the host in an IOR.
      //
      // Note that the port in the ACE_INET_Addr is actually the SSL
      // port!
      TAO_IIOP_Endpoint tmpoint (listen_point.host.in (),
                                 listen_point.port,
                                 addr);

      // @@ This is broken.  Instead of just using the default CORBA
      // SecurityAssociation options, by not supplying SSLIOP::SSL
      // instance in the endpoint constructor, we need to include the
      // actual SecurityAssociation options so that the invocation to
      // the originator is attempted with the appropriate security
      // settings. Unfortunately, there is currently no portable way to
      // send the SecurityAssociation options with the
      //    IIOP::ListenPointList.  Presumably the new Firewall
      //    specification will address this deficiency.
      TAO_SSLIOP_Synthetic_Endpoint endpoint (&tmpoint);

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
TAO::SSLIOP::Connection_Handler::setup_ssl_state (
  TAO::SSLIOP::Current_Impl *&previous_current_impl,
  TAO::SSLIOP::Current_Impl *new_current_impl,
  bool &setup_done)
{
  // Make the SSL session state available to the SSLIOP::Current
  // TSS object.
  new_current_impl->ssl (this->peer ().ssl ());

  // The following call is reentrant and thread-safe
  this->current_->setup (previous_current_impl,
                         new_current_impl,
                         setup_done);

  return 0;
}

void
TAO::SSLIOP::Connection_Handler::teardown_ssl_state (
  TAO::SSLIOP::Current_Impl *previous_current_impl,
  bool &setup_done)
{
  this->current_->teardown (previous_current_impl, setup_done);
}

int
TAO::SSLIOP::Connection_Handler::handle_write_ready (const ACE_Time_Value *t)
{
  return ACE::handle_write_ready (this->peer ().get_handle (), t);
}

TAO_END_VERSIONED_NAMESPACE_DECL
