// Event_Handler.cpp
// $Id$

#include "ace/Demux/Event_Handler.h"
#include "ace/Utils/Message_Block.h"
#include "ace/OS/OS_Errno.h"
#include "ace/Demux/Reactor.h"
#include "ace/Threads/Thread_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/Demux/Event_Handler.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Event_Handler, "$Id$")

// Implement conceptually abstract virtual functions in the base class
// so derived classes don't have to implement unused ones.

ACE_Event_Handler::ACE_Event_Handler (ACE_Reactor *r,
                                      int p)
  : priority_ (p),
    reactor_ (r)
{
  // ACE_TRACE ("ACE_Event_Handler::ACE_Event_Handler");
}

ACE_Event_Handler::~ACE_Event_Handler (void)
{
  // ACE_TRACE ("ACE_Event_Handler::~ACE_Event_Handler");
  if (this->reactor_ != 0)
    {
      ACE_Errno_Guard guard (errno);     // purge may get ENOTSUP
      this->reactor_->purge_pending_notifications (this);
    }
}

// Gets the file descriptor associated with this I/O device.

ACE_HANDLE
ACE_Event_Handler::get_handle (void) const
{
  ACE_TRACE ("ACE_Event_Handler::get_handle");
  return ACE_INVALID_HANDLE;
}

// Sets the file descriptor associated with this I/O device.

void
ACE_Event_Handler::set_handle (ACE_HANDLE)
{
  ACE_TRACE ("ACE_Event_Handler::set_handle");
}

// Gets the priority of this handler.

int
ACE_Event_Handler::priority (void) const
{
  ACE_TRACE ("ACE_Event_Handler::priority");
  return this->priority_;
}

// Sets the priority

void
ACE_Event_Handler::priority (int priority)
{
  ACE_TRACE ("ACE_Event_Handler::priority");
  this->priority_ = priority;
}

// Called when the object is about to be removed from the Dispatcher
// tables.

int
ACE_Event_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_TRACE ("ACE_Event_Handler::handle_close");
  return -1;
}

// Called when input becomes available on fd.

int
ACE_Event_Handler::handle_input (ACE_HANDLE)
{
  ACE_TRACE ("ACE_Event_Handler::handle_input");
  return -1;
}

// Called when output is possible on fd.

int
ACE_Event_Handler::handle_output (ACE_HANDLE)
{
  ACE_TRACE ("ACE_Event_Handler::handle_output");
  return -1;
}

// Called when urgent data is available on fd.

int
ACE_Event_Handler::handle_exception (ACE_HANDLE)
{
  ACE_TRACE ("ACE_Event_Handler::handle_exception");
  return -1;
}

// Called when timer expires, TV stores the current time.

int
ACE_Event_Handler::handle_timeout (const ACE_Time_Value &, const void *)
{
  ACE_TRACE ("ACE_Event_Handler::handle_timeout");
  return -1;
}

// Called when a monitored Process exits

int
ACE_Event_Handler::handle_exit (ACE_Process *)
{
  ACE_TRACE ("ACE_Event_Handler::handle_exit");
  return -1;
}

// Called when a registered signal occurs.

int
ACE_Event_Handler::handle_signal (int, siginfo_t *, ucontext_t *)
{
  ACE_TRACE ("ACE_Event_Handler::handle_signal");
  return -1;
}

int
ACE_Event_Handler::resume_handler (void)
{
  ACE_TRACE ("ACE_Event_Handler::resume_handler");

  // Return a default value and allow the reactor to take care of
  // resuming the handler
  return 0;
}


int
ACE_Event_Handler::handle_qos (ACE_HANDLE)
{
  ACE_TRACE ("ACE_Event_Handler::handle_qos");
  return -1;
}

int
ACE_Event_Handler::handle_group_qos (ACE_HANDLE)
{
  ACE_TRACE ("ACE_Event_Handler::handle_group_qos");
  return -1;
}

void
ACE_Event_Handler::reactor (ACE_Reactor *reactor)
{
  ACE_TRACE ("ACE_Event_Handler::reactor");
  this->reactor_ = reactor;
}

ACE_Reactor *
ACE_Event_Handler::reactor (void) const
{
  ACE_TRACE ("ACE_Event_Handler::reactor");
  return this->reactor_;
}

#if !defined (ACE_HAS_WINCE)

// Used to read from non-socket ACE_HANDLEs in our own thread to work
// around Win32 limitations that don't allow us to select() on
// non-sockets (such as ACE_STDIN).  This is commonly used in
// situations where the Reactor is used to demultiplex read events on
// ACE_STDIN on UNIX.  Note that <event_handler> must be a subclass of
// <ACE_Event_Handler>.  If the <get_handle> method of this event
// handler returns <ACE_INVALID_HANDLE> we default to reading from
// ACE_STDIN.

void *
ACE_Event_Handler::read_adapter (void *args)
{
  ACE_Event_Handler *this_ptr = (ACE_Event_Handler *) args;
  ACE_HANDLE handle = ACE_STDIN;

  while (this_ptr->handle_input (handle) != -1)
    continue;

  this_ptr->handle_close (handle,
                          ACE_Event_Handler::READ_MASK);
  this_ptr->reactor ()->notify ();

  return 0;
}

int
ACE_Event_Handler::register_stdin_handler (ACE_Event_Handler *eh,
					   ACE_Reactor *reactor,
					   ACE_Thread_Manager *thr_mgr,
					   int flags)
{
#if defined (ACE_WIN32) || defined (ACE_PSOS)
  ACE_UNUSED_ARG (reactor);

  eh->reactor (reactor);
  return thr_mgr->spawn (&read_adapter, (void *) eh, flags);
#else
  // Keep compilers happy.
  ACE_UNUSED_ARG (flags);
  ACE_UNUSED_ARG (thr_mgr);
  return reactor->register_handler (ACE_STDIN,
                                    eh,
                                    ACE_Event_Handler::READ_MASK);
#endif /* ACE_WIN32 || ACE_PSOS */
}

int
ACE_Event_Handler::remove_stdin_handler (ACE_Reactor *reactor,
					 ACE_Thread_Manager *thr_mgr)
{
#if defined (ACE_WIN32)
  ACE_UNUSED_ARG (reactor);
  ACE_UNUSED_ARG (thr_mgr);

  // What should we do here?
  ACE_NOTSUP_RETURN (-1);
#else
  // Keep compilers happy.
  ACE_UNUSED_ARG (thr_mgr);
  return reactor->remove_handler (ACE_STDIN,
                                  ACE_Event_Handler::READ_MASK);
#endif /* ACE_WIN32 */
}

#endif /* ACE_HAS_WINCE */

ACE_Notification_Buffer::ACE_Notification_Buffer (void)
{
  ACE_TRACE ("ACE_Notification_Buffer::ACE_Notification_Buffer");
}

ACE_Notification_Buffer::ACE_Notification_Buffer (ACE_Event_Handler *eh,
						  ACE_Reactor_Mask mask)
  : eh_ (eh),
    mask_ (mask)
{
  ACE_TRACE ("ACE_Notification_Buffer::ACE_Notification_Buffer");
}
