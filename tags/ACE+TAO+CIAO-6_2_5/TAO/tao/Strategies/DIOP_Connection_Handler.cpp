// $Id$

#include "tao/Strategies/DIOP_Connection_Handler.h"

#if defined (TAO_HAS_DIOP) && (TAO_HAS_DIOP != 0)

#include "tao/Timeprobe.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/CDR.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Protocols_Hooks.h"
#include "tao/Resume_Handle.h"

#include "tao/Strategies/DIOP_Transport.h"
#include "tao/Strategies/DIOP_Endpoint.h"

#include "ace/os_include/netinet/os_tcp.h"
#include "ace/os_include/os_netdb.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DIOP_Connection_Handler::TAO_DIOP_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_DIOP_SVC_HANDLER (t, 0 , 0),
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

TAO_DIOP_Connection_Handler::TAO_DIOP_Connection_Handler (TAO_ORB_Core *orb_core)
  : TAO_DIOP_SVC_HANDLER (orb_core->thr_mgr (), 0, 0),
    TAO_Connection_Handler (orb_core),
    dscp_codepoint_ (IPDSFIELD_DSCP_DEFAULT << 2)
{
  TAO_DIOP_Transport* specific_transport = 0;
  ACE_NEW (specific_transport,
           TAO_DIOP_Transport (this, orb_core));

  // store this pointer (indirectly increment ref count)
  this->transport (specific_transport);
}

TAO_DIOP_Connection_Handler::~TAO_DIOP_Connection_Handler (void)
{
  delete this->transport ();
  int const result =
    this->release_os_resources ();

  if (result == -1 && TAO_debug_level)
    {
      TAOLIB_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) - DIOP_Connection_Handler::")
                  ACE_TEXT ("~DIOP_Connection_Handler, ")
                  ACE_TEXT ("release_os_resources() failed %m\n")));
    }
}

// DIOP Additions - Begin
const ACE_INET_Addr &
TAO_DIOP_Connection_Handler::addr (void)
{
  return this->addr_;
}

void
TAO_DIOP_Connection_Handler::addr (const ACE_INET_Addr &addr)
{
  this->addr_ = addr;
}

const ACE_INET_Addr &
TAO_DIOP_Connection_Handler::local_addr (void)
{
  return this->local_addr_;
}

void
TAO_DIOP_Connection_Handler::local_addr (const ACE_INET_Addr &addr)
{
  this->local_addr_ = addr;
}
// DIOP Additions - End

int
TAO_DIOP_Connection_Handler::open_handler (void *v)
{
  return this->open (v);
}

int
TAO_DIOP_Connection_Handler::open (void*)
{
  TAO_DIOP_Protocol_Properties protocol_properties;

  // Initialize values from ORB params.
  protocol_properties.send_buffer_size_ =
    this->orb_core ()->orb_params ()->sock_sndbuf_size ();
  protocol_properties.recv_buffer_size_ =
    this->orb_core ()->orb_params ()->sock_rcvbuf_size ();
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

  this->peer ().open (this->local_addr_);

  if (this->set_socket_option (this->peer (),
                               protocol_properties.send_buffer_size_,
                               protocol_properties.recv_buffer_size_) == -1)
    return -1;

  if (protocol_properties.hop_limit_ >= 0)
    {
      int result = 0;
#if defined (ACE_HAS_IPV6)
      if (this->local_addr_.get_type () == AF_INET6)
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
              TAOLIB_ERROR ((LM_ERROR,
                          ACE_TEXT("TAO (%P|%t) - DIOP_Connection_Handler::open, ")
                          ACE_TEXT("couldn't set hop limit\n\n")));
            }
          return -1;
        }
    }

  if (TAO_debug_level > 5)
  {
     TAOLIB_DEBUG ((LM_DEBUG,
                 ACE_TEXT("TAO (%P|%t) - DIOP_Connection_Handler::open, ")
                 ACE_TEXT("listening on: <%C:%u>\n"),
                 this->local_addr_.get_host_name (),
                 this->local_addr_.get_port_number ()));
  }

  // Set that the transport is now connected, if fails we return -1
  // Use C-style cast b/c otherwise we get warnings on lots of
  // compilers
  if (!this->transport ()->post_open ((size_t) this->peer ().get_handle ()))
    return -1;

  this->state_changed (TAO_LF_Event::LFS_SUCCESS,
                       this->orb_core ()->leader_follower ());

  return 0;
}

