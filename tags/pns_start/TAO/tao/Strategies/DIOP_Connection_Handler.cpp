// $Id$

#include "DIOP_Connection_Handler.h"

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
#include "tao/Resume_Handle.h"

#include "DIOP_Transport.h"
#include "DIOP_Endpoint.h"

#if !defined (__ACE_INLINE__)
# include "DIOP_Connection_Handler.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, DIOP_Connect, "$Id$")

TAO_DIOP_Connection_Handler::TAO_DIOP_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_DIOP_SVC_HANDLER (t, 0 , 0),
    TAO_Connection_Handler (0),
    tcp_properties_ (0)
{
  // This constructor should *never* get called, it is just here to
  // make the compiler happy: the default implementation of the
  // Creation_Strategy requires a constructor with that signature, we
  // don't use that implementation, but some (most?) compilers
  // instantiate it anyway.
  ACE_ASSERT (0);
}


TAO_DIOP_Connection_Handler::TAO_DIOP_Connection_Handler (TAO_ORB_Core *orb_core,
                                                          CORBA::Boolean flag,
                                                          void *arg)
  : TAO_DIOP_SVC_HANDLER (orb_core->thr_mgr (), 0, 0),
    TAO_Connection_Handler (orb_core),
    tcp_properties_ (ACE_static_cast
                     (TAO_DIOP_Properties *, arg))
{
  TAO_DIOP_Transport* specific_transport = 0;
  ACE_NEW(specific_transport,
          TAO_DIOP_Transport(this, orb_core, flag));

  // store this pointer (indirectly increment ref count)
  this->transport (specific_transport);
}


TAO_DIOP_Connection_Handler::~TAO_DIOP_Connection_Handler (void)
{
  delete this->transport ();
  this->udp_socket_.close ();
}

// DIOP Additions - Begin
ACE_HANDLE
TAO_DIOP_Connection_Handler::get_handle (void) const
{
  return this->udp_socket_.get_handle ();
}


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
  return local_addr_;
}


void
TAO_DIOP_Connection_Handler::local_addr (const ACE_INET_Addr &addr)
{
  local_addr_ = addr;
}


const ACE_SOCK_Dgram &
TAO_DIOP_Connection_Handler::dgram (void)
{
  return this->udp_socket_;
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
  this->udp_socket_.open (this->local_addr_);

  if(TAO_debug_level > 5)
  {
     ACE_DEBUG ((LM_DEBUG,
                 ACE_TEXT("\nTAO (%P|%t) TAO_DIOP_Connection_Handler::open -")
                 ACE_TEXT("listening on: <%s:%u>\n"),
                 ACE_TEXT_CHAR_TO_TCHAR (this->local_addr_.get_host_name ()),
                 this->local_addr_.get_port_number ()));
  }

  // Set that the transport is now connected, if fails we return -1
  // Use C-style cast b/c otherwise we get warnings on lots of
  // compilers
  if (!this->transport ()->post_open ((size_t) this->get_handle ()))
    return -1;

  this->state_changed (TAO_LF_Event::LFS_SUCCESS);

  return 0;
}

int
TAO_DIOP_Connection_Handler::open_server (void)
{
  this->udp_socket_.open (this->local_addr_);
  if( TAO_debug_level > 5)
  {
     ACE_DEBUG ((LM_DEBUG,
                 ACE_TEXT("\nTAO (%P|%t) TAO_DIOP_Connection_Handler::open_server -")
                 ACE_TEXT("listening on %s:%d\n"),
                 ACE_TEXT_CHAR_TO_TCHAR (this->local_addr_.get_host_name ()),
                 this->local_addr_.get_port_number ()
               ));
  }

  this->transport ()->id ((size_t) this->get_handle ());

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
TAO_DIOP_Connection_Handler::handle_output (ACE_HANDLE handle)
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
TAO_DIOP_Connection_Handler::handle_timeout (const ACE_Time_Value &,
                                             const void *)
{
  // We don't use this upcall from the Reactor.  However, we should
  // override this since the base class returns -1 which will result
  // in handle_close() getting called.
  return 0;
}

int
TAO_DIOP_Connection_Handler::handle_close (ACE_HANDLE,
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
TAO_DIOP_Connection_Handler::close (u_long)
{
  return this->close_handler ();
}

int
TAO_DIOP_Connection_Handler::release_os_resources (void)
{
  return this->peer().close ();
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Concurrency_Strategy<TAO_DIOP_Connection_Handler>;
template class ACE_Creation_Strategy<TAO_DIOP_Connection_Handler>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Concurrency_Strategy<TAO_DIOP_Connection_Handler>
#pragma instantiate ACE_Creation_Strategy<TAO_DIOP_Connection_Handler>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */
