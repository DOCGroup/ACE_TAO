// $Id$

#include "orbsvcs/PortableGroup/UIPMC_Connection_Handler.h"
#include "orbsvcs/PortableGroup/UIPMC_Endpoint.h"
#include "orbsvcs/PortableGroup/UIPMC_Transport.h"
#include "orbsvcs/PortableGroup/miop_resource.h"

#include "tao/Timeprobe.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/CDR.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Resume_Handle.h"
#include "tao/Protocols_Hooks.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_UIPMC_Connection_Handler::TAO_UIPMC_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_UIPMC_SVC_HANDLER (t, 0 , 0),
    TAO_Connection_Handler (0),
    dscp_codepoint_ (IPDSFIELD_DSCP_DEFAULT << 2),
    send_hi_water_mark_ (0u)
{
  // This constructor should *never* get called, it is just here to
  // make the compiler happy: the default implementation of the
  // Creation_Strategy requires a constructor with that signature, we
  // don't use that implementation, but some (most?) compilers
  // instantiate it anyway.
  ACE_ASSERT (0);
}

TAO_UIPMC_Connection_Handler::TAO_UIPMC_Connection_Handler (TAO_ORB_Core *orb_core)
  : TAO_UIPMC_SVC_HANDLER (orb_core->thr_mgr (), 0, 0),
    TAO_Connection_Handler (orb_core),
    dscp_codepoint_ (IPDSFIELD_DSCP_DEFAULT << 2),
    send_hi_water_mark_ (0u)
{
  TAO_UIPMC_Transport* specific_transport = 0;
  ACE_NEW(specific_transport,
          TAO_UIPMC_Transport (this, orb_core));

  // store this pointer (indirectly increment ref count)
  this->transport (specific_transport);
}

TAO_UIPMC_Connection_Handler::~TAO_UIPMC_Connection_Handler (void)
{
  delete this->transport ();
  int const result =
    this->release_os_resources ();

  if (result == -1 && TAO_debug_level)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) - UIPMC_Connection_Handler::")
                  ACE_TEXT ("~UIPMC_Connection_Handler, ")
                  ACE_TEXT ("release_os_resources() failed %m\n")));
    }
}

const ACE_INET_Addr &
TAO_UIPMC_Connection_Handler::addr (void) const
{
  return this->addr_;
}

void
TAO_UIPMC_Connection_Handler::addr (const ACE_INET_Addr &addr)
{
  this->addr_ = addr;
}

const ACE_INET_Addr &
TAO_UIPMC_Connection_Handler::local_addr (void) const
{
  return this->local_addr_;
}

void
TAO_UIPMC_Connection_Handler::local_addr (const ACE_INET_Addr &addr)
{
  this->local_addr_ = addr;
}

u_long
TAO_UIPMC_Connection_Handler::send_hi_water_mark (void) const
{
  return this->send_hi_water_mark_;
}

int
TAO_UIPMC_Connection_Handler::open_handler (void *v)
{
  return this->open (v);
}

int
TAO_UIPMC_Connection_Handler::open (void*)
{
  TAO_MIOP_Resource_Factory *const factory =
    ACE_Dynamic_Service<TAO_MIOP_Resource_Factory>::instance (
      this->orb_core ()->configuration(),
      ACE_TEXT ("MIOP_Resource_Factory"));

  // Since only client can send data over MIOP
  // then ttl is only applicable to client socket.
  TAO_DIOP_Protocol_Properties protocol_properties;

  // Initialize values from ORB params.
  protocol_properties.send_buffer_size_ =
    factory->send_buffer_size () ?
    factory->send_buffer_size () :
    this->orb_core ()->orb_params ()->sock_sndbuf_size ();
  protocol_properties.hop_limit_ =
    this->orb_core ()->orb_params ()->ip_hoplimit ();
  protocol_properties.enable_multicast_loop_ =
    this->orb_core ()->orb_params ()->ip_multicastloop ();

  TAO_Protocols_Hooks *tph =
    this->orb_core ()->get_protocols_hooks ();

  if (tph != 0)
    {
      try
        {
          tph->client_protocol_properties_at_orb_level (protocol_properties);
        }
      catch (const ::CORBA::Exception&)
        {
          return -1;
        }
    }

  if (this->peer ().open (this->local_addr_) == -1 ||
      this->set_socket_option (this->peer (),
                               protocol_properties.send_buffer_size_,
                               0) == -1)
    {
      return -1;
    }

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
            IPV6_MULTICAST_HOPS,
            (void *) &hop_limit,
            sizeof (hop_limit));
        }
      else
