/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    WFMO_Reactor.h
//
// = AUTHOR
//    Irfan Pyarali, Tim Harrison, and Doug Schmidt
//
// ============================================================================

#ifndef ACE_WFMO_REACTOR_H
#define ACE_WFMO_REACTOR_H

#include "ace/Signal.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Timer_Queue.h"
#include "ace/Event_Handler.h"
#include "ace/Synch.h"
#include "ace/Reactor_Impl.h"
#include "ace/Message_Queue.h"

// Forward decl.
class ACE_WFMO_Reactor;
class ACE_Handle_Set;

class ACE_Export ACE_Wakeup_All_Threads_Handler : public ACE_Event_Handler
{
  // = TITLE
  //
  //     This is a helper class whose sole purpose is to handle events
  //     on <ACE_WFMO_Reactor->wakeup_all_threads_>
  //
public:
  virtual int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  // Called when the <ACE_WFMO_Reactor->wakeup_all_threads_>
};

class ACE_Export ACE_WFMO_Reactor_Handler_Repository
{
  // = TITLE
  //
  //     Used to map <ACE_HANDLE>s onto the appropriate
  //     <ACE_Event_Handler> * and other information.
  //
public:
  friend class ACE_WFMO_Reactor;

  class Common_Info
  {
    // = TITLE
    //
    //     This struct contains the necessary information for every
    //     <Event_Handler> entry. The reason the event is not in this
    //     structure is because we need to pass an event array into
    //     WaitForMultipleObjects and therefore keeping the events
    //     seperate makes sense.
    //
  public:
    int io_entry_;
    // This indicates whether this entry is for I/O or for a regular
    // event

    ACE_Event_Handler *event_handler_;
    // The assosiated <Event_Handler>

    ACE_HANDLE io_handle_;
    // The I/O handle related to the <Event_Handler>.  This entry is
    // only valid if the <io_entry_> flag is true.

    long network_events_;
    // This is the set of events that the <Event_Handler> is
    // interested in This entry is only valid if the <io_entry_> flag
    // is true.

    int delete_event_;
    // This flag indicates that <WFMO_Reactor> created the event on
    // behalf of the user. Therefore we need to clean this up when the
    // <Event_Handler> removes itself from <WFMO_Reactor>.  This entry
    // is only valid if the <io_entry_> flag is true.

    int delete_entry_;
    // This is set when the entry needed to be deleted.

    ACE_Reactor_Mask close_masks_;
    // These are the masks related to <handle_close> for the
    // <Event_Handler>.  This is only valid when >delete_entry_> is
    // set.

    Common_Info (void);
    // Constructor used for initializing the structure

    void reset (void);
    // Reset the state of the structure

    void set (int io_entry,
              ACE_Event_Handler *event_handler,
              ACE_HANDLE io_handle,
              long network_events,
              int delete_event,
              int delete_entry,
              ACE_Reactor_Mask close_masks);
    // Set the structure to these new values

    void set (Common_Info &common_info);
    // Set the structure to these new values

    void dump (void) const;
    // Dump the state of an object.
  };

  class Current_Info : public Common_Info
  {
    // = TITLE
    //
    //     This structure inherits from the common structure to add
    //     information for current entries.
    //
  public:
    int suspend_entry_;
    // This is set when the entry needed to be suspended.

    Current_Info (void);
    // Default constructor

    void reset (void);
    // Reset the state of the structure

    void set (int io_entry,
              ACE_Event_Handler *event_handler,
              ACE_HANDLE io_handle,
              long network_events,
              int delete_event,
              int delete_entry = 0,
              ACE_Reactor_Mask close_masks = ACE_Event_Handler::NULL_MASK,
              int suspend_entry = 0);
    // Set the structure to these new values

    void set (Common_Info &common_info,
              int suspend_entry = 0);
    // Set the structure to these new values

    void dump (ACE_HANDLE event_handle) const;
    // Dump the state of an object.
  };

  class To_Be_Added_Info : public Common_Info
  {
    // = TITLE
    //
    //     This structure inherits from the common structure to add
    //     information for <to_be_added> entries.
    //
  public:
    ACE_HANDLE event_handle_;
    // Handle for the event

    int suspend_entry_;
    // This is set when the entry needed to be suspended.

    To_Be_Added_Info (void);
    // Default constructor

    void reset (void);
    // Reset the state of the structure

    void set (ACE_HANDLE event_handle,
              int io_entry,
              ACE_Event_Handler *event_handler,
              ACE_HANDLE io_handle,
              long network_events,
              int delete_event,
              int delete_entry = 0,
              ACE_Reactor_Mask close_masks = ACE_Event_Handler::NULL_MASK,
              int suspend_entry = 0);
    // Set the structure to these new values

    void set (ACE_HANDLE event_handle,
              Common_Info &common_info,
              int suspend_entry = 0);
    // Set the structure to these new values

