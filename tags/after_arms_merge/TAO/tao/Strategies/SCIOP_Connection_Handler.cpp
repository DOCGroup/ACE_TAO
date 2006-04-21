#include "tao/Strategies/SCIOP_Connection_Handler.h"

#if TAO_HAS_SCIOP == 1

// jcohen@atl.lmco.com: The purpose of this is to allow RH9 to build
// SCIOR Profile support even without a functioning SCTP implementation

#ifndef IPPROTO_SCTP
#  include "netinet/sctp.h"
#else // !IPPROTO_SCTP
#  ifndef SCTP_NODELAY
#    define SCTP_NODELAY 1
#  endif // !SCTP_NODELAY
#endif

#include "tao/Timeprobe.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/CDR.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/Strategies/SCIOP_Transport.h"
#include "tao/Strategies/SCIOP_Endpoint.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Resume_Handle.h"
#include "tao/Protocols_Hooks.h"
#include "tao/Wait_Strategy.h"

ACE_RCSID (tao,
           SCIOP_Connection_Handler,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_SCIOP_Connection_Handler::TAO_SCIOP_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_SCIOP_SVC_HANDLER (t, 0 , 0),
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


TAO_SCIOP_Connection_Handler::TAO_SCIOP_Connection_Handler (TAO_ORB_Core *orb_core,
                                                            CORBA::Boolean flag)
  : TAO_SCIOP_SVC_HANDLER (orb_core->thr_mgr (), 0, 0),
    TAO_Connection_Handler (orb_core),
    dscp_codepoint_ (IPDSFIELD_DSCP_DEFAULT << 2)
{
  TAO_SCIOP_Transport* specific_transport = 0;
  ACE_NEW (specific_transport,
           TAO_SCIOP_Transport (this, orb_core, flag));

  // store this pointer (indirectly increment ref count)
  this->transport (specific_transport);
}

TAO_SCIOP_Connection_Handler::TAO_SCIOP_Connection_Handler (TAO_ORB_Core *orb_core)
  : TAO_SCIOP_SVC_HANDLER (orb_core->thr_mgr (), 0, 0),
    TAO_Connection_Handler (orb_core),
    dscp_codepoint_ (IPDSFIELD_DSCP_DEFAULT << 2)
{
}


TAO_SCIOP_Connection_Handler::~TAO_SCIOP_Connection_Handler (void)
{
  delete this->transport ();
}

int
TAO_SCIOP_Connection_Handler::open_handler (void *v)
{
  return this->open (v);
}

int
TAO_SCIOP_Connection_Handler::open (void*)
{
  TAO_SCIOP_Protocol_Properties protocol_properties;

  // Initialize values from ORB params.
  protocol_properties.send_buffer_size_ =
    this->orb_core ()->orb_params ()->sock_sndbuf_size ();
  protocol_properties.recv_buffer_size_ =
    this->orb_core ()->orb_params ()->sock_rcvbuf_size ();
  protocol_properties.no_delay_ =
    this->orb_core ()->orb_params ()->nodelay ();

  TAO_Protocols_Hooks *tph =
    this->orb_core ()->get_protocols_hooks ();

  bool client =
    this->transport ()->opened_as () == TAO::TAO_CLIENT_ROLE;;

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      if (client)
        {
          tph->client_protocol_properties_at_orb_level (
            protocol_properties
            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else
        {
          tph->server_protocol_properties_at_orb_level (
            protocol_properties
            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  if (this->set_socket_option (this->peer (),
                               protocol_properties.send_buffer_size_,
                               protocol_properties.recv_buffer_size_) == -1)
    return -1;

#if !defined (ACE_LACKS_TCP_NODELAY)
  if (this->peer ().set_option (IPPROTO_SCTP,
                                SCTP_NODELAY,
                                (void *) &protocol_properties.no_delay_,
                                sizeof (protocol_properties.no_delay_)) == -1)
    return -1;
#endif /* ! ACE_LACKS_TCP_NODELAY */

  if (this->transport ()->wait_strategy ()->non_blocking ())
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
                ACE_TEXT("TAO(%P|%t) - SCIOP_Connection_Handler::open, ")
                ACE_TEXT("The local addr is (%s) \n"),
                local_addr. get_host_addr ()));

  if (local_addr.get_ip_address () == remote_addr.get_ip_address ()
      && local_addr.get_port_number () == remote_addr.get_port_number ())
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
                      ACE_TEXT("TAO(%P|%t) - TAO_SCIOP_Connection_Handler::open, ")
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
      if (remote_addr.addr_to_string (client, sizeof (client)) == -1)
        return -1;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - SCIOP_Connection_Handler::open, SCIOP ")
                  ACE_TEXT ("connection to peer <%s> on %d\n"),
                  client, this->peer ().get_handle ()));
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
TAO_SCIOP_Connection_Handler::resume_handler (void)
{
  return ACE_Event_Handler::ACE_APPLICATION_RESUMES_HANDLER;
}

int
TAO_SCIOP_Connection_Handler::close_connection (void)
{
  return this->close_connection_eh (this);
}

int
TAO_SCIOP_Connection_Handler::handle_input (ACE_HANDLE h)
{
  return this->handle_input_eh (h, this);
}

int
TAO_SCIOP_Connection_Handler::handle_output (ACE_HANDLE handle)
{
  int const result =
    this->handle_output_eh (handle, this);

  if (result == -1)
    {
      this->close_connection ();
      return 0;
    }

  return result;
}

int
TAO_SCIOP_Connection_Handler::handle_timeout (const ACE_Time_Value &,
                                              const void *)
{
  // We don't use this upcall for I/O.  This is only used by the
  // Connector to indicate that the connection timedout.  Therefore,
  // we should call close().
  return this->close ();
}

int
TAO_SCIOP_Connection_Handler::handle_close (ACE_HANDLE,
                                            ACE_Reactor_Mask)
{
  ACE_ASSERT (0);
  return 0;
}

int
TAO_SCIOP_Connection_Handler::close (u_long)
{
  return this->close_handler ();
}

int
TAO_SCIOP_Connection_Handler::release_os_resources (void)
{
  return this->peer().close ();
}

int
TAO_SCIOP_Connection_Handler::add_transport_to_cache (void)
{
  ACE_INET_Addr addr;

  // Get the peername.
  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  // Construct an  SCIOP_Endpoint object
  TAO_SCIOP_Endpoint endpoint (
      addr,
      this->orb_core()->orb_params()->use_dotted_decimal_addresses ());

  // Construct a property object
  TAO_Base_Transport_Property prop (&endpoint);

  TAO::Transport_Cache_Manager &cache =
    this->orb_core ()->lane_resources ().transport_cache ();

  // Idle the transport..
  return cache.cache_idle_transport (&prop,
                                     this->transport ());

}

int
TAO_SCIOP_Connection_Handler::process_listen_point_list (
    IIOP::ListenPointList &listen_list)
{
  // Get the size of the list
  CORBA::ULong const len = listen_list.length ();

  for (CORBA::ULong i = 0; i < len; ++ i)
    {
      IIOP::ListenPoint listen_point = listen_list[i];
      ACE_INET_Addr addr (listen_point.port,
                          listen_point.host.in ());

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("(%P|%t) Listening port [%d] on [%s]\n"),
                      listen_point.port,
                      ACE_TEXT_CHAR_TO_TCHAR(listen_point.host.in ())));
        }

      // Construct an  SCIOP_Endpoint object
      TAO_SCIOP_Endpoint endpoint (addr,
        this->orb_core()->orb_params()->use_dotted_decimal_addresses ());

      // Construct a property object
      TAO_Base_Transport_Property prop (&endpoint);

      // Mark the connection as bidirectional
      prop.set_bidir_flag (1);

      // The property for this handler has changed. Recache the
      // handler with this property
      int const retval = this->transport ()->recache_transport (&prop);
      if (retval == -1)
        return retval;

      // Make the handler idle and ready for use
      this->transport ()->make_idle ();
    }

  return 0;
}

int
TAO_SCIOP_Connection_Handler::set_dscp_codepoint (CORBA::Boolean set_network_priority)
{
  int tos = IPDSFIELD_DSCP_DEFAULT << 2;

  if (set_network_priority)
    {
      TAO_Protocols_Hooks *tph =
        this->orb_core ()->get_protocols_hooks ();

      CORBA::Long codepoint =
        tph->get_dscp_codepoint ();

      tos = (int)(codepoint) << 2;
    }

  if (tos != this->dscp_codepoint_)
    {
      int const result = this->peer ().set_option (IPPROTO_IP,
                                                   IP_TOS,
                                                   (int *) &tos ,
                                                   (int) sizeof (tos));

      if (TAO_debug_level)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - SCIOP_Connection_Handler::"
                      "set_dscp_codepoint -> dscp: %x; result: %d; %s\n",
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

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_SCIOP == 1 */
