// $Id$
/*
// ============================================================================
//
// = LIBRARY
//     ACE
// 
// = FILENAME
//     CLASSIX_Reactor.cpp
//
// = AUTHOR(S)
//     Nokia Telecommunications
// 
// ============================================================================
*/
#include "ace/CLASSIX/CLASSIX_Select_Reactor.h"

#if !defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_Select_Reactor.i"
#endif /* __ACE_INLINE__ */

#include "ace/CLASSIX/CLASSIX_OS.h"
/* ------------------------------------------------------------------------- */
int
ACE_CLASSIX_Select_Reactor::wait_for_multiple_events 
(ACE_Select_Reactor_Handle_Set &theDispatchSet,
 ACE_Time_Value *max_wait_time)
{

    ACE_Time_Value timer_buf (0);
    ACE_Time_Value *this_timeout = &timer_buf;

    int number_of_active_handles = this->any_ready (theDispatchSet);

    // If there are any bits enabled in the <ready_set_> then we'll
    // handle those first, otherwise we'll block in select().

    if (number_of_active_handles == 0)
    {
	int port = K_ANYENABLED;
	do
	{
	    // Monitor all enabled ports
	    // CLASSIX uses -1 rathre than 0 for blocked receive
	    int msec = -1;
	    if (this->timer_queue_->calculate_timeout (max_wait_time,
						       this_timeout) != 0)
	    {
		if ((msec = this_timeout->msec()) == 0)
		{
		    msec = -1;
		    this_timeout = 0;
		}
	    }
	    else
		this_timeout = 0;

	    ACE_CLASSIX_Msg rmsg(0, 0);
	    port = K_ANYENABLED;
	    ssize_t size = ::ipcReceive(rmsg.get(), &port, msec);
#if 0
	    ACE_DEBUG((LM_DEBUG, 
		       "(%t)ACE_CLASSIX_Select_Reactor::"
		       "return from ipcReceive():ret = %d"
		       ", port = %d, timeout = %d\n", 
		       size, port, msec));
#endif
	    if (size >= 0)
	    {
		// Is 0 valid ???
		// Keep info about which handler this message is for and
		// its size.
		if (this->set_current_info_(port, size) == 0)
		{
		    theDispatchSet.rd_mask_.set_bit(port);
		    number_of_active_handles = 1;
		}
		else
		{
		    ACE_DEBUG((LM_DEBUG, 
			       "Synchronization problem in Reactor???\n"));
		    number_of_active_handles = -1;
		    errno = K_EFAULT;
		}
	    }
	    else 
	    {
		// make the current message information invalid
		this->set_current_info_(ACE_INVALID_HANDLE, 0);
		if ((errno = size) == K_ETIMEOUT)
		    number_of_active_handles = 0;
		else
		    number_of_active_handles = -1;
	    }
	}
	while (number_of_active_handles == -1 && 
	       this->handle_error_ (port) > 0);
    }
    // Return the number of events to dispatch.
    return number_of_active_handles;
}

int
ACE_CLASSIX_Select_Reactor::set_current_info_(ACE_HANDLE thePort, 
					      size_t theSize)
{
    ACE_MT(ACE_GUARD_RETURN (ACE_SELECT_REACTOR_MUTEX, ace_mon, 
			     this->token_, -1));

    this->current_handle_     = thePort;
    this->current_msg_size_   = theSize;
    return 0;
}

int 
ACE_CLASSIX_Select_Reactor::current_info(ACE_HANDLE thePort, 
					     size_t& theSize)
{
    ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_MUTEX, 
			      ace_mon, this->token_, -1));

    if (this->current_handle_ == thePort)
    {
	theSize = this->current_msg_size_;
	this->current_msg_size_ = 0;
	this->current_handle_   = ACE_INVALID_HANDLE;
	return 0;
    }
    else
    {
	theSize = 0;
	return -1;
    }
}

int
ACE_CLASSIX_Select_Reactor::handle_error_ (int thePort)
{

    // The thread has been aborted
    if (errno == K_EABORT)
	return this->restart_;
    // No port or a (Chorus) handler is attached to the port
    else if (errno == K_ENOPORT || errno == K_EINVAL)
	return this->check_handles_ (thePort);
    else
	return -1;
}

int
ACE_CLASSIX_Select_Reactor::check_handles_ (int thePort)
{
    ACE_TRACE ("ACE_Select_Reactor::check_handles");
    if (thePort == K_ANYENABLED)
	return -1;
    else
    // Don't know how to check if a Chorus port has been disabled or deleted.
	return 0;
}

/* ------------------------------------------------------------------------- */
void
ACE_CLASSIX_Select_Reactor_Notify::dump (void) const
{
  ACE_TRACE ("ACE_CLASSIX_Select_Reactor_Notify::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("ACE_CLASSIX_select_reactor_ = %x"), 
	      this->select_reactor_));
  this->notification_sap_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

int
ACE_CLASSIX_Select_Reactor_Notify::open (ACE_Reactor_Impl *theReactor,
					 ACE_Timer_Queue*,
					 int the_disable_notify_pipe)
{
    if (the_disable_notify_pipe == 0)
    {
	this->select_reactor_ =  ACE_dynamic_cast 
	    (ACE_CLASSIX_Select_Reactor *, theReactor);

	if (this->notification_sap_.open (&this->notification_port_) != 0 ||
	    this->notification_sap_.selectable() != 0)
	    return -1;

	return this->select_reactor_->register_handler
	    (this->notification_sap_.get_handle (),
	     this,
	     ACE_Event_Handler::READ_MASK);
    }
    else
    {
	this->select_reactor_ = 0;
    }
}

