/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Reactor.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_REACTOR_H)
#define ACE_REACTOR_H

#include "ace/Handle_Set.h"
#include "ace/Timer_Queue.h"
#include "ace/Signal.h"
#include "ace/Thread.h"
#include "ace/Token.h"
#include "ace/Pipe.h"
#include "ace/SOCK_Stream.h"

// Add useful typedefs to simplify the following code.
typedef void (ACE_Handle_Set::*ACE_FDS_PTMF) (ACE_HANDLE);
typedef int (ACE_Event_Handler::*ACE_EH_PTMF) (ACE_HANDLE);

// Forward declaration.
class ACE_Reactor;

class ACE_Export ACE_Reactor_Handle_Set
  // = TITLE
  //      Track handles we are interested for various events.
{
public:
  ACE_Handle_Set rd_mask_;
  // Read events (e.g., input pending, accept pending).

  ACE_Handle_Set wr_mask_;
  // Write events (e.g., flow control abated, non-blocking connection
  // complete).

  ACE_Handle_Set ex_mask_;
  // Exception events (e.g., SIG_URG).
};

#if defined (ACE_MT_SAFE)

// The following two classes have to be moved out here to keep the SGI
// C++ compiler happy (it doesn't like nested classes).

class ACE_Export ACE_Reactor_Notify : public ACE_Event_Handler
  // = TITLE
  //     Unblock the <ACE_Reactor> from its event loop.
  //
  // = DESCRIPTION
  //     This implementation is necessary for cases where the
  //     <ACE_Reactor> is run in a multi-threaded program.  In this
  //     case, we need to be able to unblock select() or poll() when
  //     updates occur other than in the main <ACE_Reactor> thread.
  //     To do this, we signal an auto-reset event the <ACE_Reactor>
  //     is listening on.  If an <ACE_Event_Handler> and
  //     <ACE_Reactor_Mask> is passed to <notify>, the appropriate
  //     <handle_*> method is dispatched in the context of the
  //     <ACE_Reactor> thread.
{
public:
  // = Initialization and termination methods.
  int open (ACE_Reactor *);
  int close (void);

  int dispatch_notifications (int &number_of_active_handles,
			      const ACE_Handle_Set &rd_mask);
  // Handles pending threads (if any) that are waiting to unblock the
  // Reactor.

  ssize_t notify (ACE_Event_Handler * = 0, 
		  ACE_Reactor_Mask = ACE_Event_Handler::EXCEPT_MASK,
		  ACE_Time_Value * = 0);
  // Called by a thread when it wants to unblock the Reactor.  This
  // wakeups the <ACE_Reactor> if currently blocked in
  // select()/poll().  Pass over both the <Event_Handler> *and* the
  // <mask> to allow the caller to dictate which <Event_Handler>
  // method the <Reactor> will invoke.  The <ACE_Time_Value> indicates
  // how long to blocking trying to notify the <Reactor>.  If
  // <timeout> == 0, the caller will block until action is possible,
  // else will wait until the relative time specified in *<timeout>
  // elapses).

  virtual int handle_input (ACE_HANDLE handle);
  // Called back by the Reactor when a thread wants to unblock us.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Reactor *reactor_;
  // Keep a back pointer to the Reactor.

  ACE_Pipe notification_pipe_;
  // Contains the HANDLE the Reactor is listening on, as well as the
  // HANDLE that threads wanting the attention of the Reactor will
  // write to.
};

#if defined (ACE_REACTOR_HAS_DEADLOCK_DETECTION)
#include "ace/Local_Tokens.h"
typedef ACE_Local_Mutex ACE_REACTOR_MUTEX;
#else 
typedef ACE_Token ACE_REACTOR_MUTEX;
#endif /* ACE_REACTOR_HAS_DEADLOCK_DETECTION */
 