#endif /* ACE_HAS_IPV6 */
        {
#if defined (ACE_WIN32)
          DWORD hop_limit =
            static_cast<DWORD> (protocol_properties.hop_limit_);
#elif defined (ACE_HAS_IP_MULTICAST_TTL_AS_INT)
          int hop_limit =
            static_cast<int> (protocol_properties.hop_limit_);
#else
          unsigned char hop_limit =
            static_cast<unsigned char> (protocol_properties.hop_limit_);
#endif
          result = this->peer ().set_option (
            IPPROTO_IP,
            IP_MULTICAST_TTL,
            (void *) &hop_limit,
            sizeof (hop_limit));
        }

      if (result != 0)
        {
          if (TAO_debug_level)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT("TAO (%P|%t) - UIPMC_Connection_Handler::open, ")
                          ACE_TEXT("couldn't set hop limit '%m'\n")));
            }
          return -1;
        }
    }

  int result = 0;
#if defined (ACE_HAS_IPV6)
  if (this->local_addr_.get_type () == AF_INET6)
    {
#if defined (ACE_WIN32)
      DWORD enable_loop =
        static_cast<DWORD> (protocol_properties.enable_multicast_loop_);
#elif defined (ACE_HAS_IPV6_MULTICAST_LOOP_AS_BOOL)
      bool enable_loop =
          static_cast<bool> (protocol_properties.enable_multicast_loop_);
#else
      unsigned int enable_loop =
        static_cast<unsigned int> (protocol_properties.enable_multicast_loop_);
#endif
      result = this->peer ().set_option (
        IPPROTO_IPV6,
        IPV6_MULTICAST_LOOP,
        (void *) &enable_loop,
        sizeof (enable_loop));
    }
  else
#endif /* ACE_HAS_IPV6 */
    {
#if defined (ACE_WIN32)
      DWORD enable_loop =
        static_cast<DWORD> (protocol_properties.enable_multicast_loop_);
#elif defined (ACE_HAS_IP_MULTICAST_LOOP_AS_INT)
      int enable_loop =
        static_cast<int> (protocol_properties.enable_multicast_loop_);
#else
      unsigned char enable_loop =
        static_cast<unsigned char> (protocol_properties.enable_multicast_loop_);
#endif
      result = this->peer ().set_option (
        IPPROTO_IP,
        IP_MULTICAST_LOOP,
        (void *) &enable_loop,
        sizeof (enable_loop));
    }

  if (result != 0)
    {
      if (TAO_debug_level)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - UIPMC_Connection_Handler::open, ")
                      ACE_TEXT ("couldn't %s multicast packets looping '%m'\n"),
                      protocol_properties.enable_multicast_loop_ ?
                      ACE_TEXT ("enable") : ACE_TEXT ("disable")
                     ));
        }
      return -1;
    }

  this->send_hi_water_mark_ = factory->send_hi_water_mark ();
  if (!this->send_hi_water_mark_)
    {
#if defined (ACE_LACKS_SO_SNDBUF)
          // Assume a small buffer
          this->send_hi_water_mark_ = 1024u;
          if (TAO_debug_level)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - UIPMC_Connection_Handler::")
                        ACE_TEXT ("open, -ORBSendHighWaterMark not specified ")
                        ACE_TEXT ("using %u bytes\n"),
                        this->send_hi_water_mark_));
#else
      int size = sizeof (this->send_hi_water_mark_);
      result =
        this->peer ().get_option (SOL_SOCKET,
                                  SO_SNDBUF,
                                  static_cast<void *> (&this->send_hi_water_mark_),
                                  &size);
      if (!result)
        {
          // Note unix kernals double the value that is set (to hold the
          // internal data structures seporating each packet) and this doubled
          // value is what is returned by the get_option, so it is best to halve.
          this->send_hi_water_mark_ >>= 1;
          if (TAO_debug_level)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - UIPMC_Connection_Handler::")
                        ACE_TEXT ("open, -ORBSendHighWaterMark not specified, ")
                        ACE_TEXT ("using -ORBSndSock value of %u bytes\n"),
                        this->send_hi_water_mark_));
        }
      else
        {
          // Assume a small buffer
          this->send_hi_water_mark_ = 1024u;
          if (TAO_debug_level)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - UIPMC_Connection_Handler::")
                        ACE_TEXT ("open, -ORBSendHighWaterMark not specified ")
                        ACE_TEXT ("and getsockopt failed '%m', using %u bytes\n"),
                        this->send_hi_water_mark_));
          return -1;
        }
#endif // defined (ACE_LACKS_SO_SNDBUF)
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
TAO_UIPMC_Connection_Handler::resume_handler (void)
{
  return ACE_Event_Handler::ACE_APPLICATION_RESUMES_HANDLER;
}

int
TAO_UIPMC_Connection_Handler::close_connection (void)
{
  return this->close_connection_eh (this);
}

int
TAO_UIPMC_Connection_Handler::handle_input (ACE_HANDLE h)
{
  return this->handle_input_eh (h, this);
}

int
TAO_UIPMC_Connection_Handler::handle_output (ACE_HANDLE handle)
{
  int result =
    this->handle_output_eh (handle, this);

  if (result == -1)
    {
      this->close_connection ();
      return 0;
    }

  return result;
}