    void dump (void) const;
    // Dump the state of an object.
  };

  class Suspended_Info : public Common_Info
  {
    // = TITLE
    //
    //     This structure inherits from the common structure to add
    //     information for suspended entries.
    //
  public:
    ACE_HANDLE event_handle_;
    // Handle for the event

    int resume_entry_;
    // This is set when the entry needed to be resumed.

    Suspended_Info (void);
    // Constructor used for initializing the structure

    void reset (void);
    // Reset the state of the structure

    void set (ACE_HANDLE event_handle,
              int io_entry,
              ACE_Event_Handler *event_handler,
              ACE_HANDLE io_handle,
              long network_events,
              int delete_event,
              int delete_entry = 0,
              ACE_Reactor_Mask close_masks = 0,
              int resume_entry = 0);
    // Set the structure to these new values

    void set (ACE_HANDLE event_handle,
              Common_Info &common_info,
              int resume_entry = 0);
    // Set the structure to these new values

    void dump (void) const;
    // Dump the state of an object.
  };

  ACE_WFMO_Reactor_Handler_Repository (ACE_WFMO_Reactor &wfmo_reactor);
  // Constructor.

  virtual ~ACE_WFMO_Reactor_Handler_Repository (void);
  // Destructor.

  int open (size_t size);
  // Initialize the repository of the approriate <size>.

  int close (void);
  // Close down the handler repository.

  // = Search structure operations.

  int bind (ACE_HANDLE, ACE_Event_Handler *);
  // Bind the <ACE_Event_Handler *> to the <ACE_HANDLE>. This is for
  // the simple event entry.

  int bind_i (int io_entry,
              ACE_Event_Handler *event_handler,
              long network_events,
              ACE_HANDLE io_handle,
              ACE_HANDLE event_handle,
              int delete_event);
  // Insert I/O <Event_Handler> entry into the system. This method
  // assumes that the lock are head *before* this method is invoked.

  int unbind (ACE_HANDLE,
              ACE_Reactor_Mask mask);
  // Remove the binding of <ACE_HANDLE> in accordance with the <mask>.

  int unbind_i (ACE_HANDLE,
                ACE_Reactor_Mask mask,
                int &changes_required);
  // Non-lock-grabbing version of <unbind>

  void unbind_all (void);
  // Remove all bindings of <ACE_HANDLE, ACE_Event_Handler> tuples.

  // = Sanity checking.

  // Check the <handle> to make sure it's a valid ACE_HANDLE
  int invalid_handle (ACE_HANDLE handle) const;

  // = Accessors.
  size_t max_handlep1 (void) const;
  // Maximum ACE_HANDLE value, plus 1.

  ACE_HANDLE *handles (void) const;
  // Pointer to the beginning of the current array of <ACE_HANDLE>
  // *'s.

  Current_Info *current_info (void) const;
  // Pointer to the beginning of the current array of
  // <ACE_Event_Handler> *'s.

  virtual int changes_required (void);
  // Check if changes to the handle set are required.

  virtual int make_changes (void);
  // Make changes to the handle set

  int scheduled_for_deletion (size_t index) const;
  // Check to see if <index> has been scheduled for deletion

  int modify_network_events_i (ACE_HANDLE io_handle,
                               ACE_Reactor_Mask new_masks,
                               ACE_Reactor_Mask &old_masks,
                               long &new_network_events,
                               ACE_HANDLE &event_handle,
                               int &delete_event,
                               int operation);
  // This method is used to calculate the network mask after a mask_op
  // request to <WFMO_Reactor>. Note that because the <Event_Handler>
  // may already be in the handler repository, we may have to find the
  // old event and the old network events

  ACE_Reactor_Mask bit_ops (long &existing_masks,
                            ACE_Reactor_Mask to_be_removed_masks,
                            int operation);
  // This method is used to change the network mask left (if any)
  // after a remove request to <WFMO_Reactor>

  int suspend_handler_i (ACE_HANDLE handle,
                         int &changes_required);
  // Temporarily suspend entry

  int resume_handler_i (ACE_HANDLE handle,
                        int &changes_required);
  // Resume suspended entry

  int make_changes_in_current_infos (void);
  // Deletions and suspensions in current_info_

  int make_changes_in_suspension_infos (void);
  // Deletions and resumptions in current_suspended_info_

  int make_changes_in_to_be_added_infos (void);
  // Deletions in to_be_added_info_, or transfers to current_info_ or
  // current_suspended_info_ from to_be_added_info_

  int remove_handler_i (size_t index,
                        ACE_Reactor_Mask mask);
  // Removes the <ACE_Event_Handler> at <index> from the table.

  int remove_suspended_handler_i (size_t index,
                                  ACE_Reactor_Mask mask);
  // Removes the <ACE_Event_Handler> at <index> from the table.

  int remove_to_be_added_handler_i (size_t index,
                                    ACE_Reactor_Mask to_be_removed_masks);
  // Removes the <ACE_Event_Handler> at <index> from the table.

  void dump (void) const;
  // Dump the state of an object.

protected:
  ACE_WFMO_Reactor &wfmo_reactor_;
  // Reference to our <WFMO_Reactor>.

  size_t max_size_;
  // Maximum number of handles.

  ACE_HANDLE *current_handles_;
  // Array of <ACE_HANDLEs> passed to <WaitForMultipleObjects>.  This
  // is not part of the structure as the handle array needs to be
  // passed directly to <WaitForMultipleObjects>.

  Current_Info *current_info_;
  // Array of current entries in the table

  size_t max_handlep1_;
  // A count of the number of active handles.

  To_Be_Added_Info *to_be_added_info_;
  // Information for entries to be added

  size_t handles_to_be_added_;
  // Number of records to be added

  Suspended_Info *current_suspended_info_;
  // Currently suspended handles

  size_t suspended_handles_;
  // Number of currently suspended handles

  size_t handles_to_be_suspended_;
  // Number of records to be suspended

  size_t handles_to_be_resumed_;
  // Number of records to be resumed

  size_t handles_to_be_deleted_;
  // Number of records to be deleted

};

class ACE_Export ACE_WFMO_Reactor_Notify : public ACE_Reactor_Notify
{
  // = TITLE
  //     Unblock the <ACE_WFMO_Reactor> from its event loop, passing
  //     it an optional <ACE_Event_Handler> to dispatch.
  //
  // = DESCRIPTION
  //     This implementation is necessary for cases where the
  //     <ACE_WFMO_Reactor> is run in a multi-threaded program.  In
  //     this case, we need to be able to unblock
  //     WaitForMultipleObjects() when updates occur other than in the
  //     main <ACE_WFMO_Reactor> thread.  To do this, we signal an
  //     auto-reset event the <ACE_WFMO_Reactor> is listening on.  If
  //     an <ACE_Event_Handler> and <ACE_Reactor_Mask> is passed to
  //     <notify>, the appropriate <handle_*> method is dispatched.
public:
  ACE_WFMO_Reactor_Notify (void);
  // Constructor

  virtual int open (ACE_Reactor_Impl *wfmo_reactor,
                    ACE_Timer_Queue *timer_queue,
                    int disable_notify = 0);
  // Initialization.  <timer_queue> is stored to call <gettimeofday>.

  virtual int close (void);
  // No-op.

  ssize_t notify (ACE_Event_Handler *event_handler = 0,
                  ACE_Reactor_Mask mask = ACE_Event_Handler::EXCEPT_MASK,
                  ACE_Time_Value *timeout = 0);
  // Special trick to unblock <WaitForMultipleObjects> when updates
  // occur.  All we do is enqueue <event_handler> and <mask> onto the
  // <ACE_Message_Queue> and wakeup the <WFMO_Reactor> by signaling
  // its <ACE_Event> handle.  The <ACE_Time_Value> indicates how long
  // to blocking trying to notify the <WFMO_Reactor>.  If <timeout> ==
  // 0, the caller will block until action is possible, else will wait
  // until the relative time specified in <timeout> elapses).

  virtual int dispatch_notifications (int &number_of_active_handles,
                                      const ACE_Handle_Set &rd_mask);
  // No-op.

  virtual ACE_HANDLE get_handle (void) const;
  // Returns a handle to the <ACE_Auto_Event>.

  void max_notify_iterations (int);
  // Set the maximum number of times that the
  // <ACE_WFMO_Reactor_Notify::handle_input> method will iterate and
  // dispatch the <ACE_Event_Handlers> that are passed in via the
  // notify queue before breaking out of its
  // <ACE_Message_Queue::dequeue> loop.  By default, this is set to
  // -1, which means "iterate until the queue is empty."  Setting this
  // to a value like "1 or 2" will increase "fairness" (and thus
  // prevent starvation) at the expense of slightly higher dispatching
  // overhead.

  int max_notify_iterations (void);
  // Get the maximum number of times that the
  // <ACE_WFMO_Reactor_Notify::handle_input> method will iterate and
  // dispatch the <ACE_Event_Handlers> that are passed in via the
  // notify queue before breaking out of its
  // <ACE_Message_Queue::dequeue> loop.

  virtual void dump (void) const;
  // Dump the state of an object.

private:
  ACE_Timer_Queue *timer_queue_;
  // Pointer to the wfmo_reactor's timer queue.

  virtual int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  // Called when the notification event waited on by
  // <ACE_WFMO_Reactor> is signaled.  This dequeues all pending
  // <ACE_Event_Handlers> and dispatches them.

  ACE_Auto_Event wakeup_one_thread_;
  // An auto event is used so that we can <signal> it to wakeup one
  // thread up (e.g., when the <notify> method is called).

#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
// because Sun C++ 4.1 can't cope with this declaration:
  ACE_Message_Queue<ACE_MT_SYNCH> message_queue_;
#endif /* ACE_WIN32 */
  // Message queue that keeps track of pending <ACE_Event_Handlers>.
  // This queue must be thread-safe because it can be called by
  // multiple threads of control.

  int max_notify_iterations_;
  // Keeps track of the maximum number of times that the
  // <ACE_WFMO_Reactor_Notify::handle_input> method will iterate and
  // dispatch the <ACE_Event_Handlers> that are passed in via the
  // notify queue before breaking out of its
  // <ACE_Message_Queue::dequeue> loop.  By default, this is set to
  // -1, which means "iterate until the queue is empty."
};

#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
class ACE_Export ACE_WFMO_Reactor : public ACE_Reactor_Impl
{
  // = TITLE
  //     An object oriented event demultiplexor and event handler
  //     WFMO_Reactor for Win32 WaitForMultipleObjects
  //
  // = DESCRIPTION
  //     The ACE_WFMO_Reactor is an object-oriented event
  //     demultiplexor and event handler Reactor.  The sources of
  //     events that the ACE_WFMO_Reactor waits for and dispatches
  //     includes I/O events, general Win32 synchronization events
  //     (such as mutexes, semaphores, threads, etc.) and timer
  //     events.
  //
  //     Note that changes to the state of WFMO_Reactor are not
  //     instantaneous.  Most changes (registration, removal,
  //     suspension, and resumption of handles, and changes in
  //     ownership) are made when the WFMO_Reactor reaches a stable
  //     state.  Users should be careful, specially when removing
  //     handlers.  This is because the WFMO_Reactor will call
  //     handle_close on the handler when it is finally removed and
  //     not when remove_handler is called.  If the handler is not
  //     going to be around when the WFMO_Reactor calls
  //     handler->handle_close(), use the DONT_CALL flag with
  //     remove_handler().  Or else, dynamically allocate the handler,
  //     and then call "delete this" inside handler->handle_close().
public:
  friend class ACE_WFMO_Reactor_Handler_Repository;
  friend class ACE_WFMO_Reactor_Test;

  enum
  {
    DEFAULT_SIZE = MAXIMUM_WAIT_OBJECTS - 2
    // Default size of the WFMO_Reactor's handle table. Two slots will
    // be added to the <size> parameter in the constructor and open
    // methods which will store handles used for internal management
    // purposes.
  };

  // = Initialization and termination methods.

  ACE_WFMO_Reactor (ACE_Sig_Handler * = 0,
                    ACE_Timer_Queue * = 0);
  // Initialize <ACE_WFMO_Reactor> with the default size.

  ACE_WFMO_Reactor (size_t size,
                    int unused = 0,
                    ACE_Sig_Handler * = 0,
                    ACE_Timer_Queue * = 0);
  // Initialize <ACE_WFMO_Reactor> with size <size>.  <size> should
  // not exceed <ACE_WFMO_Reactor::DEFAULT_SIZE>. Two slots will be
  // added to the <size> parameter which will store handles used for
  // internal management purposes.

  virtual int open (size_t size = ACE_WFMO_Reactor::DEFAULT_SIZE,
                    int restart = 0,
                    ACE_Sig_Handler * = 0,
                    ACE_Timer_Queue * = 0,
                    int disable_notify_pipe = 0,
                    ACE_Reactor_Notify * = 0);
  // Initialize <ACE_WFMO_Reactor> with size <size>.  <size> should
  // not exceed <ACE_WFMO_Reactor::DEFAULT_SIZE>. Two slots will be
  // added to the <size> parameter which will store handles used for
  // internal management purposes.

  virtual int current_info (ACE_HANDLE, size_t & /* size */);
  // Returns -1 (not used in this implementation);

  virtual int set_sig_handler (ACE_Sig_Handler *signal_handler);
  // Use a user specified signal handler instead.

  virtual int set_timer_queue (ACE_Timer_Queue *timer_queue);
  // Use a user specified timer queue instead.

  virtual int close (void);
  // Close down the WFMO_Reactor and release all of its resources.

  virtual ~ACE_WFMO_Reactor (void);
  // Close down the WFMO_Reactor and release all of its resources.

  // = Event loop drivers.

  virtual int handle_events (ACE_Time_Value *max_wait_time = 0);
  virtual int alertable_handle_events (ACE_Time_Value *max_wait_time = 0);
  // This event loop driver blocks for up to <max_wait_time> before
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
  // <WaitForMultipleObjects> is used as the demultiplexing call
  //
  // Returns the total number of I/O and timer <ACE_Event_Handler>s
  // that were dispatched, 0 if the <max_wait_time> elapsed without
  // dispatching any handlers, or -1 if an error occurs.
  //
  // The only difference between <alertable_handle_events> and
  // <handle_events> is that in the alertable case, TRUE is passed to
  // <WaitForMultipleObjects> for the <bAlertable> option.