int
TAO_DIOP_Connection_Handler::open_server (void)
{
  TAO_DIOP_Protocol_Properties protocol_properties;

  // Initialize values from ORB params.
  protocol_properties.send_buffer_size_ =
    this->orb_core ()->orb_params ()->sock_sndbuf_size ();
  protocol_properties.recv_buffer_size_ =
    this->orb_core ()->orb_params ()->sock_rcvbuf_size ();

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

  this->peer ().open (this->local_addr_);

  if (this->set_socket_option (this->peer (),
                               protocol_properties.send_buffer_size_,
                               protocol_properties.recv_buffer_size_) == -1)
    return -1;

  if (TAO_debug_level > 5)
    {
      TAOLIB_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - DIOP_Connection_Handler::open_server, ")
                  ACE_TEXT("listening on %C:%d\n"),
                  this->local_addr_.get_host_name (),
                  this->local_addr_.get_port_number ()
                ));
    }

  this->transport ()->id ((size_t) this->peer ().get_handle ());

  return 0;
}

int
TAO_DIOP_Connection_Handler::resume_handler (void)
{
  return ACE_Event_Handler::ACE_APPLICATION_RESUMES_HANDLER;
}

int
TAO_DIOP_Connection_Handler::close_connection (void)
{
  return this->close_connection_eh (this);
}

int
TAO_DIOP_Connection_Handler::handle_input (ACE_HANDLE h)
{
  return this->handle_input_eh (h, this);
}

int
TAO_DIOP_Connection_Handler::handle_output (ACE_HANDLE handle)
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
TAO_DIOP_Connection_Handler::handle_timeout (const ACE_Time_Value &,
                                             const void *)
{
  // We don't use this upcall from the Reactor.  However, we should
  // override this since the base class returns -1 which will result
  // in handle_close() getting called.
  return 0;
}

int
TAO_DIOP_Connection_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  // No asserts here since the handler is registered with the Reactor
  // and the handler ownership is given to the Reactor.  When the
  // Reactor closes, it will call handle_close() on the handler.  It
  // is however important to overwrite handle_close() to do nothing
  // since the base class does too much.
  return 0;
}

int
TAO_DIOP_Connection_Handler::close (u_long flags)
{
  return this->close_handler (flags);
}

int
TAO_DIOP_Connection_Handler::release_os_resources (void)
{
  return this->peer ().close ();
}

int
TAO_DIOP_Connection_Handler::add_transport_to_cache (void)
{
  ACE_INET_Addr addr;

  // This function is called by the acceptor to add this
  // transport to the transport cache.  This is really
  // important for proper shutdown.  The address used
  // is irrelevent, since DIOP is connectionless.

  // Construct a DIOP_Endpoint object.
  TAO_DIOP_Endpoint endpoint (
      addr,
      this->orb_core ()->orb_params ()->cache_incoming_by_dotted_decimal_address ());

  // Construct a property object
  TAO_Base_Transport_Property prop (&endpoint);

  // Add the handler to Cache
  return this->orb_core ()->lane_resources ()
    .transport_cache ().cache_transport (&prop, this->transport ());
}

int
TAO_DIOP_Connection_Handler::set_tos (int tos)
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
              TAOLIB_DEBUG ((LM_DEBUG,
                          "TAO (%P|%t) - DIOP_Connection_Handler::"
                          "set_dscp_codepoint -> IPV6_TCLASS not supported yet\n"));
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
          TAOLIB_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - DIOP_Connection_Handler::"
                      "set_dscp_codepoint, dscp: %x; result: %d; %C\n",
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
TAO_DIOP_Connection_Handler::set_dscp_codepoint (CORBA::Long dscp)
{
  int tos = IPDSFIELD_DSCP_DEFAULT << 2;
  tos = (int)(dscp) << 2;
  this->set_tos (tos);
  return 0;
}

int
TAO_DIOP_Connection_Handler::set_dscp_codepoint (CORBA::Boolean set_network_priority)
{
  int tos = IPDSFIELD_DSCP_DEFAULT << 2;

  if (set_network_priority)
    {
      TAO_Protocols_Hooks *tph = this->orb_core ()->get_protocols_hooks ();

      if (tph != 0)
        {
          CORBA::Long codepoint = tph->get_dscp_codepoint ();

          tos = (int)(codepoint) << 2;
          this->set_tos (tos);
        }
    }
  return 0;
}

int
TAO_DIOP_Connection_Handler::handle_write_ready (const ACE_Time_Value *t)
{
  return ACE::handle_write_ready (this->peer ().get_handle (), t);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */
