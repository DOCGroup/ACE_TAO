// $Id$


#include "DIOP_Connection_Handler.h"
#include "tao/Timeprobe.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/CDR.h"
#include "tao/Messaging_Policy_i.h"
#include "tao/Server_Strategy_Factory.h"
#include "DIOP_Transport.h"
#include "DIOP_Endpoint.h"
#include "tao/Connection_Cache_Manager.h"
#include "tao/Base_Connection_Property.h"

#if !defined (__ACE_INLINE__)
# include "DIOP_Connection_Handler.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, DIOP_Connect, "$Id$")



TAO_DIOP_Connection_Handler::TAO_DIOP_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_DIOP_SVC_HANDLER (t, 0 , 0),
    TAO_Connection_Handler (0),
    transport_ (this, 0, 0),
    refcount_ (1),
    tcp_properties_ (0)
{
  // This constructor should *never* get called, it is just here to
  // make the compiler happy: the default implementation of the
  // Creation_Strategy requires a constructor with that signature, we
  // don't use that implementation, but some (most?) compilers
  // instantiate it anyway.
  ACE_ASSERT (this->orb_core () != 0);
}


TAO_DIOP_Connection_Handler::TAO_DIOP_Connection_Handler (TAO_ORB_Core *orb_core,
                                                          CORBA::Boolean flag,
                                                          void *arg)
  : TAO_DIOP_SVC_HANDLER (orb_core->thr_mgr (), 0, 0),
    TAO_Connection_Handler (orb_core),
    transport_ (this, orb_core, flag),
    refcount_ (1),
    tcp_properties_ (ACE_static_cast
                     (TAO_DIOP_Properties *, arg))
{
}


TAO_DIOP_Connection_Handler::~TAO_DIOP_Connection_Handler (void)
{
  // If the socket has not already been closed.
  if (this->get_handle () != ACE_INVALID_HANDLE)
    {
      // Cannot deal with errors, and therefore they are ignored.
      this->transport_.send_buffered_messages ();
    }
  else
    {
      // Dequeue messages and delete message blocks.
      this->transport_.dequeue_all ();
    }

  // @@ Frank: Added from DIOP_Connect.cpp
  this->handle_cleanup ();

  udp_socket_.close ();
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
TAO_DIOP_Connection_Handler::open (void*)
{
   // @@ Frank: Not needed for DIOP
   /*
  if (this->set_socket_option (this->peer (),
                               this->tcp_properties_->send_buffer_size,
                               this->tcp_properties_->recv_buffer_size) == -1)
    return -1;

#if !defined (ACE_LACKS_TCP_NODELAY)

  if (this->peer ().set_option (ACE_IPPROTO_TCP,
                                TCP_NODELAY,
                                (void *) &tcp_properties_->no_delay,
                                sizeof (int)) == -1)
    return -1;
#endif  ! ACE_LACKS_TCP_NODELAY

  if (this->transport_.wait_strategy ()->non_blocking ())
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
                  ACE_TEXT ("TAO (%P|%t) DIOP connection from client")
                  ACE_TEXT ("<%s> on %d\n"),
                  client, this->peer ().get_handle ()));
    }
  */

  // @@ Frank: From DIOP_Connect.cpp
  this->udp_socket_.open (this->local_addr_);

  ACE_DEBUG ((LM_DEBUG,
              "Opened connector on %s:%d\n",
              this->addr_.get_host_name (),
              this->addr_.get_port_number ()));

  return 0;
}

int
TAO_DIOP_Connection_Handler::open_server (void)
{
  this->udp_socket_.open (this->local_addr_);
  ACE_DEBUG ((LM_DEBUG,
              "Opened acceptor on %s:%d\n",
              this->addr_.get_host_name (),
              this->addr_.get_port_number ()));
  return 0;
}

int
TAO_DIOP_Connection_Handler::activate (long flags,
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
                 ACE_TEXT ("TAO (%P|%t) DIOP_Connection_Handler::activate %d ")
                 ACE_TEXT ("threads, flags = %d\n"),
                 n_threads,
                 flags,
                 THR_BOUND));
  // @@ Michael: I believe we do not need active service handlers right now.
  // @@ Frank: Not disabled yet...

  return TAO_DIOP_SVC_HANDLER::activate (flags,
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
TAO_DIOP_Connection_Handler::svc (void)
{
  // @@ Michael: I believe we do not need active service handlers right now.
  // @@ Frank: Not yet...

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
TAO_DIOP_Connection_Handler::handle_close (ACE_HANDLE handle,
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
                 ACE_TEXT ("DIOP_Connection_Handler::handle_close ")
                 ACE_TEXT ("(%d, %d)\n"),
                 handle,
                 rm));

  --this->refcount_;
  if (this->refcount_ == 0 &&
      this->is_registered ())
    {
      // @@ Frank: Added reactor check.  not sure if this is right?
      if (this->reactor ())
      {
         // Make sure there are no timers.
         this->reactor ()->cancel_timer (this);
      }

      // Set the flag to indicate that it is no longer registered with
      // the reactor, so that it isn't included in the set that is
      // passed to the reactor on ORB destruction.
      this->is_registered (0);

      // Close the handle..
      if (this->get_handle () != ACE_INVALID_HANDLE)
        {
          // Send the buffered messages first
          this->transport_.send_buffered_messages ();

          this->peer ().close ();

          // Purge the entry too
          this->mark_invalid ();
        }

      // Decrement the reference count
      this->decr_ref_count ();


    }

  return 0;
}

ACE_HANDLE
TAO_DIOP_Connection_Handler::fetch_handle (void)
{
  return this->get_handle ();
}


int
TAO_DIOP_Connection_Handler::handle_timeout (const ACE_Time_Value &,
                                             const void *)
{
  // This method is called when buffering timer expires.
  //
  ACE_Time_Value *max_wait_time = 0;

  TAO_Stub *stub = 0;
  int has_timeout;
  this->orb_core ()->call_timeout_hook (stub,
                                        has_timeout,
                                        *max_wait_time);

  // Cannot deal with errors, and therefore they are ignored.
  this->transport ()->send_buffered_messages (max_wait_time);

  return 0;
}


int
TAO_DIOP_Connection_Handler::close (u_long)
{
  this->decr_ref_count ();

  return 0;
}


int
TAO_DIOP_Connection_Handler::add_handler_to_cache (void)
{
  ACE_INET_Addr addr;

  // Get the peername.
  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  // Construct an  DIOP_Endpoint object
  TAO_DIOP_Endpoint endpoint (addr,
                              0);

  // Construct a property object
  TAO_Base_Connection_Property prop (&endpoint);

  // Add the handler to Cache
  return this->orb_core ()->connection_cache ().cache_handler (&prop,
                                                               this);
}

// @@ Frank: Hopefully this isn't needed
/*
int
TAO_DIOP_Connection_Handler::process_listen_point_list (
    DIOP::ListenPointList &listen_list)
{
  // Get the size of the list
  CORBA::ULong len = listen_list.length ();

  for (CORBA::ULong i = 0; i < len; ++ i)
    {
      DIOP::ListenPoint listen_point = listen_list[i];
      ACE_INET_Addr addr (listen_point.port,
                          listen_point.host.in ());


      // Construct an  DIOP_Endpoint object
      TAO_DIOP_Endpoint endpoint (addr,
                                  0);

      // Construct a property object
      TAO_Base_Connection_Property prop (&endpoint);

      // Mark the connection as bidirectional
      prop.set_bidir_flag (1);

      // The property for this handler has changed. Recache the
      // handler with this property
      int retval = this->recache_handler (&prop);
      if (retval == -1)
        return retval;

      // Make the handler idle and ready for use
      this->make_idle ();
    }

  return 0;
}
*/

int
TAO_DIOP_Connection_Handler::handle_input (ACE_HANDLE h)
{
  return this->handle_input_i (h);
}


int
TAO_DIOP_Connection_Handler::handle_input_i (ACE_HANDLE,
                                             ACE_Time_Value *max_wait_time)
{
  this->refcount_++;

  // Call the transport read the message
  int result = this->transport_.read_process_message (max_wait_time);

  // Now the message has been read
  if (result == -1 && TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - %p\n"),
                  ACE_TEXT ("DIOP_Connection_Handler::read_message \n")));

    }

  // The upcall is done. Bump down the reference count
  --this->refcount_;
  if (this->refcount_ == 0)
    this->decr_ref_count ();

  if (result == -1)
    return result;
  else if (result == 0)
    // Requires another call to handle_input ()
    return 1;

  return 0;
}

// @@ Frank: From DIOP_Connect.cpp
int
TAO_DIOP_Connection_Handler::handle_cleanup (void)
{
  // Deregister this handler with the ACE_Reactor.
  if (this->reactor ())
    {
      ACE_Reactor_Mask mask =
        ACE_Event_Handler::ALL_EVENTS_MASK | ACE_Event_Handler::DONT_CALL;

      // Make sure there are no timers.
      this->reactor ()->cancel_timer (this);

      // Remove self from reactor.
      this->reactor ()->remove_handler (this, mask);
    }

  return 0;
}




// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