ssize_t
ACE_CLASSIX_Select_Reactor_Notify::notify (ACE_Event_Handler *eh,
                                   ACE_Reactor_Mask mask,
                                   ACE_Time_Value *timeout)
{
    ACE_Notification_Buffer buffer (eh, mask);
    ACE_CLASSIX_Msg msg(&buffer, sizeof (buffer));
    KnIpcDest dest;
    dest.target = this->notification_sap_.get_addr().get_id();

    ssize_t n = ipcSend (msg.get(), K_DEFAULTPORT, &dest);
    if (n < 0)
	ACE_DEBUG((LM_DEBUG, "ipcSend() error = %d\n", n));
    return n == 0 ? 0 : -1;
}

// Handles pending threads (if any) that are waiting to unblock the
// Select_Reactor.

int
ACE_CLASSIX_Select_Reactor_Notify::dispatch_notifications (
    int & number_of_active_handles,
    const ACE_Handle_Set &rd_mask)
{
  ACE_TRACE ("(%t) ACE_Select_Reactor_Notify::handle_notification");

  ACE_HANDLE read_handle =
      this->notification_sap_.get_handle ();

  if (rd_mask.is_set (read_handle))
    {
	number_of_active_handles--;
	return this->handle_input (read_handle);
    }
  else
    return 0;
}

// Special trick to unblock select() when updates occur in somewhere
// other than the main ACE_Select_Reactor thread.  All we do is write data to
// a pipe that the ACE_Select_Reactor is listening on.  Thanks to Paul
// Stephenson for suggesting this approach.

int
ACE_CLASSIX_Select_Reactor_Notify::handle_input (ACE_HANDLE handle)
{
    // Precondition: this->select_reactor_.token_.current_owner () ==
    // ACE_Thread::self ();

    if (handle != this->notification_sap_.get_handle())
    {
	ACE_DEBUG((LM_DEBUG, "ACE_CLASSIX_Select_Reator_Notify::"
		   "handle_input() Not my handle\n"));
	return 0;
    }

    ssize_t                 n = 0;  
    size_t                  n1= 0;

    int                     number_dispatched = 0;

    ACE_Notification_Buffer buffer;
    ACE_CLASSIX_Msg rmsg(&buffer, sizeof (buffer));

    if (this->select_reactor_->current_info(handle, n1) == -1 ||
	n1 != sizeof buffer)
    {
	// I'm not quite sure what to do at this point.  It's
	// probably best just to return -1.
	ACE_DEBUG((LM_DEBUG, 
		   "ACE_CLASSIX_Select_Reactor_Notify:: "
		   "read not expected by the reactor\n", n1));
	return -1;
    }

    while ((n = ::ipcGetData(rmsg.get())) > 0)
    {
	if (n != sizeof buffer)
        {
	    // I'm not sure quite what to do at this point.  It's
	    // probably best just to return -1.
	    ACE_DEBUG((LM_DEBUG, 
		       "ACE_CLASSIX_Select_Reactor_Notify::ipcGetDAta() "
		       "incorrect read(%d)\n", n));
            return -1;
        }

	// If eh == 0 then another thread is unblocking the ACE_Select_Reactor
	// to update the ACE_Select_Reactor's internal structures.  Otherwise,
	// we need to dispatch the appropriate handle_* method on the
	// ACE_Event_Handler pointer we've been passed.
	if (buffer.eh_ != 0)
        {
	    int result = 0;

	    switch (buffer.mask_)
            {
            case ACE_Event_Handler::READ_MASK:
            case ACE_Event_Handler::ACCEPT_MASK:
		result = buffer.eh_->handle_input (ACE_INVALID_HANDLE);
		break;
            case ACE_Event_Handler::WRITE_MASK:
		result = buffer.eh_->handle_output (ACE_INVALID_HANDLE);
		break;
            case ACE_Event_Handler::EXCEPT_MASK:
		result = buffer.eh_->handle_exception (ACE_INVALID_HANDLE);
		break;
            default:
		// Should we bail out if we get an invalid mask?
		ACE_ERROR ((LM_ERROR, ASYS_TEXT ("invalid mask = %d\n"), buffer.mask_));
            }
	    if (result == -1)
		buffer.eh_->handle_close (ACE_INVALID_HANDLE,
					  ACE_Event_Handler::EXCEPT_MASK);
        }

	number_dispatched++;

	// Bail out if we've reached the <notify_threshold_>.  Note that
	// by default <notify_threshold_> is -1, so we'll loop until all
	// the notifications in the pipe have been dispatched.
	if (number_dispatched == this->select_reactor_->max_notify_iterations())
	    break;
    }

    // Reassign number_dispatched to -1 if things have gone seriously
    // wrong.
    if (n < 0)
	number_dispatched = -1;


  // Enqueue ourselves into the list of waiting threads.  When we
  // reacquire the token we'll be off and running again with ownership
  // of the token.  The postcondition of this call is that
  // this->select_reactor_.token_.current_owner () == ACE_Thread::self ();
    this->select_reactor_->renew();
    return number_dispatched;
}
/* ------------------------------------------------------------------------- */
