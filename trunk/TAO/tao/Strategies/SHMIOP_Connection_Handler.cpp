// $Id$

#include "SHMIOP_Connection_Handler.h"

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "tao/Timeprobe.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/CDR.h"
#include "tao/Messaging_Policy_i.h"
#include "tao/GIOP_Message_Base.h"
#include "tao/GIOP_Message_Lite.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Transport_Cache_Manager.h"
#include "SHMIOP_Endpoint.h"

#if !defined (__ACE_INLINE__)
# include "SHMIOP_Connection_Handler.inl"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(Strategies, SHMIOP_Connection_Handler, "$Id$")

TAO_SHMIOP_Connection_Handler::TAO_SHMIOP_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_SHMIOP_SVC_HANDLER (t, 0 , 0),
    TAO_Connection_Handler (0),
    pending_upcalls_ (1)
{
  // This constructor should *never* get called, it is just here to
  // make the compiler happy: the default implementation of the
  // Creation_Strategy requires a constructor with that signature, we
  // don't use that implementation, but some (most?) compilers
  // instantiate it anyway.
  ACE_ASSERT (this->orb_core () != 0);
}


TAO_SHMIOP_Connection_Handler::TAO_SHMIOP_Connection_Handler (TAO_ORB_Core *orb_core,
                                                              CORBA::Boolean /*flag*/,
                                                          void *)
  : TAO_SHMIOP_SVC_HANDLER (orb_core->thr_mgr (), 0, 0),
    TAO_Connection_Handler (orb_core),
    pending_upcalls_ (1)
{
  TAO_SHMIOP_Transport* specific_transport = 0;
  ACE_NEW (specific_transport,
          TAO_SHMIOP_Transport(this, orb_core, 0));

  // store this pointer (indirectly increment ref count)
  this->transport (specific_transport);
  TAO_Transport::release (specific_transport);
}


TAO_SHMIOP_Connection_Handler::~TAO_SHMIOP_Connection_Handler (void)
{
}

int
TAO_SHMIOP_Connection_Handler::open (void*)
{
  if (this->set_socket_option (this->peer (),
                               this->orb_core ()->orb_params ()->sock_sndbuf_size (),
                               this->orb_core ()->orb_params ()->sock_rcvbuf_size ())
      == -1)
    return -1;
#if !defined (ACE_LACKS_TCP_NODELAY)

  int nodelay =
    this->orb_core ()->orb_params ()->nodelay ();

  if (this->peer ().set_option (ACE_IPPROTO_TCP,
                                TCP_NODELAY,
                                (void *) &nodelay,
                                sizeof (int)) == -1)
    return -1;
#endif /* ! ACE_LACKS_TCP_NODELAY */

  if (this->transport ()->wait_strategy ()->non_blocking ())
    {
      if (this->peer ().enable (ACE_NONBLOCK) == -1)
        return -1;
    }

  // Called by the <Strategy_Acceptor> when the handler is
  // completely connected.
  ACE_INET_Addr addr;

  char client[MAXHOSTNAMELEN + 16];

  // Get the peername.
  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  // Verify that we can resolve the peer hostname.
  else if (addr.addr_to_string (client, sizeof (client)) == -1)
    return -1;

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) SHMIOP connection from client")
                  ACE_TEXT ("<%s> on %d\n"),
                  client, this->peer ().get_handle ()));
    }

  // Set the id in the transport now that we're active.
  this->transport ()->id ((int) this->get_handle ());

  return 0;
}

int
TAO_SHMIOP_Connection_Handler::activate (long flags,
                                       int n_threads,
                                       int force_active,
                                       long priority,
                                       int grp_id,
                                       ACE_Task_Base *task,
                                       ACE_hthread_t thread_handles[],
                                       void *stack[],
                                       size_t stack_size[],
                                       ACE_thread_t  thread_names[])
{
  if (TAO_debug_level)
    ACE_DEBUG  ((LM_DEBUG,
                 ACE_TEXT ("TAO (%P|%t) SHMIOP_Connection_Handler::activate %d ")
                 ACE_TEXT ("threads, flags = %d\n"),
                 n_threads,
                 flags,
                 THR_BOUND));

  // Set the id in the transport now that we're active.
  this->transport ()->id ((int) this->get_handle ());

  return TAO_SHMIOP_SVC_HANDLER::activate (flags,
                                         n_threads,
                                         force_active,
                                         priority,
                                         grp_id,
                                         task,
                                         thread_handles,
                                         stack,
                                         stack_size,
                                         thread_names);
}