class ACE_Export ACE_Reactor_Token : public ACE_REACTOR_MUTEX
  // = TITLE
  //     Used as a synchronization mechanism to coordinate concurrent
  //     access to a Reactor object.
{
public:
  ACE_Reactor_Token (ACE_Reactor &r);

  virtual void sleep_hook (void);
  // Called just before the ACE_Event_Handler goes to sleep.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Reactor &reactor_;
};

#endif /* ACE_MT_SAFE */    

class ACE_Export ACE_Reactor_Handler_Repository
  // = TITLE
  //     Used to map <ACE_HANDLE>s onto the appropriate
  //     <ACE_Event_Handler> *. 
  // 
  // = DESCRIPTION
  //     This class is necessary to shield differences between UNIX
  //     and Win32.  In UNIX, <ACE_HANDLE> is an int, whereas in Win32
  //     it's a void *.  This class hides all these details from the
  //     bulk of the <ACE_Reactor> code.  All of these methods are
  //     called with the main <Reactor> token lock held.
{
public:
  friend class ACE_Reactor_Handler_Repository_Iterator;

  // = Initialization and termination methods.
  ACE_Reactor_Handler_Repository (ACE_Reactor &);
  // Default "do-nothing" constructor.
  
  int open (size_t size);
  // Initialize a repository of the appropriate <size>.

  int close (void);
  // Close down the repository.

  // = Search structure operations.

  ACE_Event_Handler *find (ACE_HANDLE handle, size_t *index_p = 0);
  // Return the <ACE_Event_Handler *> associated with <ACE_HANDLE>.
  // If <index_p> is non-0, then return the index location of the
  // <handle>, if found.

  int bind (ACE_HANDLE, 
	    ACE_Event_Handler *, 
	    ACE_Reactor_Mask);
  // Bind the <ACE_Event_Handler *> to the <ACE_HANDLE> with the
  // appropriate <ACE_Reactor_Mask> settings.

  int unbind (ACE_HANDLE, 
	      ACE_Reactor_Mask mask);
  // Remove the binding of <ACE_HANDLE> in accordance with the <mask>.

  int unbind_all (void);
  // Remove all the <ACE_HANDLE, ACE_Event_Handler> tuples.

  // = Sanity checking.

  // Check the <handle> to make sure it's a valid ACE_HANDLE that
  // within the range of legal handles (i.e., >= 0 && < max_size_).
  int invalid_handle (ACE_HANDLE handle);

  // Check the <handle> to make sure it's a valid ACE_HANDLE that
  // within the range of currently registered handles (i.e., >= 0 && <
  // max_handlep1_).
  int handle_in_range (ACE_HANDLE handle);

  // = Accessors.
  size_t max_handlep1 (void);
  // Maximum ACE_HANDLE value, plus 1.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Reactor &reactor_;
  // Reference to our <Reactor>.

  ssize_t max_size_;
  // Maximum number of handles.

  int max_handlep1_;
  // The highest currently active handle, plus 1 (ranges between 0 and
  // <max_size_>.

#if defined (ACE_WIN32)
  // = This structure maps <HANDLES> to <Event_Handlers>.
  struct ACE_NT_EH_Record
    {
      ACE_HANDLE handle_;
      ACE_Event_Handler *event_handler_;
    };

  ACE_NT_EH_Record *event_handlers_;
  // The NT version implements this via a dynamically allocated
  // array of <ACE_NT_EH_Record *>.  Since NT implements ACE_HANDLE
  // as a void * we can't directly index into this array.  Therefore,
  // we just do a linear search (for now).  Next, we'll modify
  // things to use hashing or something faster...
#else
  ACE_Event_Handler **event_handlers_;
  // The UNIX version implements this via a dynamically allocated
  // array of <ACE_Event_Handler *> that is indexed directly using
  // the ACE_HANDLE value.
#endif /* ACE_WIN32 */
};

