/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Reactor_Impl.h
//
// = AUTHOR
//    Irfan Pyarali 
// 
// ============================================================================

#if !defined (ACE_REACTOR_IMPL_H)
#define ACE_REACTOR_IMPL_H

class ACE_Handle_Set;

// Timer Queue is a complicated template class. A simple forward
// declaration will not work
#include "ace/Timer_Queue.h"

// Event_Handler.h contains the definition of ACE_Reactor_Mask
#include "ace/Event_Handler.h"

// We are using 4 or 5 signal classes, we could forward declare
// them.... But Timer_Queue_T.h includes Signal.h, so I don't think
// forward declaration will be useful here
#include "ace/Signal.h"

class ACE_Export ACE_Reactor_Impl
  // = TITLE
  //
  //     An abstract class for implementing the Reactor Pattern.
{
public:

  virtual ~ACE_Reactor_Impl (void) {};
  // Close down and release all resources.
  
  virtual int open (size_t size, 
		    int restart = 0, 
		    ACE_Sig_Handler * = 0,
		    ACE_Timer_Queue * = 0) = 0;
  // Initialization.  

  virtual int close (void) = 0;
  // Close down and release all resources.

  // = Event loop drivers.  

  virtual int handle_events (ACE_Time_Value *max_wait_time = 0) = 0;
  virtual int alertable_handle_events (ACE_Time_Value *max_wait_time = 0) = 0;
  // This event loop driver blocks for up to <max_wait_time> before
  // returning.  It will return earlier if events occur.  Note that
  // <max_wait_time> can be 0, in which case this method blocks
  // indefinitely until events occur.
  //
  // <max_wait_time> is decremented to reflect how much time this call
  // took.  For instance, if a time value of 3 seconds is passed to
  // handle_events and an event occurs after 2 seconds,
  // <max_wait_time> will equal 1 second.  This can be used if an
  // application wishes to handle events for some fixed amount of
  // time.
  // 
  // Returns the total number of <ACE_Event_Handler>s that were
  // dispatched, 0 if the <max_wait_time> elapsed without dispatching
  // any handlers, or -1 if an error occurs.
  //
  // The only difference between <alertable_handle_events> and
  // <handle_events> is that in the alertable case, the eventloop will
  // return when the system queues an I/O completion routine or an
  // Asynchronous Procedure Call.

  virtual int handle_events (ACE_Time_Value &max_wait_time) = 0;
  virtual int alertable_handle_events (ACE_Time_Value &max_wait_time) = 0;
  // This method is just like the one above, except the
  // <max_wait_time> value is a reference and can therefore never be
  // NULL.
  //
  // The only difference between <alertable_handle_events> and
  // <handle_events> is that in the alertable case, the eventloop will
  // return when the system queues an I/O completion routine or an
  // Asynchronous Procedure Call.

  // = Register and remove Handlers. 

  virtual int register_handler (ACE_Event_Handler *event_handler,
				ACE_Reactor_Mask mask) = 0;
  // Register <event_handler> with <mask>.  The I/O handle will always
  // come from <get_handle> on the <event_handler>.
 
  virtual int register_handler (ACE_HANDLE io_handle, 
				ACE_Event_Handler *event_handler,
				ACE_Reactor_Mask mask) = 0; 
  // Register <event_handler> with <mask>.  The I/O handle is provided
  // through the <io_handle> parameter.
 
#if defined (ACE_WIN32)

  // Originally this interface was available for all platforms, but
  // because ACE_HANDLE is an int on non-Win32 platforms, compilers
  // are not able to tell the difference between
  // register_handler(ACE_Event_Handler*,ACE_Reactor_Mask) and
  // register_handler(ACE_Event_Handler*,ACE_HANDLE). Therefore, we
  // have restricted this method to Win32 only.

  virtual int register_handler (ACE_Event_Handler *event_handler, 
				ACE_HANDLE event_handle = ACE_INVALID_HANDLE) = 0;
  // Register an <event_handler> that will be notified when
  // <event_handle> is signaled.  Since no event mask is passed
  // through this interface, it is assumed that the <event_handle>
  // being passed in is an event handle and not an I/O handle.

#endif /* ACE_WIN32 */

  virtual int register_handler (ACE_HANDLE event_handle,
				ACE_HANDLE io_handle,
				ACE_Event_Handler *event_handler, 
				ACE_Reactor_Mask mask) = 0;
  // Register an <event_handler> that will be notified when
  // <event_handle> is signaled.  <mask> specifies the network events
  // that the <event_handler> is interested in.
 
  virtual int register_handler (const ACE_Handle_Set &handles,
				ACE_Event_Handler *event_handler,
				ACE_Reactor_Mask mask) = 0; 
  // Register <event_handler> with all the <handles> in the <Handle_Set>.

  virtual int register_handler (int signum, 
				ACE_Event_Handler *new_sh,
				ACE_Sig_Action *new_disp = 0,
				ACE_Event_Handler **old_sh = 0,
				ACE_Sig_Action *old_disp = 0) = 0;
  // Register <new_sh> to handle the signal <signum> using the
  // <new_disp>.  Returns the <old_sh> that was previously registered
  // (if any), along with the <old_disp> of the signal handler.

  virtual int register_handler (const ACE_Sig_Set &sigset, 
				ACE_Event_Handler *new_sh,
				ACE_Sig_Action *new_disp = 0) = 0;
  // Registers <new_sh> to handle a set of signals <sigset> using the
  // <new_disp>.

  virtual int remove_handler (ACE_Event_Handler *event_handler,
			      ACE_Reactor_Mask mask) = 0;
  // Removes <event_handler>.  Note that the I/O handle will be
  // obtained using <get_handle> method of <event_handler> .  If
  // <mask> == <ACE_Event_Handler::DONT_CALL> then the <handle_close>
  // method of the <event_handler> is not invoked.

  virtual int remove_handler (ACE_HANDLE handle, 
			      ACE_Reactor_Mask mask) = 0;
  // Removes <handle>.  If <mask> == <ACE_Event_Handler::DONT_CALL>
  // then the <handle_close> method of the associated <event_handler>
  // is not invoked.

  virtual int remove_handler (const ACE_Handle_Set &handle_set,
			      ACE_Reactor_Mask mask) = 0;
  // Removes all handles in <handle_set>.  If <mask> ==
  // <ACE_Event_Handler::DONT_CALL> then the <handle_close> method of
  // the associated <event_handler>s is not invoked.

  virtual int remove_handler (int signum, 
			      ACE_Sig_Action *new_disp, 
			      ACE_Sig_Action *old_disp = 0,
			      int sigkey = -1) = 0;
  // Remove the ACE_Event_Handler currently associated with <signum>.
  // Install the new disposition (if given) and return the previous
  // disposition (if desired by the caller).  Returns 0 on success and
  // -1 if <signum> is invalid.

  virtual int remove_handler (const ACE_Sig_Set &sigset) = 0;
  // Calls <remove_handler> for every signal in <sigset>.

  // = Suspend and resume Handlers. 

  virtual int suspend_handler (ACE_Event_Handler *event_handler) = 0;
  // Suspend <event_handler> temporarily.  Use
  // <event_handler->get_handle()> to get the handle.

  virtual int suspend_handler (ACE_HANDLE handle) = 0;
  // Suspend <handle> temporarily.

  virtual int suspend_handler (const ACE_Handle_Set &handles) = 0;
  // Suspend all <handles> in handle set temporarily.

  virtual int suspend_handlers (void) = 0;
  // Suspend all <handles> temporarily. 

  virtual int resume_handler (ACE_Event_Handler *event_handler) = 0;
  // Resume <event_handler>. Use <event_handler->get_handle()> to get
  // the handle.

  virtual int resume_handler (ACE_HANDLE handle) = 0;
  // Resume <handle>. 

  virtual int resume_handler (const ACE_Handle_Set &handles) = 0;
  // Resume all <handles> in handle set. 

  virtual int resume_handlers (void) = 0;
  // Resume all <handles>.

  virtual int reset_new_handle (void) = 0;
  // If we need to reset handles returned from accept/connect.

  // Timer management.

  virtual long schedule_timer (ACE_Event_Handler *event_handler,
			       const void *arg,
			       const ACE_Time_Value &delta,
			       const ACE_Time_Value &interval = ACE_Time_Value::zero) = 0;  
  // Schedule an <event_handler> that will expire after <delay> amount
  // of time.  If it expires then <arg> is passed in as the value to
  // the <event_handler>'s <handle_timeout> callback method.  If
  // <interval> is != to <ACE_Time_Value::zero> then it is used to
  // reschedule the <event_handler> automatically.  This method
  // returns a <timer_id> that uniquely identifies the <event_handler>
  // in an internal list.  This <timer_id> can be used to cancel an
  // <event_handler> before it expires.  The cancellation ensures that
  // <timer_ids> are unique up to values of greater than 2 billion
  // timers.  As long as timers don't stay around longer than this
  // there should be no problems with accidentally deleting the wrong
  // timer.  Returns -1 on failure (which is guaranteed never to be a
  // valid <timer_id>.

  virtual int cancel_timer (ACE_Event_Handler *event_handler,
			    int dont_call_handle_close = 1) = 0;
  // Cancel all Event_Handlers that match the address of
  // <event_handler>.  Returns number of handlers cancelled.

  virtual int cancel_timer (long timer_id, 
			    const void **arg = 0,
			    int dont_call_handle_close = 1) = 0;
  // Cancel the single Event_Handler that matches the <timer_id> value
  // (which was returned from the schedule method).  If arg is
  // non-NULL then it will be set to point to the ``magic cookie''
  // argument passed in when the Event_Handler was registered.  This
  // makes it possible to free up the memory and avoid memory leaks.
  // Returns 1 if cancellation succeeded and 0 if the <timer_id>
  // wasn't found.

  // = High-level Event_Handler scheduling operations
  
  virtual int schedule_wakeup (ACE_Event_Handler *event_handler,
			       ACE_Reactor_Mask masks_to_be_added) = 0;
  // Add <masks_to_be_added> to the <event_handler>'s entry.
  // <event_handler> must already have been registered.
  
  virtual int schedule_wakeup (ACE_HANDLE handle,
			       ACE_Reactor_Mask masks_to_be_added) = 0;
  // Add <masks_to_be_added> to the <handle>'s entry.  <event_handler>
  // associated with <handle> must already have been registered.

  virtual int cancel_wakeup (ACE_Event_Handler *event_handler,
			     ACE_Reactor_Mask masks_to_be_cleared) = 0;
  // Clear <masks_to_be_cleared> from the <event_handler>'s entry.

  virtual int cancel_wakeup (ACE_HANDLE handle,
			     ACE_Reactor_Mask masks_to_be_cleared) = 0;
  // Clear <masks_to_be_cleared> from the <handle>'s entry.

  // = Notification methods.

  virtual int notify (ACE_Event_Handler *event_handler = 0, 
		      ACE_Reactor_Mask mask = ACE_Event_Handler::EXCEPT_MASK,
		      ACE_Time_Value * = 0) = 0;
  // Notify <event_handler> of <mask> event.  The <ACE_Time_Value>
  // indicates how long to blocking trying to notify.  If <timeout> ==
  // 0, the caller will block until action is possible, else will wait
  // until the relative time specified in <timeout> elapses).

  virtual void max_notify_iterations (int) = 0;
  // Set the maximum number of times that ACE_Reactor_Impl will
  // iterate and dispatch the <ACE_Event_Handlers> that are passed in
  // via the notify queue before breaking out of its
  // <ACE_Message_Queue::dequeue> loop.  By default, this is set to
  // -1, which means "iterate until the queue is empty."  Setting this
  // to a value like "1 or 2" will increase "fairness" (and thus
  // prevent starvation) at the expense of slightly higher dispatching
  // overhead.

  virtual int max_notify_iterations (void) = 0;
  // Get the maximum number of times that the ACE_Reactor_Impl will
  // iterate and dispatch the <ACE_Event_Handlers> that are passed in
  // via the notify queue before breaking out of its
  // <ACE_Message_Queue::dequeue> loop.

  virtual int handler (ACE_HANDLE handle,
		       ACE_Reactor_Mask mask, 
		       ACE_Event_Handler **event_handler = 0) = 0;  
  // Check to see if <handle> is associated with a valid Event_Handler
  // bound to <mask>.  Return the <event_handler> associated with this
  // <handler> if <event_handler> != 0.

  virtual int handler (int signum, 
		       ACE_Event_Handler ** = 0) = 0;
  // Check to see if <signum> is associated with a valid Event_Handler
  // bound to a signal.  Return the <event_handler> associated with
  // this <handler> if <event_handler> != 0.

  virtual int initialized (void) = 0;
  // Returns true if Reactor has been successfully initialized, else
  // false.

  virtual size_t size (void) = 0;
  // Returns the current size of the Reactor's internal descriptor
  // table.

  virtual ACE_Lock &lock (void) = 0;
  // Returns a reference to the Reactor's internal lock.

  virtual void wakeup_all_threads (void) = 0;
  // Wake up all threads in waiting in the event loop

  virtual int owner (ACE_thread_t new_owner, ACE_thread_t *old_owner = 0) = 0;
  // Transfers ownership of Reactor_Impl to the <new_owner>.

  virtual int owner (ACE_thread_t *owner) = 0;
  // Return the ID of the "owner" thread.

  virtual void requeue_position (int) = 0;
  // Set position of the owner thread.

  virtual int requeue_position (void) = 0;
  // Get position of the owner thread.

  // = Low-level wait_set mask manipulation methods.

  virtual int mask_ops (ACE_Event_Handler *event_handler,
			ACE_Reactor_Mask mask, 
			int ops) = 0;
  // GET/SET/ADD/CLR the dispatch mask "bit" bound with the
  // <event_handler> and <mask>.

  virtual int mask_ops (ACE_HANDLE handle, 
			ACE_Reactor_Mask mask, 
			int ops) = 0;  
  // GET/SET/ADD/CLR the dispatch MASK "bit" bound with the <handle>
  // and <mask>.

  // = Low-level ready_set mask manipulation methods.
  virtual int ready_ops (ACE_Event_Handler *event_handler, 
			 ACE_Reactor_Mask mask, 
			 int ops) = 0;
  // GET/SET/ADD/CLR the ready "bit" bound with the <event_handler>
  // and <mask>.

  virtual int ready_ops (ACE_HANDLE handle, 
			 ACE_Reactor_Mask, 
			 int ops) = 0;
  // GET/SET/ADD/CLR the ready "bit" bound with the <handle> and <mask>.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#endif /* ACE_REACTOR_IMPL_H */
