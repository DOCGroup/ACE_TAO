// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/PortableGroup/UIPMC_Mcast_Connection_Handler.h"
#include "orbsvcs/PortableGroup/UIPMC_Endpoint.h"
#include "orbsvcs/PortableGroup/UIPMC_Mcast_Transport.h"
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

TAO_UIPMC_Mcast_Connection_Handler::TAO_UIPMC_Mcast_Connection_Handler (
  ACE_Thread_Manager *t)
  : TAO_UIPMC_MCAST_SVC_HANDLER (t, 0 , 0),
    TAO_Connection_Handler (0),
    listen_on_all_(false)
{
  // This constructor should *never* get called, it is just here to
  // make the compiler happy: the default implementation of the
  // Creation_Strategy requires a constructor with that signature, we
  // don't use that implementation, but some (most?) compilers
  // instantiate it anyway.
  ACE_ASSERT (0);
}

TAO_UIPMC_Mcast_Connection_Handler::TAO_UIPMC_Mcast_Connection_Handler (
  TAO_ORB_Core *orb_core)
  : TAO_UIPMC_MCAST_SVC_HANDLER (orb_core->thr_mgr (), 0, 0),
    TAO_Connection_Handler (orb_core),
    listen_on_all_(false)
{
  TAO_UIPMC_Mcast_Transport *specific_transport = 0;
  ACE_NEW(specific_transport,
          TAO_UIPMC_Mcast_Transport (this, orb_core));

  // store this pointer (indirectly increment ref count)
  this->transport (specific_transport);
}

TAO_UIPMC_Mcast_Connection_Handler::~TAO_UIPMC_Mcast_Connection_Handler (void)
{
  delete this->transport ();
  int const result =
    this->release_os_resources ();

  if (result == -1 && TAO_debug_level)
    {
      ORBSVCS_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) - UIPMC_Mcast_Connection_Handler::")
                  ACE_TEXT ("~UIPMC_Mcast_Connection_Handler, ")
                  ACE_TEXT ("release_os_resources() failed '%m'\n")));
    }
}

const ACE_INET_Addr &
TAO_UIPMC_Mcast_Connection_Handler::addr (void) const
{
  return this->addr_;
}

void
TAO_UIPMC_Mcast_Connection_Handler::addr (const ACE_INET_Addr &addr)
{
  this->addr_ = addr;
}

const ACE_INET_Addr &
TAO_UIPMC_Mcast_Connection_Handler::local_addr (void) const
{
  return this->local_addr_;
}

void
TAO_UIPMC_Mcast_Connection_Handler::local_addr (const ACE_INET_Addr &addr)
{
  this->local_addr_ = addr;
}

int
TAO_UIPMC_Mcast_Connection_Handler::open_handler (void *v)
{
  return this->open (v);
}

int
TAO_UIPMC_Mcast_Connection_Handler::open (void*)
{
  if (this->listen_on_all_)
    {
      this->peer ().opts(ACE_SOCK_Dgram_Mcast::OPT_NULLIFACE_ALL | this->peer ().opts());
    }

  TAO_MIOP_Resource_Factory *const factory =
    ACE_Dynamic_Service<TAO_MIOP_Resource_Factory>::instance (
      this->orb_core ()->configuration(),
      ACE_TEXT ("MIOP_Resource_Factory"));
  TAO_DIOP_Protocol_Properties protocol_properties;
  protocol_properties.recv_buffer_size_ =
    factory->receive_buffer_size () ?
    factory->receive_buffer_size () :
    this->orb_core ()->orb_params ()->sock_rcvbuf_size ();

  if (this->peer ().join (this->local_addr_) == 0)
    {
      if (TAO_debug_level > 5)
        {
          char tmp[INET6_ADDRSTRLEN];
          this->local_addr_.get_host_addr (tmp, sizeof tmp);
          ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - UIPMC_Mcast_Connection_Handler::open, ")
                      ACE_TEXT("subscribed to multicast group at %C:%u\n"),
                      tmp,
                      this->local_addr_.get_port_number ()
                  ));
        }
    }
#ifndef ALLOW_UNICAST_MIOP
  else
    {
      char tmp[INET6_ADDRSTRLEN];
      this->local_addr_.get_host_addr (tmp, sizeof tmp);
      ORBSVCS_DEBUG ((LM_ERROR,
                  ACE_TEXT("TAO (%P|%t) - UIPMC_Mcast_Connection_Handler::open, ")
                  ACE_TEXT("failed to subscribe to multicast group at %C:%u '%m'\n"),
                  tmp,
                  this->local_addr_.get_port_number ()
              ));
      return -1;
    }
#endif // ALLOW_UNICAST_MIOP

  if (this->set_socket_option (this->peer (),
                               0,
                               protocol_properties.recv_buffer_size_) == -1)
    {
      return -1;
    }

  // The socket has to be set in non-blocking mode in order to work with
  // TAO_UIPMC_Mcast_Transport::handle_input().
  if (this->peer ().enable (ACE_NONBLOCK) == -1)
    {
      if (TAO_debug_level)
        ORBSVCS_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) - UIPMC_Mcast_Connection_Handler::")
                    ACE_TEXT ("open, failed to set to non-blocking mode ")
                    ACE_TEXT ("'%m'\n")));

      return -1;
    }

  this->transport ()->id ((size_t) this->peer ().get_handle ());
  return 0;
}

int
TAO_UIPMC_Mcast_Connection_Handler::resume_handler (void)
{
  return ACE_Event_Handler::ACE_APPLICATION_RESUMES_HANDLER;
}

int
TAO_UIPMC_Mcast_Connection_Handler::close_connection (void)
{
  return this->close_connection_eh (this);
}

int
TAO_UIPMC_Mcast_Connection_Handler::handle_input (ACE_HANDLE h)
{
  return this->handle_input_eh (h, this);
}

int
TAO_UIPMC_Mcast_Connection_Handler::handle_output (ACE_HANDLE handle)
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
TAO_UIPMC_Mcast_Connection_Handler::handle_timeout (const ACE_Time_Value &,
                                                        const void *)
{
  // Using this to ensure this instance will be deleted (if necessary)
  // only after reset_state(). Without this, when this refcount==1 -
  // the call to close() will cause a call to remove_reference() which
  // will delete this. At that point this->reset_state() is in no
  // man's territory and that causes SEGV on some platforms (Windows!)

  TAO_Auto_Reference<TAO_UIPMC_Mcast_Connection_Handler> safeguard (*this);

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
TAO_UIPMC_Mcast_Connection_Handler::handle_close (ACE_HANDLE,
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
TAO_UIPMC_Mcast_Connection_Handler::close (u_long flags)
{
  return this->close_handler (flags);
}

int
TAO_UIPMC_Mcast_Connection_Handler::release_os_resources (void)
{
  return this->peer ().close ();
}

int
TAO_UIPMC_Mcast_Connection_Handler::add_transport_to_cache (void)
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
TAO_UIPMC_Mcast_Connection_Handler::handle_write_ready
  (const ACE_Time_Value *t)
{
  return ACE::handle_write_ready (this->peer ().get_handle (), t);
}

void
TAO_UIPMC_Mcast_Connection_Handler::listen_on_all(bool value)
{
  this->listen_on_all_ = value;
}

TAO_END_VERSIONED_NAMESPACE_DECL
