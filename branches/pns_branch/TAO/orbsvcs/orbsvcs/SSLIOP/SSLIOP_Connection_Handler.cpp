#include "SSLIOP_Connection_Handler.h"
#include "SSLIOP_Endpoint.h"

#include "tao/debug.h"
#include "tao/Base_Transport_Property.h"
#include "tao/ORB_Core.h"
#include "tao/IIOP_Endpoint.h"
#include "tao/IIOP_Connection_Handler.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Wait_Strategy.h"
#include "ace/os_include/netinet/os_tcp.h"
#include "ace/os_include/os_netdb.h"

#if !defined (__ACE_INLINE__)
# include "SSLIOP_Connection_Handler.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (SSLIOP,
           SSLIOP_Connection_Handler,
           "$Id$")

// ****************************************************************

TAO::SSLIOP::Connection_Handler::Connection_Handler (
    ACE_Thread_Manager *t)
  : SVC_HANDLER (t, 0 , 0),
    TAO_Connection_Handler (0),
    current_ (),
    tcp_properties_ (0)
{
  // This constructor should *never* get called, it is just here to
  // make the compiler happy: the default implementation of the
  // Creation_Strategy requires a constructor with that signature, we
  // don't use that implementation, but some (most?) compilers
  // instantiate it anyway.
  ACE_ASSERT (0);
}

TAO::SSLIOP::Connection_Handler::Connection_Handler (
    TAO_ORB_Core *orb_core,
    CORBA::Boolean /* flag */, // SSLIOP does *not* support GIOPlite
    void *arg)
  : SVC_HANDLER (orb_core->thr_mgr (), 0, 0),
    TAO_Connection_Handler (orb_core),
    current_ (),
    tcp_properties_ (0)
{
  TAO::SSLIOP::Connection_Handler_State *s =
    static_cast<TAO::SSLIOP::Connection_Handler_State *> (arg);

  this->tcp_properties_ = s->tcp_properties;
  this->current_ = s->ssliop_current;

  TAO::SSLIOP::Transport* specific_transport = 0;
  ACE_NEW (specific_transport,
          TAO::SSLIOP::Transport (this, orb_core, 0));

  // store this pointer (indirectly increment ref count)
  this->transport (specific_transport);
}


TAO::SSLIOP::Connection_Handler::~Connection_Handler (void)
{
  delete this->transport ();
}

int
TAO::SSLIOP::Connection_Handler::open_handler (void *v)
{
  return this->open (v);
}

int
TAO::SSLIOP::Connection_Handler::open (void *)
{
  if (this->set_socket_option (this->peer (),
                               tcp_properties_->send_buffer_size,
                               tcp_properties_->recv_buffer_size) == -1)
    return -1;

#if !defined (ACE_LACKS_TCP_NODELAY)
  if (this->peer ().set_option (ACE_IPPROTO_TCP,
                                TCP_NODELAY,
                                (void *) &this->tcp_properties_->no_delay,
                                sizeof (int)) == -1)
    return -1;
#endif /* ! ACE_LACKS_TCP_NODELAY */

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
    }

  // Called by the <Strategy_Acceptor> when the handler is
  // completely connected.
  ACE_INET_Addr remote_addr;
  if (this->peer ().get_remote_addr (remote_addr) == -1)
    return -1;

  ACE_INET_Addr local_addr;
  if (this->peer ().get_local_addr (local_addr) == -1)
    return -1;

  if (local_addr.get_ip_address () == remote_addr.get_ip_address ()
      && local_addr.get_port_number () == remote_addr.get_port_number ())
    {
      if (TAO_debug_level > 0)
        {
          char remote_as_string[MAXHOSTNAMELEN + 16];
          char local_as_string[MAXHOSTNAMELEN + 16];

          (void) remote_addr.addr_to_string (remote_as_string,
                                             sizeof (remote_as_string));
          (void) local_addr.addr_to_string (local_as_string,
                                            sizeof (local_as_string));
          ACE_ERROR ((LM_ERROR,
                      "TAO(%P|%t) - TAO::SSLIOP::Connection_Handler::open, "
                      "Holy Cow! The remote addr and "
                      "local addr are identical (%s == %s)\n",
                      remote_as_string, local_as_string));
        }

      return -1;
    }

  if (TAO_debug_level > 0)
    {
      char client[MAXHOSTNAMELEN + 16];

      // Verify that we can resolve the peer hostname.
      if (remote_addr.addr_to_string (client, sizeof (client)) == -1)
        return -1;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) SSLIOP connection from ")
                  ACE_TEXT ("client <%s> on %d\n"),
                  client,
                  this->peer ().get_handle ()));
    }

  // Set that the transport is now connected, if fails we return -1
  // Use C-style cast b/c otherwise we get warnings on lots of
  // compilers
  if (!this->transport ()->post_open ((size_t) this->get_handle ()))
    return -1;

  // @@ Not needed
  this->state_changed (TAO_LF_Event::LFS_SUCCESS);

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
  const int result =
    this->handle_input_eh (h, this);

  if (result == -1)
    {
      this->close_connection ();
      return 0;
    }

  return result;
}