  virtual int handle_events (ACE_Time_Value &max_wait_time);
  virtual int alertable_handle_events (ACE_Time_Value &max_wait_time);
  // This method is just like the one above, except the
  // <max_wait_time> value is a reference and can therefore never be
  // NULL.
  //
  // The only difference between <alertable_handle_events> and
  // <handle_events> is that in the alertable case, TRUE is passed to
  // <WaitForMultipleObjects> for the <bAlertable> option.


  // = Register and remove Handlers.

  virtual int register_handler (ACE_Event_Handler *event_handler,
                                ACE_HANDLE event_handle = ACE_INVALID_HANDLE);
  // Register an <ACE_Event_Handler> <event_handler>.  Since no Event
  // Mask is passed through this interface, it is assumed that the
  // <handle> being passed in is an event handle and when the event
  // becomes signaled, <WFMO_Reactor> will call handle_signal on
  // <event_handler>.  If <handle> == <ACE_INVALID_HANDLE> the
  // <ACE_WFMO_Reactor> will call the <get_handle> method of
  // <event_handler> to extract the underlying event handle.

  virtual int register_handler (ACE_HANDLE event_handle,
                                ACE_HANDLE io_handle,
                                ACE_Event_Handler *event_handler,
                                ACE_Reactor_Mask mask);
  // Register an <ACE_Event_Handler> <event_handle>.  <mask> specifies
  // the network events that the <event_handler> is interested in.  If
  // <io_handle> == <ACE_INVALID_HANDLE> the <ACE_WFMO_Reactor> will
  // call the <get_handle> method of <event_handler> to extract the
  // underlying I/O handle. If the <event_handle> ==
  // <ACE_INVALID_HANDLE>, WFMO_Reactor will create an event for
  // associating it with the I/O handle. When the <event_handle> is
  // signalled, the appropriate <handle_*> callback will be invoked on
  // the <Event_Handler>

  virtual int register_handler (ACE_HANDLE io_handle,
                                ACE_Event_Handler *event_handler,
                                ACE_Reactor_Mask mask);
  // This is a simple version of the above <register_handler> method
  // where the I/O handle is passed in and the event handle will
  // always be created by <WFMO_Reactor>

  virtual int register_handler (ACE_Event_Handler *event_handler,
                                ACE_Reactor_Mask mask);
  // This is a simple version of the above <register_handler> method
  // where the I/O handle will always come from <get_handle> on the
  // <Event_Handler> and the event handle will always be created by
  // <WFMO_Reactor>

  virtual int register_handler (const ACE_Handle_Set &handles,
                                ACE_Event_Handler *event_handler,
                                ACE_Reactor_Mask mask);
  // Register <event_handler> with all the <handles> in the
  // <Handle_Set>.

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

  virtual int remove_handler (ACE_Event_Handler *event_handler,
                              ACE_Reactor_Mask mask);
  // Removes <event_handler> from the <ACE_WFMO_Reactor>.  Note that
  // the <ACE_WFMO_Reactor> will call the <get_handle> method of
  // <event_handler> to extract the underlying handle.  If <mask> ==
  // <ACE_Event_Handler::DONT_CALL> then the <handle_close> method of
  // the <event_handler> is not invoked. Note that the <handle> can
  // either be the <event_handle> or the <io_handle>

  virtual int remove_handler (ACE_HANDLE handle,
                              ACE_Reactor_Mask mask);
  // Removes <handle> from the <ACE_WFMO_Reactor>.  If <mask> ==
  // <ACE_Event_Handler::DONT_CALL> then the <handle_close> method of
  // the <event_handler> is not invoked. Note that the <handle> can
  // either be the <event_handle> or the <io_handle>
  //
  // For the case of I/O entries, this removes the <mask> binding of
  // <Event_Handler> whose handle is <handle> from <WFMO_Reactor>.  If
  // there are no more bindings for this <event_handler> then it is
  // removed from the WFMO_Reactor.  For simple event entries, mask is
  // mostly ignored and the <Event_Handler> is always removed from
  // <WFMO_Reactor>

  virtual int remove_handler (const ACE_Handle_Set &handle_set,
                              ACE_Reactor_Mask);
  // Removes all the <mask> bindings for handles in the <handle_set>
  // bind of <Event_Handler>.  If there are no more bindings for any
  // of these handles then they are removed from WFMO_Reactor.

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

  virtual int suspend_handler (ACE_Event_Handler *event_handler);
  // Suspend <event_handler> temporarily.  Use
  // <event_handler->get_handle()> to get the handle.

  virtual int suspend_handler (ACE_HANDLE handle);
  // Suspend <handle> temporarily.