int
TAO_SHMIOP_Connection_Handler::svc (void)
{
  // This method is called when an instance is "activated", i.e.,
  // turned into an active object.  Presumably, activation spawns a
  // thread with this method as the "worker function".

  // Clear the non-blocking mode here
  ACE_Flag_Manip::clr_flags (this->get_handle (),
                             ACE_NONBLOCK);

  // Call the implementation here
  return this->svc_i ();
}


int
TAO_SHMIOP_Connection_Handler::handle_close (ACE_HANDLE handle,
                                             ACE_Reactor_Mask rm)
{
  // @@ Alex: we need to figure out if the transport decides to close
  //    us or something else.  If it is something else (for example
  //    the cached connector trying to make room for other
  //    connections) then we should let the transport know, so it can
  //    in turn take appropiate action (such as sending exceptions to
  //    all waiting reply handlers).
  if (TAO_debug_level)
    ACE_DEBUG  ((LM_DEBUG,
                 ACE_TEXT ("TAO (%P|%t) ")
                 ACE_TEXT ("SHMIOP_Connection_Handler::handle_close ")
                 ACE_TEXT ("(%d, %d)\n"),
                 handle,
                 rm));

  --this->pending_upcalls_;
  if (this->pending_upcalls_ <= 0)
    {
      if (this->transport ()->wait_strategy ()->is_registered ())
        {
          // Make sure there are no timers.
          this->reactor ()->cancel_timer (this);

          // Set the flag to indicate that it is no longer registered with
          // the reactor, so that it isn't included in the set that is
          // passed to the reactor on ORB destruction.
          this->transport ()->wait_strategy ()->is_registered (0);
        }

      // Close the handle..
      if (this->get_handle () != ACE_INVALID_HANDLE)
        {
          // Mark the entry as invalid
          this->transport ()->mark_invalid ();

          // Signal the transport that we will no longer have
          // a reference to it.  This will eventually call
          // TAO_Transport::release ().
          this->transport (0);
        }

      this->destroy ();
    }

  return 0;
}

ACE_HANDLE
TAO_SHMIOP_Connection_Handler::fetch_handle (void)
{
  return this->get_handle ();
}

int
TAO_SHMIOP_Connection_Handler::handle_output (ACE_HANDLE)
{
  return this->transport ()->handle_output ();
}

int
TAO_SHMIOP_Connection_Handler::add_transport_to_cache (void)
{
  ACE_INET_Addr addr;

  // Get the peername.
  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  // Construct an  SHMIOP_Endpoint object
  TAO_SHMIOP_Endpoint endpoint (
      addr,
      this->orb_core()->orb_params()->use_dotted_decimal_addresses ());

  // Construct a property object
  TAO_Base_Transport_Property prop (&endpoint);

  // Add the handler to Cache
  return this->orb_core ()->transport_cache ()->cache_transport (&prop,
                                                                 this->transport ());
}


int
TAO_SHMIOP_Connection_Handler::handle_input (ACE_HANDLE h)
{
  return this->handle_input_i (h);
}


int
TAO_SHMIOP_Connection_Handler::handle_input_i (ACE_HANDLE,
                                             ACE_Time_Value *max_wait_time)
{
  this->pending_upcalls_++;

  // Call the transport read the message
  int result = this->transport ()->read_process_message (max_wait_time);

  // Now the message has been read
  if (result == -1 && TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - %p\n"),
                  ACE_TEXT ("SHMIOP_Connection_Handler::read_message \n")));

    }

  // The upcall is done. Bump down the reference count
  if (--this->pending_upcalls_ <= 0)
    result = -1;

  if (result == 0 || result == -1)
    {
      return result;
    }

  return 0;
}



// ****************************************************************
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Svc_Handler<ACE_MEM_STREAM, ACE_NULL_SYNCH>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Svc_Handler<ACE_MEM_STREAM, ACE_NULL_SYNCH>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /*(TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0) */