int
TAO::SSLIOP::Connection_Handler::handle_output (ACE_HANDLE handle)
{
  const int result =
    this->handle_output_eh (handle, this);

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
  // We don't use this upcall for I/O.  This is only used by the
  // Connector to indicate that the connection timedout.  Therefore,
  // we should call close().
  return this->close ();
}

int
TAO::SSLIOP::Connection_Handler::handle_close (ACE_HANDLE,
                                             ACE_Reactor_Mask)
{
  ACE_ASSERT (0);
  return 0;
}

int
TAO::SSLIOP::Connection_Handler::close (u_long)
{
  return this->close_handler ();
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

  TAO_SSLIOP_Endpoint endpoint (&ssl,
                                &tmpoint);

  // Construct a property object
  TAO_Base_Transport_Property prop (&endpoint);

  TAO_Transport_Cache_Manager &cache =
    this->orb_core ()->lane_resources ().transport_cache ();

  // Add the handler to Cache
  return cache.cache_idle_transport (&prop,
                                     this->transport ());
}

int
TAO::SSLIOP::Connection_Handler::process_listen_point_list (
  IIOP::ListenPointList &listen_list)
{
  // Get the size of the list
  const CORBA::ULong len = listen_list.length ();

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      IIOP::ListenPoint listen_point = listen_list[i];
      ACE_INET_Addr addr (listen_point.port,
                          listen_point.host.in ());


      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) Listening port [%d] on [%s]\n",
                      listen_point.port,
                      listen_point.host.in ()));
        }

      // Construct an  IIOP_Endpoint object
      // Construct an IIOP_Endpoint object.
      //
      // Note that the port in the ACE_INET_Addr is actually the SSL
      // port!
      TAO_IIOP_Endpoint tmpoint (addr,
        this->orb_core()->orb_params()->use_dotted_decimal_addresses ());

      // @@ This is broken.  We need to include the
      //    SecurityAssociation options so that the invocation to the
      //    originator is attempted with the appropriate security
      //    settings.  Unfortunately, there is currently no portable
      //    way to send the SecurityAssociation options with the
      //    IIOP::ListenPointList.  Presumably the new Firewall
      //    specification will address this deficiency.
      const ::SSLIOP::SSL ssl =
        {
          0,                        // target_supports
          0,                        // target_requires
          listen_point.port         // port
        };

      // Construct an SSLIOP_Endpoint
      TAO_SSLIOP_Endpoint endpoint (&ssl,
                                    &tmpoint);

      // Construct a property object
      TAO_Base_Transport_Property prop (&endpoint);

      // Mark the connection as bidirectional
      prop.set_bidir_flag (1);

      // The property for this handler has changed. Recache the
      // handler with this property
      const int retval = this->transport ()->recache_transport (&prop);
      if (retval == -1)
        return retval;

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
  this->current_->teardown (previous_current_impl,
                            setup_done);
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Svc_Handler<ACE_SSL_SOCK_STREAM, ACE_NULL_SYNCH>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Svc_Handler<ACE_SSL_SOCK_STREAM, ACE_NULL_SYNCH>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
