// Event_Handler.cpp
// $Id$

#include "ace/Event_Handler.h"
#include "ace/Message_Block.h"
#include "ace/OS_Errno.h"
#include "ace/Reactor.h"
#include "ace/Thread_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/Event_Handler.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Event_Handler, "$Id$")

// Implement conceptually abstract virtual functions in the base class
// so derived classes don't have to implement unused ones.

ACE_Event_Handler::ACE_Event_Handler (ACE_Reactor *r,
                                      int p)
  : reference_count_ (1)
  , priority_ (p)
  , reactor_ (r)
  , reference_counting_policy_ (Reference_Counting_Policy::DISABLED)
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
  return ACE_Event_Handler::ACE_REACTOR_RESUMES_HANDLER;
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

ACE_Reactor_Timer_Interface *
ACE_Event_Handler::reactor_timer_interface (void) const
{
  ACE_TRACE ("ACE_Event_Handler::reactor_timer_interface");
  return this->reactor_;
}

ACE_Event_Handler::Reference_Count
ACE_Event_Handler::add_reference (void)
{
  return ++this->reference_count_;
}

ACE_Event_Handler::Reference_Count
ACE_Event_Handler::remove_reference (void)
{
  Reference_Count result =
    --this->reference_count_;

  if (result == 0)
    delete this;

  return result;
}

ACE_Event_Handler::Policy::~Policy (void)
{
}

ACE_Event_Handler::Reference_Counting_Policy::Reference_Counting_Policy (Reference_Counting_Policy::Value value)
  : value_ (value)
{
}

ACE_Event_Handler::Reference_Counting_Policy::Value
ACE_Event_Handler::Reference_Counting_Policy::value (void) const
{
  return this->value_;
}

void
ACE_Event_Handler::Reference_Counting_Policy::value (ACE_Event_Handler::Reference_Counting_Policy::Value value)
{
  this->value_ = value;
}

ACE_Event_Handler::Reference_Counting_Policy &
ACE_Event_Handler::reference_counting_policy (void)
{
  return this->reference_counting_policy_;
}

#if !defined (ACE_HAS_WINCE)

ACE_THR_FUNC_RETURN
ACE_Event_Handler::read_adapter (void *args)
{
  ACE_Event_Handler *this_ptr = (ACE_Event_Handler *) args;

  ACE_HANDLE handle = this_ptr->get_handle ();
  if (handle == ACE_INVALID_HANDLE)
    handle = ACE_STDIN;

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

ACE_Event_Handler_var::ACE_Event_Handler_var (void)
  : ptr_ (0)
{
}

ACE_Event_Handler_var::ACE_Event_Handler_var (ACE_Event_Handler *p)
  : ptr_ (p)
{
}

ACE_Event_Handler_var::ACE_Event_Handler_var (const ACE_Event_Handler_var &b)
  : ptr_ (b.ptr_)
{
  if (this->ptr_ != 0)
    {
      this->ptr_->add_reference ();
    }
}

ACE_Event_Handler_var::~ACE_Event_Handler_var (void)
{
  if (this->ptr_ != 0)
    {
      this->ptr_->remove_reference ();
    }
}

ACE_Event_Handler_var &
ACE_Event_Handler_var::operator= (ACE_Event_Handler *p)
{
  if (this->ptr_ == p)
    return *this;

  if (this->ptr_ != 0)
    this->ptr_->remove_reference ();

  this->ptr_ = p;

  return *this;
}

ACE_Event_Handler_var &
ACE_Event_Handler_var::operator= (const ACE_Event_Handler_var &b)
{
  if (this->ptr_ != b.ptr_)
    {
      if (this->ptr_ != 0)
        {
          this->ptr_->remove_reference ();
        }

      if ((this->ptr_ = b.ptr_) != 0)
        {
          this->ptr_->add_reference ();
        }
    }

  return *this;
}

ACE_Event_Handler *
ACE_Event_Handler_var::operator->() const
{
  return this->ptr_;
}

ACE_Event_Handler *
ACE_Event_Handler_var::handler (void) const
{
  return this->ptr_;
}

ACE_Event_Handler *
ACE_Event_Handler_var::release (void)
{
  ACE_Event_Handler *old = this->ptr_;
  this->ptr_ = 0;
  return old;
}

void
ACE_Event_Handler_var::reset (ACE_Event_Handler *p)
{
  *this = p;
}

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