class ACE_Export ACE_Reactor_Handler_Repository_Iterator
  // = TITLE
  //     Iterate through the <ACE_Reactor_Handler_Repository>.
{
public:
  // = Initialization method.
  ACE_Reactor_Handler_Repository_Iterator (const ACE_Reactor_Handler_Repository *s);

  // = Iteration methods.

  int next (ACE_Event_Handler *&next_item);
  // Pass back the <next_item> that hasn't been seen in the Set.
  // Returns 0 when all items have been seen, else 1.

  int advance (void);
  // Move forward by one element in the set.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  const ACE_Reactor_Handler_Repository *rep_;
  // Reference to the Handler_Repository we are iterating over.

  ssize_t current_;
  // Pointer to the current iteration level.
};

class ACE_Export ACE_Dispatch_Strategy
{
public:

private:
};

class ACE_Export ACE_Reactor
  // = TITLE
  //     An object oriented event demultiplexor and event handler
  //     dispatcher. 
  //     
  // = DESCRIPTION
  //     The ACE_Reactor is an object-oriented event demultiplexor
  //     and event handler dispatcher.  The sources of events that the
  //     ACE_Reactor waits for and dispatches includes I/O events,
  //     signals, and timer events.  All public methods acquire the
  //     main <Reactor> token lock and call down to private or
  //     protected methods, which assume that the lock is held and so
  //     therefore don't (re)acquire the lock.
{
public:
  enum 
  {
    DEFAULT_SIZE = ACE_DEFAULT_REACTOR_SIZE,     
    // Default size of the Reactor's handle table.

    // = Operations on the "ready" mask and the "dispatch" mask.
    GET_MASK = 1, 
    // Retrieve current value of the the "ready" mask or the "dispatch" mask. 
    SET_MASK = 2, 
    // Set value of bits to new mask (changes the entire mask).
    ADD_MASK = 3, 
    // Bitwise "or" the value into the mask (only changes enabled bits).
    CLR_MASK = 4 
    // Bitwise "and" the negation of the value out of the mask (only changes enabled bits).
  };

  // = Initialization and termination methods.

  ACE_Reactor (ACE_Sig_Handler * = 0,
	       ACE_Timer_Queue * = 0);
  // Initialize <ACE_Reactor> with the default size.

  ACE_Reactor (size_t size, 
	       int restart = 0, 
	       ACE_Sig_Handler * = 0,
	       ACE_Timer_Queue * = 0);
  // Initialize <ACE_Reactor> with size <size>.

  virtual int open (size_t size = DEFAULT_SIZE, 
		    int restart = 0, 
		    ACE_Sig_Handler * = 0,
		    ACE_Timer_Queue * = 0);
  // Initialize <ACE_Reactor> with size <size>.

  virtual void close (void);
  // Close down the reactor and release all of its resources.

  virtual ~ACE_Reactor (void);
  // Close down the reactor and release all of its resources.
  
  // = Event loop drivers. 
  virtual int handle_events (ACE_Time_Value *max_wait_time = 0);
  // This event loop driver that blocks for <max_wait_time> before
  // returning.  It will return earlier if timer events, I/O events,
  // or signal events occur.  Note that <max_wait_time> can be 0, in
  // which case this method blocks indefinitely until events occur.
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
  // any handlers, or -1 if something goes wrong.

  virtual int handle_events (ACE_Time_Value &max_wait_time);
  // This method is just like the one above, except the
  // <max_wait_time> value is a reference and can therefore never be
  // NULL.

  // = Register and remove <ACE_Event_Handler>s. 
  virtual int register_handler (ACE_Event_Handler *eh,
				ACE_Reactor_Mask mask);
  // Register a <eh> with a particular <mask>.  Note that the
  // <Reactor[> will call eh->get_handle() to extract the underlying
  // I/O handle.
 
  virtual int register_handler (ACE_HANDLE handle, 
				ACE_Event_Handler *eh,
				ACE_Reactor_Mask mask); 
  // Register a <eh> with a particular <mask>.  Note that since the
  // <handle> is given the Reactor will *not* call eh->get_handle() to
  // extract the underlying I/O handle.

  virtual int register_handler (const ACE_Handle_Set &handles,
				ACE_Event_Handler *eh,
				ACE_Reactor_Mask mask); 
  // Register <eh> with all the <handles> in the <Handle_Set>.

  virtual int register_handler (int signum, 
				ACE_Event_Handler *new_sh,
				ACE_Sig_Action *new_disp = 0,
				ACE_Event_Handler **old_sh = 0,
				ACE_Sig_Action *old_disp = 0);
  // Register <new_sh> to handle the signal <signum> using the
  // <new_disp>.  Returns the <old_sh> that was previously registered
  // (if any), along with the <old_disp> of the signal handler.

  virtual int register_handler (const ACE_Sig_Set &sigset, 
				ACE_Event_Handler *new_sh,
				ACE_Sig_Action *new_disp = 0);
  // Registers <new_sh> to handle a set of signals <sigset> using the
  // <new_disp>.

  virtual int remove_handler (ACE_Event_Handler *eh,
			      ACE_Reactor_Mask mask);
  // Removes the <mask> binding of <eh> from the Reactor.  If there
  // are no more bindings for this <eh> then it is removed from the
  // Reactor.  Note that the Reactor will call eh->get_handle() to
  // extract the underlying I/O handle.

  virtual int remove_handler (ACE_HANDLE handle, 
			      ACE_Reactor_Mask);
  // Removes the <mask> bind of <Event_Handler> whose handle is
  // <handle> from the Reactor.  If there are no more bindings for
  // this <eh> then it is removed from the Reactor.

  virtual int remove_handler (const ACE_Handle_Set &handle_set,
			      ACE_Reactor_Mask);
  // Removes all the <mask> bindings for handles in the <handle_set>
  // bind of <Event_Handler>.  If there are no more bindings for any
  // of these handlers then they are removed from the Reactor.

  virtual int remove_handler (int signum, 
			      ACE_Sig_Action *new_disp, 
			      ACE_Sig_Action *old_disp = 0,
			      int sigkey = -1);
  // Remove the ACE_Event_Handler currently associated with <signum>.
  // <sigkey> is ignored in this implementation since there is only
  // one instance of a signal handler.  Install the new disposition
  // (if given) and return the previous disposition (if desired by the
  // caller).  Returns 0 on success and -1 if <signum> is invalid.

  virtual int remove_handler (const ACE_Sig_Set &sigset);
  // Calls <remove_handler> for every signal in <sigset>.

  // = Suspend and resume Handlers. 

  virtual int suspend_handler (ACE_Event_Handler *eh);
  // Temporarily suspend the <Event_Handler> associated with <eh>.

  virtual int suspend_handler (ACE_HANDLE handle);
  // Temporarily suspend the <Event_Handler> associated with <handle>.

  virtual int resume_handler (ACE_Event_Handler *eh);
  // Resume a temporarily suspend <Event_Handler> associated with
  // <eh>. 

  virtual int resume_handler (ACE_HANDLE handle);
  // Resume a temporarily suspended <Event_Handler> associated with
  // <handle>.

  virtual int suspend_handlers (void);
  // Suspend all the <Event_Handlers> in the Reactor.

  virtual int resume_handlers (void);
  // Resume all the <Event_Handlers> in the Reactor.

  // = Timer management. 
  virtual int schedule_timer (ACE_Event_Handler *, 
			      const void *arg,
			      const ACE_Time_Value &delta,
			      const ACE_Time_Value &interval = ACE_Time_Value::zero);  
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

  virtual int cancel_timer (ACE_Event_Handler *event_handler);
  // Cancel all <event_handlers> that match the address of
  // <event_handler>.  Returns number of handler's cancelled.

  virtual int cancel_timer (int timer_id, const void **arg = 0);
  // Cancel the single <ACE_Event_Handler> that matches the <timer_id>
  // value (which was returned from the <schedule> method).  If arg is
  // non-NULL then it will be set to point to the ``magic cookie''
  // argument passed in when the <Event_Handler> was registered.  This
  // makes it possible to free up the memory and avoid memory leaks.
  // Returns 1 if cancellation succeeded and 0 if the <timer_id>
  // wasn't found.

  // = High-level Event_Handler scheduling operations
  
  virtual int schedule_wakeup (ACE_Event_Handler *eh,
			       ACE_Reactor_Mask mask);
  // ADD the dispatch MASK "bit" bound with the <eh> and the <mask>.

  virtual int schedule_wakeup (ACE_HANDLE handle,
			       ACE_Reactor_Mask mask);
  // ADD the dispatch MASK "bit" bound with the <handle> and the <mask>.

  virtual int cancel_wakeup (ACE_Event_Handler *eh,
			     ACE_Reactor_Mask mask);
  // CLR the dispatch MASK "bit" bound with the <eh> and the <mask>.

  virtual int cancel_wakeup (ACE_HANDLE handle,
			     ACE_Reactor_Mask mask);
  // CLR the dispatch MASK "bit" bound with the <handle> and the <mask>.

  // = Notification methods.
  virtual int notify (ACE_Event_Handler * = 0, 
		      ACE_Reactor_Mask = ACE_Event_Handler::EXCEPT_MASK,
		      ACE_Time_Value * = 0);
  // Called by a thread when it wants to unblock the Reactor.  This
  // wakeups the <ACE_Reactor> if currently blocked in
  // select()/poll().  Pass over both the <Event_Handler> *and* the
  // <mask> to allow the caller to dictate which <Event_Handler>
  // method the <Reactor> will invoke.  The <ACE_Time_Value> indicates
  // how long to blocking trying to notify the <Reactor>.  If
  // <timeout> == 0, the caller will block until action is possible,
  // else will wait until the relative time specified in *<timeout>
  // elapses).

  // 
  void requeue_position (int);
  // Set position that the main ACE_Reactor thread is requeued in the
  // list of waiters during a notify() callback.

  int requeue_position (void);
  // Get position that the main ACE_Reactor thread is requeued in the
  // list of waiters during a notify() callback.

  void max_notify_iterations (int);
  // Set the maximum number of times that the
  // <ACE_Reactor_Notify::handle_input> method will iterate and
  // dispatch the <ACE_Event_Handlers> that are passed in via the
  // notify pipe before breaking out of its <recv> loop.  By default,
  // this is set to -1, which means "iterate until the pipe is empty."
  // Setting this to a value like "1 or 2" will increase "fairness"
  // (and thus prevent starvation) at the expense of slightly higher
  // dispatching overhead.

  int max_notify_iterations (void);
  // Get the maximum number of times that the
  // <ACE_Reactor_Notify::handle_input> method will iterate and
  // dispatch the <ACE_Event_Handlers> that are passed in via the
  // notify pipe before breaking out of its <recv> loop.

  // = Low-level wait_set mask manipulation methods.
  virtual int mask_ops (ACE_Event_Handler *eh,
			ACE_Reactor_Mask mask, 
			int ops);
  // GET/SET/ADD/CLR the dispatch mask "bit" bound with the <eh> and
  // <mask>.

  virtual int mask_ops (ACE_HANDLE handle, 
			ACE_Reactor_Mask mask, 
			int ops);  
  // GET/SET/ADD/CLR the dispatch MASK "bit" bound with the <handle>
  // and <mask>.

  // = Low-level ready_set mask manipulation methods.
  virtual int ready_ops (ACE_Event_Handler *eh, 
			 ACE_Reactor_Mask mask, 
			 int ops);
  // GET/SET/ADD/CLR the ready "bit" bound with the <eh> and <mask>.

  virtual int ready_ops (ACE_HANDLE handle, 
			 ACE_Reactor_Mask, 
			 int ops);
  // GET/SET/ADD/CLR the ready "bit" bound with the <handle> and <mask>.

  // = Only the owner thread that can perform a <handle_events>.

  void owner (ACE_thread_t n_id, ACE_thread_t *o_id = 0);
  // Set the new owner of the thread and return the old owner.

  int owner (ACE_thread_t *);
  // Return the current owner of the thread.

  // = Miscellaneous Handler operations. 
  virtual int handler (ACE_HANDLE handle,
		       ACE_Reactor_Mask mask, 
		       ACE_Event_Handler **eh = 0); 
  // Check to see if <handle> is associated with a valid Event_Handler
  // bound to <mask>.  Return the <eh> associated with this <handler>
  // if <eh> != 0.

  virtual int handler (int signum, 
		       ACE_Event_Handler ** = 0);
  // Check to see if <signum> is associated with a valid Event_Handler
  // bound to a signal.  Return the <eh> associated with this
  // <handler> if <eh> != 0.

  int initialized (void);
  // Returns true if we've been successfully initialized, else false.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  // = Internal methods that do the actual work.

  // All of these methods assume that the <Reactor>'s token lock is
  // held by the public methods that call down to them.

  virtual int register_handler_i (ACE_HANDLE handle, 
				 ACE_Event_Handler *eh, 
				 ACE_Reactor_Mask mask); 
  // Do the work of actually binding the <handle> and <eh> with the
  // <mask>.

  virtual int remove_handler_i (ACE_HANDLE handle, 
				ACE_Reactor_Mask);
  // Do the work of actually unbinding the <handle> and <eh> with the
  // <mask>.

  virtual int suspend_i (ACE_HANDLE handle);
  // Suspend the <Event_Handler> associated with <handle>
  virtual int resume_i (ACE_HANDLE handle);
  // Resume the <Event_Handler> associated with <handle>

  virtual int remove_handler_i (const ACE_Handle_Set &handles, 
				 ACE_Reactor_Mask);
  // Remove a set of <handles>.

  virtual int register_handler_i (const ACE_Handle_Set &handles, 
				  ACE_Event_Handler *handler, 
				  ACE_Reactor_Mask mask);
  // Register a set of <handles>.

  virtual int handler_i (ACE_HANDLE handle, 
			  ACE_Reactor_Mask, 
			  ACE_Event_Handler ** = 0);
  // Implement the public <handler> method.

  virtual int handler_i (int signum, ACE_Event_Handler ** = 0);
  // Implement the public <handler> method.

  virtual int any_ready (ACE_Reactor_Handle_Set &handle_set);
  // Check if there are any HANDLEs enabled in the <ready_set_>, and
  // if so, update the <handle_set> and return the number ready.  If
  // there aren't any HANDLEs enabled return 0.

  virtual int handle_error (void);
  // Take corrective action when errors occur.

  virtual int check_handles (void);
  // Make sure the handles are all valid.

  virtual int bit_ops (ACE_HANDLE handle, 
		       ACE_Reactor_Mask mask, 
		       ACE_Reactor_Handle_Set &wait_Set,
		       int ops);
  // Allow manipulation of the <wait_set_> mask and <ready_set_> mask.

  virtual int wait_for_multiple_events (ACE_Reactor_Handle_Set &,
					ACE_Time_Value *); 
  // Wait for events to occur.

  // = Dispatching methods.

  virtual int dispatch (int nfound,
			ACE_Reactor_Handle_Set &);
  // Template Method that dispatches <ACE_Event_Handler>s for time
  // events, I/O events, and signal events.  Returns the total number
  // of <ACE_Event_Handler>s that were dispatched or -1 if something
  // goes wrong.

  virtual int dispatch_timer_handlers (void);
  // Dispatch any expired timer handlers.  Returns -1 if the state of
  // the <wait_set_> has changed, else returns number of timer
  // handlers dispatched.

  virtual int dispatch_notification_handlers (int &number_of_active_handles,
					      ACE_Reactor_Handle_Set &dispatch_set);
  // Dispatch any notification handlers.  Returns -1 if the state of
  // the <wait_set_> has changed, else returns number of handlers
  // notified.

  virtual int dispatch_io_handlers (int &number_of_active_handles,
				    ACE_Reactor_Handle_Set &dispatch_set);
  // Dispatch all the input/output/except handlers that are enabled in
  // the <dispatch_set>.  Returns -1 if the state of the <wait_set_>
  // has changed, else returns number of handlers dispatched.

  virtual void notify_handle (ACE_HANDLE handle,
			      ACE_Reactor_Mask mask, 
			      ACE_Handle_Set &, 
			      ACE_Event_Handler *eh,
			      ACE_EH_PTMF callback);
  // Notify the appropriate <callback> in the context of the <eh>
  // associated with <handle> that a particular event has occurred.

  ACE_Reactor_Handler_Repository handler_rep_;
  // Table that maps <ACE_HANDLEs> to <ACE_Event_Handler *>'s.

  ACE_Timer_Queue *timer_queue_; 
  // Defined as a pointer to allow overriding by derived classes...

  int delete_timer_queue_;
  // Keeps track of whether we should delete the timer queue (if we
  // didn't create it, then we don't delete it).

  ACE_Sig_Handler *signal_handler_;
  // Handle signals without requiring global/static variables. 

  int delete_signal_handler_;
  // Keeps track of whether we should delete the signal handler (if we
  // didn't create it, then we don't delete it).

  ACE_Reactor_Handle_Set wait_set_;
  // Tracks handles that are waited for by select().

  ACE_Reactor_Handle_Set ready_set_;
  // Track HANDLES we are interested in for various events that must
  // be dispatched *without* going through select().

  int restart_; 
  // Restart automatically when interrupted 

  int requeue_position_;
  // Position that the main ACE_Reactor thread is requeued in the list
  // of waiters during a notify() callback.  If this value == -1 we
  // are requeued at the end of the list.  Else if it's 0 then we are
  // requeued at the front of the list.  Else if it's > 1 then that
  // indicates the number of waiters to skip over.

  int max_notify_iterations_;
  // Keeps track of the maximum number of times that the
  // <ACE_Reactor_Notify::handle_input> method will iterate and
  // dispatch the <ACE_Event_Handlers> that are passed in via the
  // notify pipe before breaking out of its <recv> loop.  By default,
  // this is set to -1, which means "iterate until the pipe is empty."

  int initialized_;
  // True if we've been initialized yet...

  ACE_thread_t owner_;
  // The original thread that created this Reactor.

  int state_changed_;
  // True if state has changed during dispatching of
  // <ACE_Event_Handlers>, else false.  This is used to determine
  // whether we need to make another trip through the <Reactor>'s
  // <wait_for_multiple_events> loop.

#if defined (ACE_MT_SAFE)
  ACE_Reactor_Notify notify_handler_;
  // Callback object that unblocks the ACE_Reactor if it's sleeping.

  ACE_Reactor_Token token_;
  // Synchronization token for the MT_SAFE ACE_Reactor.

  void renew (void);
  // Enqueue ourselves into the list of waiting threads at the
  // appropriate point specified by <requeue_position_>.  

  friend class ACE_Reactor_Notify;
#endif /* ACE_MT_SAFE */

  friend class ACE_Reactor_Handler_Repository;

private:
  int release_token (void);
  // Release the token lock when a Win32 structured exception occurs.

  int handle_events_i (ACE_Time_Value *max_wait_time = 0);
  // Stops the VC++ compiler from bitching about exceptions and destructors

  // Deny access since member-wise won't work...
  ACE_Reactor (const ACE_Reactor &);
  ACE_Reactor &operator = (const ACE_Reactor &);
};

#if defined (__ACE_INLINE__)
#include "ace/Reactor.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_REACTOR_H */