  virtual int suspend_handler (const ACE_Handle_Set &handles);
  // Suspend all <handles> in handle set temporarily.

  virtual int suspend_handlers (void);
  // Suspend all <handles> temporarily.

  virtual int resume_handler (ACE_Event_Handler *event_handler);
  // Resume <event_handler>. Use <event_handler->get_handle()> to get
  // the handle.

  virtual int resume_handler (ACE_HANDLE handle);
  // Resume <handle>.

  virtual int resume_handler (const ACE_Handle_Set &handles);
  // Resume all <handles> in handle set.

  virtual int resume_handlers (void);
  // Resume all <handles>.

  virtual int uses_event_associations (void);
  // Return 1 if we any event associations were made by the reactor
  // for the handles that it waits on, 0 otherwise. Since the
  // WFMO_Reactor does use event associations, this function always
  // return 1.

  // Timer management.

  virtual long schedule_timer (ACE_Event_Handler *event_handler,
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

  virtual int cancel_timer (ACE_Event_Handler *event_handler,
                            int dont_call_handle_close = 1);
  // Cancel all Event_Handlers that match the address of
  // <event_handler>.  Returns number of handler's cancelled.

  virtual int cancel_timer (long timer_id,
                            const void **arg = 0,
                            int dont_call_handle_close = 1);
  // Cancel the single Event_Handler that matches the <timer_id> value
  // (which was returned from the schedule method).  If arg is
  // non-NULL then it will be set to point to the ``magic cookie''
  // argument passed in when the Event_Handler was registered.  This
  // makes it possible to free up the memory and avoid memory leaks.
  // Returns 1 if cancellation succeeded and 0 if the <timer_id>
  // wasn't found.

  // = High-level Event_Handler scheduling operations

  virtual int schedule_wakeup (ACE_Event_Handler *event_handler,
                               ACE_Reactor_Mask masks_to_be_added);
  // Add <masks_to_be_added> to the <event_handler>'s entry in
  // WFMO_Reactor.  <event_handler> must already have been registered
  // with WFMO_Reactor.

  virtual int schedule_wakeup (ACE_HANDLE handle,
                               ACE_Reactor_Mask masks_to_be_added);
  // Add <masks_to_be_added> to the <handle>'s entry in WFMO_Reactor.
  // The Event_Handler associated with <handle> must already have been
  // registered with WFMO_Reactor.

  virtual int cancel_wakeup (ACE_Event_Handler *event_handler,
                             ACE_Reactor_Mask masks_to_be_deleted);
  // Remove <masks_to_be_deleted> to the <handle>'s entry in
  // WFMO_Reactor.  The Event_Handler associated with <handle> must
  // already have been registered with WFMO_Reactor.

  virtual int cancel_wakeup (ACE_HANDLE handle,
                             ACE_Reactor_Mask masks_to_be_deleted);
  // Remove <masks_to_be_deleted> to the <handle>'s entry in
  // WFMO_Reactor.  The Event_Handler associated with <handle> must
  // already have been registered with WFMO_Reactor.

  // = Notification methods.

  virtual int notify (ACE_Event_Handler * = 0,
                      ACE_Reactor_Mask = ACE_Event_Handler::EXCEPT_MASK,
                      ACE_Time_Value * = 0);
  // Wakeup one <ACE_WFMO_Reactor> thread if it is currently blocked
  // in <WaitForMultipleObjects>.  The <ACE_Time_Value> indicates how
  // long to blocking trying to notify the <WFMO_Reactor>.  If
  // <timeout> == 0, the caller will block until action is possible,
  // else will wait until the relative time specified in <timeout>
  // elapses).

  virtual void max_notify_iterations (int);
  // Set the maximum number of times that the
  // <ACE_WFMO_Reactor_Notify::handle_input> method will iterate and
  // dispatch the <ACE_Event_Handlers> that are passed in via the
  // notify queue before breaking out of its
  // <ACE_Message_Queue::dequeue> loop.  By default, this is set to
  // -1, which means "iterate until the queue is empty."  Setting this
  // to a value like "1 or 2" will increase "fairness" (and thus
  // prevent starvation) at the expense of slightly higher dispatching
  // overhead.

  virtual int max_notify_iterations (void);
  // Get the maximum number of times that the
  // <ACE_WFMO_Reactor_Notify::handle_input> method will iterate and
  // dispatch the <ACE_Event_Handlers> that are passed in via the
  // notify queue before breaking out of its
  // <ACE_Message_Queue::dequeue> loop.

  virtual int handler (ACE_HANDLE handle,
                       ACE_Reactor_Mask mask,
                       ACE_Event_Handler **event_handler = 0);
  // Not implemented

  virtual int handler (int signum,
                       ACE_Event_Handler ** = 0);
  // Check to see if <signum> is associated with a valid Event_Handler
  // bound to a signal.  Return the <event_handler> associated with
  // this <handler> if <event_handler> != 0.

  virtual int initialized (void);
  // Returns true if WFMO_Reactor has been successfully initialized, else
  // false.

  virtual size_t size (void);
  // Returns the current size of the WFMO_Reactor's internal
  // descriptor table.

  virtual ACE_Lock &lock (void);
  // Returns a reference to the WFMO_Reactor's internal lock.

  virtual void wakeup_all_threads (void);
  // Wake up all threads in WaitForMultipleObjects so that they can
  // reconsult the handle set

  virtual int owner (ACE_thread_t new_owner, ACE_thread_t *old_owner = 0);
  // Transfers ownership of the WFMO_Reactor to the <new_owner>. The
  // transfer will not complete until all threads are ready (just like
  // the handle set).

  virtual int owner (ACE_thread_t *owner);
  // Return the ID of the "owner" thread.

  virtual void requeue_position (int);
  // Not implemented

  virtual int requeue_position (void);
  // Not implemented

  // = Low-level wait_set mask manipulation methods.

  virtual int mask_ops (ACE_Event_Handler *event_handler,
                        ACE_Reactor_Mask masks,
                        int operation);
  // Modify <masks> of the <event_handler>'s entry in WFMO_Reactor
  // depending upon <operation>.  <event_handler> must already have
  // been registered with WFMO_Reactor.

  virtual int mask_ops (ACE_HANDLE handle,
                        ACE_Reactor_Mask masks,
                        int ops);
  // Modify <masks> of the <handle>'s entry in WFMO_Reactor depending
  // upon <operation>.  <handle> must already have been registered
  // with WFMO_Reactor.

  // = Low-level ready_set mask manipulation methods.

  virtual int ready_ops (ACE_Event_Handler *event_handler,
                         ACE_Reactor_Mask mask,
                         int ops);
  // Not implemented

  virtual int ready_ops (ACE_HANDLE handle,
                         ACE_Reactor_Mask,
                         int ops);
  // Not implemented

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  virtual void dump (void) const;
  // Dump the state of an object.

protected:
  virtual int register_handler_i (ACE_HANDLE event_handle,
                                  ACE_HANDLE io_handle,
                                  ACE_Event_Handler *event_handler,
                                  ACE_Reactor_Mask mask);
  // Registration workhorse

  virtual int event_handling (ACE_Time_Value *max_wait_time = 0,
                              int alertable = 0);
  // Event handling workhorse

  virtual int mask_ops_i (ACE_HANDLE io_handle,
                          ACE_Reactor_Mask masks,
                          int operation);
  // Bit masking workhorse

  virtual ACE_thread_t owner_i (void);
  // Return the ID of the "owner" thread. Does not do any locking.

  virtual int ok_to_wait (ACE_Time_Value *max_wait_time,
                          int alertable);
  // Check to see if it is ok to enter ::WaitForMultipleObjects().

  virtual int wait_for_multiple_events (int timeout,
                                        int alertable);
  // Wait for timer and I/O events to occur.

  virtual DWORD poll_remaining_handles (size_t index);
  // Check for activity on remaining handles.

  virtual int expire_timers (void);
  // Expire timers. Only the owner thread does useful stuff in this
  // function.

  virtual int dispatch (int wait_status);
  // Dispatches the timers and I/O handlers.

  virtual int safe_dispatch (int wait_status);
  // Protect against structured exceptions caused by user code when
  // dispatching handles

  virtual int dispatch_handles (size_t index);
  // Dispatches any active handles from handles_[<index>] to
  // handles_[active_handles_] using <WaitForMultipleObjects> to poll
  // through our handle set looking for active handles.

  virtual int dispatch_handler (size_t index,
                                size_t max_handlep1);
  // Dispatches a single handler.  Returns 0 on success, -1 if the
  // handler was removed.

  virtual int simple_dispatch_handler (int index,
                                       ACE_HANDLE event_handle);
  // Dispatches a single handler.  Returns 0 on success, -1 if the
  // handler was removed.

  virtual int complex_dispatch_handler (int index,
                                        ACE_HANDLE event_handle);
  // Dispatches a single handler.  Returns 0 on success, -1 if the
  // handler was removed.

  virtual int dispatch_window_messages (void);
  // Dispatches window messages. Noop for WFMO_Reactor.

  virtual ACE_Reactor_Mask upcall (ACE_Event_Handler *event_handler,
                                   ACE_HANDLE io_handle,
                                   ACE_HANDLE event_handle,
                                   long interested_events);

  virtual int calculate_timeout (ACE_Time_Value *time);
  // Used to caluculate the next timeout

  virtual int update_state (void);
  // Update the state of the handler repository

  virtual int new_owner (void);
  // Check to see if we have a new owner

  virtual int change_owner (void);
  // Set owner to new owner

  ACE_Sig_Handler *signal_handler_;
  // Handle signals without requiring global/static variables.

  int delete_signal_handler_;
  // Keeps track of whether we should delete the signal handler (if we
  // didn't create it, then we don't delete it).

  ACE_Timer_Queue *timer_queue_;
  // Defined as a pointer to allow overriding by derived classes...

  int delete_timer_queue_;
  // Keeps track of whether we should delete the timer queue (if we
  // didn't create it, then we don't delete it).

  int delete_handler_rep_;
  // Keeps track of whether we should delete the handler repository

  ACE_Reactor_Notify *notify_handler_;
  // Used when <notify> is called.

  int delete_notify_handler_;
  // Keeps track of whether we should delete the notify handler.

  ACE_Process_Mutex lock_;
  // Synchronization for the ACE_WFMO_Reactor.
  //
  // A Process Mutex is used here because of two reasons:
  // (a) The implementation of ACE_Thread_Mutex uses CriticalSections
  //     CriticalSections are not waitable using ::WaitForMultipleObjects
  // (b) This is really not a process mutex because it is not
  //     named. No other process can use this mutex.

  ACE_Lock_Adapter<ACE_Process_Mutex> lock_adapter_;
  // Adapter used to return internal lock to outside world.

  ACE_WFMO_Reactor_Handler_Repository handler_rep_;
  // Table that maps <ACE_HANDLEs> to <ACE_Event_Handler *>'s.

  ACE_Manual_Event ok_to_wait_;
  // A manual event used to block threads from proceeding into
  // WaitForMultipleObjects

  ACE_Manual_Event wakeup_all_threads_;
  // A manual event is used so that we can wake everyone up (e.g.,
  // when <ACE_Event_Handlers> are bounded and unbound from the
  // handler repository).

  ACE_Wakeup_All_Threads_Handler wakeup_all_threads_handler_;
  // Used when <wakeup_all_threads_> is signaled

  ACE_Auto_Event waiting_to_change_state_;
  // The changing thread waits on this event, till all threads are not
  // active anymore

  size_t active_threads_;
  // Count of currently active threads

  ACE_thread_t owner_;
  // The thread which is "owner" of the WFMO_Reactor. The owner
  // concept is used because we don't want multiple threads to try to
  // expire timers. Therefore the "owner" thread is the only one
  // allowed to expire timers. Also, the owner thread is the only
  // thread which waits on the notify handle. Note that the ownership
  // can be transferred.

  ACE_thread_t new_owner_;
  // The owner to be of the WFMO_Reactor

  ACE_thread_t change_state_thread_;
  // This is the thread which is responsible for the changing the
  // state of the <WFMO_Reactor> handle set

  ACE_HANDLE atomic_wait_array_ [2];
  // This is an array of ACE_HANDLEs which keep track of the <lock_>
  // and <ok_to_wait_> handles

  int open_for_business_;
  // This flag is used to keep track of whether we are already closed.

private:
  ACE_WFMO_Reactor (const ACE_WFMO_Reactor &);
  ACE_WFMO_Reactor &operator = (const ACE_WFMO_Reactor &);
  // Deny access since member-wise won't work...
};

// If we don't have WinSOCK2, we need these defined
#if !defined (ACE_HAS_WINSOCK2) || (ACE_HAS_WINSOCK2 == 0)
/*
 * WinSock 2 extension -- bit values and indices for FD_XXX network events
 */
#define FD_READ_BIT      0
#define FD_WRITE_BIT     1
#define FD_OOB_BIT       2
#define FD_ACCEPT_BIT    3
#define FD_CONNECT_BIT   4
#define FD_CLOSE_BIT     5
#define FD_QOS_BIT       6
#define FD_GROUP_QOS_BIT 7

#define FD_QOS           (1 << FD_QOS_BIT)
#define FD_GROUP_QOS     (1 << FD_GROUP_QOS_BIT)

#define FD_MAX_EVENTS    8
#define FD_ALL_EVENTS    ((1 << FD_MAX_EVENTS) - 1)

#define WSAEVENT                HANDLE

typedef struct _WSANETWORKEVENTS
{
  long lNetworkEvents;
  int iErrorCode[FD_MAX_EVENTS];
} WSANETWORKEVENTS, FAR * LPWSANETWORKEVENTS;

int WSAEventSelect (SOCKET s,
                    WSAEVENT hEventObject,
                    long lNetworkEvents);

int WSAEnumNetworkEvents (SOCKET s,
                          WSAEVENT hEventObject,
                          LPWSANETWORKEVENTS lpNetworkEvents);

#endif /* !defined ACE_HAS_WINSOCK2 */

#endif /* ACE_WIN32 */

#if defined (__ACE_INLINE__)
#include "ace/WFMO_Reactor.i"
#endif /* __ACE_INLINE__ */
#endif /* ACE_WFMO_REACTOR_H */