int
TAO_UIPMC_Connection_Handler::handle_timeout (const ACE_Time_Value &,
                                              const void *)
{
  // Using this to ensure this instance will be deleted (if necessary)
  // only after reset_state(). Without this, when this refcount==1 -
  // the call to close() will cause a call to remove_reference() which
  // will delete this. At that point this->reset_state() is in no
  // man's territory and that causes SEGV on some platforms (Windows!)

  TAO_Auto_Reference<TAO_UIPMC_Connection_Handler> safeguard (*this);

  // NOTE: Perhaps not the best solution, as it feels like the upper
  // layers should be responsible for this?

  // We don't use this upcall for I/O.  This is only used by the
  // Connector to indicate that the connection timedout.  Therefore,
  // we should call close().
  int ret = this->close ();
  this->reset_state (TAO_LF_Event::LFS_TIMEOUT);
  return ret;
}

int
TAO_UIPMC_Connection_Handler::handle_close (ACE_HANDLE,
                                            ACE_Reactor_Mask)
{
  // No asserts here since the handler is registered with the Reactor
  // and the handler ownership is given to the Reactor.  When the
  // Reactor closes, it will call handle_close() on the handler.  It
  // is however important to overwrite handle_close() to do nothing
  // since the base class does too much.
  return 0;
}

int
TAO_UIPMC_Connection_Handler::close (u_long flags)
{
  return this->close_handler (flags);
}

int
TAO_UIPMC_Connection_Handler::release_os_resources (void)
{
  return this->peer ().close ();
}

int
TAO_UIPMC_Connection_Handler::add_transport_to_cache (void)
{
  ACE_INET_Addr addr;

  // This function is called by the acceptor to add this
  // transport to the transport cache.  This is really
  // important for proper shutdown.  The address used
  // is irrelevent, since UIPMC is connectionless.

  // Construct a UIPMC_Endpoint object.
  TAO_UIPMC_Endpoint endpoint (addr);

  // Construct a property object
  TAO_Base_Transport_Property prop (&endpoint);

  // Add the handler to Cache
  return this->orb_core ()->lane_resources ()
          .transport_cache ().cache_transport (&prop,
                                               this->transport ());
}

int
TAO_UIPMC_Connection_Handler::set_tos (int tos)
{
  // Since only client can send data over MIOP
  // then dscp is only applicable to client socket.
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
                          ACE_TEXT ("TAO (%P|%t) - UIPMC_Connection_Handler::")
                          ACE_TEXT ("set_dscp_codepoint -> IPV6_TCLASS not ")
                          ACE_TEXT ("supported yet\n")));
            }
          return 0;
        }
# else /* !IPV6_TCLASS */
        result = this->peer ().set_option (IPPROTO_IPV6,
                                           IPV6_TCLASS,
                                           (int *) &tos,
                                           (int) sizeof (tos));
      else
# endif /* IPV6_TCLASS */
#endif /* ACE_HAS_IPV6 */
      result = this->peer ().set_option (IPPROTO_IP,
                                         IP_TOS,
                                         (int *) &tos,
                                         (int) sizeof (tos));

      if (TAO_debug_level)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - UIPMC_Connection_Handler::")
                      ACE_TEXT ("set_dscp_codepoint -> dscp: %x; ")
                      ACE_TEXT ("result: %d; %s\n"),
                      tos,
                      result,
                      result == -1 ?
                        ACE_TEXT ("try running as superuser") : ACE_TEXT ("")));
        }

      // On successful setting of TOS field.
      if (result == 0)
        this->dscp_codepoint_ = tos;
    }

  return 0;
}

int
TAO_UIPMC_Connection_Handler::set_dscp_codepoint (CORBA::Long dscp_codepoint)
{
  int tos = IPDSFIELD_DSCP_DEFAULT << 2;

  CORBA::Long codepoint = dscp_codepoint;

  tos = static_cast<int> (codepoint) << 2;

  this->set_tos (tos);

  return 0;
}

int
TAO_UIPMC_Connection_Handler::set_dscp_codepoint (CORBA::Boolean set_network_priority)
{
  int tos = IPDSFIELD_DSCP_DEFAULT << 2;

  if (set_network_priority)
    {
      TAO_Protocols_Hooks *tph =
        this->orb_core ()->get_protocols_hooks ();

      if (tph != 0 )
        {
          CORBA::Long codepoint =
            tph->get_dscp_codepoint ();

          tos = static_cast<int> (codepoint) << 2;
          this->set_tos (tos);
        }
    }

  return 0;
}

int
TAO_UIPMC_Connection_Handler::handle_write_ready (const ACE_Time_Value *t)
{
  return ACE::handle_write_ready (this->peer ().get_handle (), t);
}

TAO_END_VERSIONED_NAMESPACE_DECL
