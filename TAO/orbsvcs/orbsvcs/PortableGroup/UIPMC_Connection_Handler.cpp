// This may look like C, but it's really -*- C++ -*-
//
// $Id$


#include "UIPMC_Connection_Handler.h"

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

#include "UIPMC_Transport.h"
#include "UIPMC_Endpoint.h"

#if !defined (__ACE_INLINE__)
# include "UIPMC_Connection_Handler.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, UIPMC_Connect, "$Id$")

TAO_UIPMC_Connection_Handler::TAO_UIPMC_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_UIPMC_SVC_HANDLER (t, 0 , 0),
    TAO_Connection_Handler (0),
    using_mcast_ (0),
    uipmc_properties_ (0)
{
  // This constructor should *never* get called, it is just here to
  // make the compiler happy: the default implementation of the
  // Creation_Strategy requires a constructor with that signature, we
  // don't use that implementation, but some (most?) compilers
  // instantiate it anyway.
  ACE_ASSERT (0);
}


TAO_UIPMC_Connection_Handler::TAO_UIPMC_Connection_Handler (TAO_ORB_Core *orb_core,
                                                            void *arg)
  : TAO_UIPMC_SVC_HANDLER (orb_core->thr_mgr (), 0, 0),
    TAO_Connection_Handler (orb_core),
    using_mcast_ (0),
    uipmc_properties_ (ACE_static_cast
                       (TAO_UIPMC_Properties *, arg))
{
  TAO_UIPMC_Transport* specific_transport = 0;
  ACE_NEW(specific_transport,
          TAO_UIPMC_Transport(this, orb_core, 0));

  // store this pointer (indirectly increment ref count)
  this->transport (specific_transport);
}


TAO_UIPMC_Connection_Handler::~TAO_UIPMC_Connection_Handler (void)
{
  if (this->using_mcast_)
    {
      // Closing a multicast socket automatically unsubscribes us from
      // the multicast group.
      this->mcast_socket_.close ();
    }
  else
    this->udp_socket_.close ();

  delete this->transport ();
}

ACE_HANDLE
TAO_UIPMC_Connection_Handler::get_handle (void) const
{
  if (this->using_mcast_)
    return this->mcast_socket_.get_handle ();
  else
    return this->udp_socket_.get_handle ();
}

const ACE_INET_Addr &
TAO_UIPMC_Connection_Handler::addr (void)
{
  return this->addr_;
}


void
TAO_UIPMC_Connection_Handler::addr (const ACE_INET_Addr &addr)
{
  this->addr_ = addr;
}


const ACE_INET_Addr &
TAO_UIPMC_Connection_Handler::local_addr (void)
{
  return local_addr_;
}


void
TAO_UIPMC_Connection_Handler::local_addr (const ACE_INET_Addr &addr)
{
  local_addr_ = addr;
}


const ACE_SOCK_Dgram &
TAO_UIPMC_Connection_Handler::dgram (void)
{
  return this->udp_socket_;
}

const ACE_SOCK_Dgram_Mcast &
TAO_UIPMC_Connection_Handler::mcast_dgram (void)
{
  return this->mcast_socket_;
}


int
TAO_UIPMC_Connection_Handler::open_handler (void *v)
{
  return this->open (v);
}

int
TAO_UIPMC_Connection_Handler::open (void*)
{
  this->udp_socket_.open (this->local_addr_);

  if(TAO_debug_level > 5)
  {
     ACE_DEBUG ((LM_DEBUG,
                 ACE_TEXT("\nTAO (%P|%t) TAO_UIPMC_Connection_Handler::open -")
                 ACE_TEXT("listening on: <%s:%u>\n"),
                 this->local_addr_.get_host_name (),
                 this->local_addr_.get_port_number ()));
  }

  // Set the id in the transport now that we're active.
  this->transport ()->id ((size_t) this->udp_socket_.get_handle ());
  this->using_mcast_ = 0;

  return 0;
}

int
TAO_UIPMC_Connection_Handler::open_server (void)
{
  this->mcast_socket_.subscribe (this->local_addr_);
  if( TAO_debug_level > 5)
  {
     ACE_DEBUG ((LM_DEBUG,
                 ACE_TEXT("\nTAO (%P|%t) TAO_UIPMC_Connection_Handler::open_server -")
                 ACE_TEXT("subcribed to multicast group at %s:%d\n"),
                 this->local_addr_.get_host_name (),
                 this->local_addr_.get_port_number ()
               ));
  }

  this->transport ()->id ((size_t) this->mcast_socket_.get_handle ());
  this->using_mcast_ = 1;

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
  int result =
    this->handle_input_eh (h, this);

  if (result == -1)
    {
      this->close_connection ();
      return 0;
    }

  return result;
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
  // We don't use this upcall for I/O.  This is only used by the
  // Connector to indicate that the connection timedout.  Therefore,
  // we should call close().
  return this->close ();
}

int
TAO_UIPMC_Connection_Handler::handle_close (ACE_HANDLE,
                                            ACE_Reactor_Mask)
{
  ACE_ASSERT (0);
  return 0;
}

int
TAO_UIPMC_Connection_Handler::close (u_long)
{
  this->state_changed (TAO_LF_Event::LFS_CONNECTION_CLOSED);
  this->transport ()->remove_reference ();
  return 0;
}

int
TAO_UIPMC_Connection_Handler::release_os_resources (void)
{
  return this->peer().close ();
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

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Concurrency_Strategy<TAO_UIPMC_Connection_Handler>;
template class ACE_Creation_Strategy<TAO_UIPMC_Connection_Handler>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Concurrency_Strategy<TAO_UIPMC_Connection_Handler>
#pragma instantiate ACE_Creation_Strategy<TAO_UIPMC_Connection_Handler>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
