// -*- C++ -*-

// =========================================================================
/**
 *  @file    Dev_Poll_Reactor.h
 *
 *  $Id$
 *
 *  EXPERIMENTAL /dev/poll (or Linux /dev/epoll) based Reactor
 *  implementation.
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
// =========================================================================


#ifndef ACE_DEV_POLL_REACTOR_H
#define ACE_DEV_POLL_REACTOR_H

#include /**/ "ace/pre.h"

#include "ace/Reactor_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#if defined (ACE_HAS_EVENT_POLL) && defined (ACE_HAS_DEV_POLL)
#  error ACE_HAS_EVENT_POLL and ACE_HAS_DEV_POLL are mutually exclusive.
#endif  /* ACE_HAS_EVENT_POLL && defined ACE_HAS_DEV_POLL */


#if defined (ACE_HAS_EVENT_POLL) || defined (ACE_HAS_DEV_POLL)

#include "ace/Pipe.h"

// Forward declarations
class ACE_Sig_Handler;
class ACE_Dev_Poll_Reactor;

/**
 * @class ACE_Dev_Poll_Event_Tuple
 *
 * @brief Class that associates specific event mask with a given event
 *        handler.
 *
 * This class merely provides a means to associate an event mask
 * with an event handler.  Such an association is needed since it is
 * not possible to retrieve the event mask from the "interest set"
 * stored in the `/dev/poll' or `/dev/epoll' driver.  Without this
 * external association, it would not be possible keep track of the
 * event mask for a given event handler when suspending it or resuming
 * it.
 *
 * @note An ACE_Handle_Set is not used since the number of handles may
 *       exceed its capacity (ACE_DEFAULT_SELECT_REACTOR_SIZE).
 */
class ACE_Dev_Poll_Event_Tuple
{
public:

  /// Constructor.
  ACE_Dev_Poll_Event_Tuple (void);

public:

  /// The event handler.
  ACE_Event_Handler *event_handler;

  /// The event mask for the above event handler.
  ACE_Reactor_Mask mask;

  /// Flag that states whether or not the event handler is suspended.
  char suspended;

  /// The number of outstanding upcalls occurring on the above event
  /// handler.
  /**
   * @todo The reference count should really be maintained within the
   *       event handler.  This approach was taken to allow for
   *       backward compatibility and quick implementation.  One
   *       approach for maintaining backward compatibility while
   *       implementing reference counting within the event handler is
   *       to create an ACE_Ref_Counted_Event_Handler "mix-in" class
   *       that concrete ACE_Event_Handlers can inherit from
   *       (i.e. multiple inheritance).  Thus, legacy non-reference
   *       counted event handlers need not pay for reference counting
   *       resources.
   */
  unsigned long refcount;

};

// ---------------------------------------------------------------------

#if 0
/**
 * @class ACE_Dev_Poll_Ready_Set
 *
 * @brief Class that contains the list of "ready" file descriptors.
 *
 * This class points to an array of pollfd structures corresponding to
 * "ready" file descriptors, such as those corresponding to event
 * handlers that request an additional callback after being initially
 * dispatched (i.e. return a value greater than zero).
 * @par
 * The idea is to store the "ready" set in an existing area of memory
 * that already contains pollfd instances.  Doing so is safe since the
 * "ready" set is dispatched before polling for additional events,
 * thus avoiding being potentially overwritten during the event poll.
 * @par
 * When the "ready" set is dispatched, all that needs to be done is to
 * iterate over the contents of the array.  There is no need to "walk"
 * the array in search of ready file descriptors since the array by
 * design only contains ready file descriptors.  As such, this
 * implementation of a ready set is much more efficient in the
 * presence of a large number of file descriptors in terms of both
 * time and space than the one used in the Select_Reactor, for
 * example.
 */
class ACE_Dev_Poll_Ready_Set
{
public:

  /// Constructor.
  ACE_Dev_Poll_Ready_Set (void);

public:

  /// The array containing the pollfd structures corresponding to the
  /// "ready" file descriptors.
  struct pollfd *pfds;

  /// The number of "ready" file descriptors in the above array.
  int nfds;

};
#endif  /* 0 */

// ---------------------------------------------------------------------

/**
 * @class ACE_Dev_Poll_Reactor_Notify
 *
 * @brief Event handler used for unblocking the ACE_Dev_Poll_Reactor
 *        from its event loop.
 *
 * This event handler is used internally by the ACE_Dev_Poll_Reactor
 * as a means to allow a thread other then the one running the event
 * loop to unblock the event loop.
 */
class ACE_Dev_Poll_Reactor_Notify : public ACE_Reactor_Notify
{
public:

  /// Constructor
  ACE_Dev_Poll_Reactor_Notify (void);

  /**
   * @name Initialization and Termination Methods
   *
   * Methods called when initializing and terminating this event
   * handler.
   */
  virtual int open (ACE_Reactor_Impl *,
                    ACE_Timer_Queue *timer_queue = 0,
                    int disable_notify = 0);
  virtual int close (void);

  /**
   * Called by a thread when it wants to unblock the Reactor_Impl.
   * This wakeups the Reactor_Impl if currently blocked.  Pass over
   * both the Event_Handler *and* the mask to allow the caller to
   * dictate which Event_Handler method the Reactor_Impl will
   * invoke.  The ACE_Time_Value indicates how long to blocking
   * trying to notify the Reactor_Impl.  If timeout == 0, the
   * caller will block until action is possible, else will wait until
   * the relative time specified in *timeout elapses).
   */
  virtual ssize_t notify (ACE_Event_Handler * = 0,
                          ACE_Reactor_Mask = ACE_Event_Handler::EXCEPT_MASK,
                          ACE_Time_Value * = 0);

  /// Unimplemented method required by pure virtual method in abstract
  /// base class.
  /**
   * This method's interface is not very compatibile with this
   * Reactor's design.  It's not clear why this method is pure virtual
   * either.
   */
  virtual int dispatch_notifications (int &number_of_active_handles,
                                      ACE_Handle_Set &rd_mask);

  /// Returns the ACE_HANDLE of the notify pipe on which the reactor
  /// is listening for notifications so that other threads can unblock
  /// the Reactor_Impl.
  virtual ACE_HANDLE notify_handle (void);

  /// Verify whether the buffer has dispatchable info  or not.
  virtual int is_dispatchable (ACE_Notification_Buffer &buffer);

  /// Handle one of the notify call on the handle.  This could be
  /// because of a thread trying to unblock the Reactor_Impl.
  virtual int dispatch_notify (ACE_Notification_Buffer &buffer);

  /// Read one of the notify call on the handle into the
  /// buffer. This could be because of a thread trying to unblock
  /// the Reactor_Impl.
  virtual int read_notify_pipe (ACE_HANDLE handle,
                                ACE_Notification_Buffer &buffer);

  /// Called back by the ACE_Dev_Poll_Reactor when a thread wants to
  /// unblock us.
  virtual int handle_input (ACE_HANDLE handle);

  /**
   * Set the maximum number of times that the handle_input method
   * will iterate and dispatch the ACE_Event_Handlers that are
   * passed in via the notify queue before breaking out of the event
   * loop.  By default, this is set to -1, which means "iterate until
   * the queue is empty."  Setting this to a value like "1 or 2" will
   * increase "fairness" (and thus prevent starvation) at the expense
   * of slightly higher dispatching overhead.
   */
  virtual void max_notify_iterations (int);

  /**
   * Get the maximum number of times that the handle_input method
   * will iterate and dispatch the ACE_Event_Handlers that are
   * passed in via the notify queue before breaking out of its event
   * loop.
   */
  virtual int max_notify_iterations (void);

  /**
   * Purge any notifications pending in this reactor for the specified
   * ACE_Event_Handler object. Returns the number of notifications
   * purged. Returns -1 on error.
   */
  virtual int purge_pending_notifications (
    ACE_Event_Handler * = 0,
    ACE_Reactor_Mask    = ACE_Event_Handler::ALL_EVENTS_MASK);



  /// Dump the state of an object.
  virtual void dump (void) const;

protected:

  /**
   * Keep a back pointer to the ACE_Dev_Poll_Reactor.  If this value
   * if NULL then the ACE_Dev_Poll_Reactor has been initialized with
   * disable_notify_pipe.
   */
  ACE_Dev_Poll_Reactor *dp_reactor_;

  /**
   * Contains the ACE_HANDLE the ACE_Dev_Poll_Reactor is listening
   * on, as well as the ACE_HANDLE that threads wanting the attention
   * of the ACE_Dev_Poll_Reactor will write to.
   */
  ACE_Pipe notification_pipe_;

  /**
   * Keeps track of the maximum number of times that the
   * ACE_Dev_Poll_Reactor_Notify::handle_input method will iterate and
   * dispatch the ACE_Event_Handlers that are passed in via the
   * notify pipe before breaking out of its recv loop.  By default,
   * this is set to -1, which means "iterate until the pipe is empty."
   */
  int max_notify_iterations_;

#if defined (ACE_HAS_REACTOR_NOTIFICATION_QUEUE)
  /**
   * @name Reactor Notification Attributes
   *
   * This configuration queues up notifications in separate buffers
   * that are in user-space, rather than stored in a pipe in the OS
   * kernel.  The kernel-level notifications are used only to trigger
   * the Reactor to check its notification queue.  This enables many
   * more notifications to be stored than would otherwise be the
   * case.
   */
  //@{

  /// Keeps track of allocated arrays of type
  /// ACE_Notification_Buffer.
  ACE_Unbounded_Queue <ACE_Notification_Buffer *> alloc_queue_;

  /// Keeps track of all pending notifications.
  ACE_Unbounded_Queue <ACE_Notification_Buffer *> notify_queue_;

  /// Keeps track of all free buffers.
  ACE_Unbounded_Queue <ACE_Notification_Buffer *> free_queue_;

  /// Synchronization for handling of queues.
  ACE_SYNCH_MUTEX notify_queue_lock_;

  //@}
#endif /* ACE_HAS_REACTOR_NOTIFICATION_QUEUE */

};

// ---------------------------------------------------------------------

/**
 * @class ACE_Dev_Poll_Reactor_Handler_Repository
 *
 * @brief Used to map ACE_HANDLEs onto the appropriate
 *        ACE_Event_Handler *.
 *
 * This class is simply a container that maps a handle to its
 * corresponding event handler.  It is not meant for use outside of
 * the Dev_Poll_Reactor.
 */
class ACE_Dev_Poll_Reactor_Handler_Repository
{
public:

  /// Constructor.
  ACE_Dev_Poll_Reactor_Handler_Repository (void);

  /// Initialize a repository of the appropriate <size>.
  int open (size_t size);

  /// Close down the repository.
  int close (void);

  /**
   * @name Repository Manipulation Operations
   *
   * Methods used to search and modify the handler repository.
   */
  //@{

  /**
   * Return the ACE_Event_Handler associated with ACE_HANDLE.  If
   * index_p is non-zero, then return the index location of the
   * handle, if found.
   */
  ACE_Event_Handler *find (ACE_HANDLE handle, size_t *index_p = 0);

  /// Set the event mask for event handler associated with the given
  /// handle.
  void mask (ACE_HANDLE handle, ACE_Reactor_Mask mask);

  /// Retrieve the event mask for the event handler associated with
  /// the given handle.
  ACE_Reactor_Mask mask (ACE_HANDLE handle);

  /// Mark the event handler associated with the given handle as
  /// "suspended."
  void suspend (ACE_HANDLE handle);

  /// Mark the event handler associated with the given handle as
  /// "resumed."
  void resume (ACE_HANDLE handle);

  /// Is the event handler for the given handle suspended?
  int suspended (ACE_HANDLE handle) const;

  /// Bind the ACE_Event_Handler to the ACE_HANDLE with the
  /// appropriate ACE_Reactor_Mask settings.
  int bind (ACE_HANDLE handle,
            ACE_Event_Handler *handler,
            ACE_Reactor_Mask mask);

  /// Remove the binding of ACE_HANDLE in accordance with the <mask>.
  int unbind (ACE_HANDLE handle);

  /// Remove all the (ACE_HANDLE, ACE_Event_Handler) tuples.
  int unbind_all (void);

  /// Increase the reference count on the event handler corresponding
  /// to the given file descriptor.
  /**
   * @return Returns the updated reference count.
   */
  unsigned long add_ref (ACE_HANDLE handle);

  /// Decrease the reference count on the event handler corresponding
  /// to the given file descriptor.
  /**
   * @return Returns the updated reference count.
   */
  unsigned long remove_ref (ACE_HANDLE handle);
  //@}

  /**
   * @name Sanity Checking
   *
   * Methods used to prevent "out-of-range" errors when indexing the
   * underlying handler array.
   */
  //@{

  // Check the <handle> to make sure it's a valid ACE_HANDLE that
  // within the range of legal handles (i.e., greater than or equal to
  // zero and less than max_size_).
  int invalid_handle (ACE_HANDLE handle) const;

  // Check the handle to make sure it's a valid ACE_HANDLE that is
  // within the range of currently registered handles (i.e., greater
  // than or equal to zero and less than max_handlep1_).
  int handle_in_range (ACE_HANDLE handle) const;

  //@}

  /// Returns the current table size.
  size_t size (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:

  /// Maximum number of handles.
  int max_size_;

  /// The underlying array of event handlers.
  /**
   * The array of event handlers is directly indexed directly using
   * an ACE_HANDLE value.  This is Unix-specific.
   */
  ACE_Dev_Poll_Event_Tuple *handlers_;

};

// ---------------------------------------------------------------------

/**
 * @class ACE_Dev_Poll_Reactor
 *
 * @brief A `/dev/poll' or `/dev/epoll' based Reactor implemenatation.
 *
 * @note This reactor is EXPERIMENTAL.
 *
 * The ACE_Dev_Poll_Reactor uses the `/dev/poll' or '/dev/epoll'
 * character devices to demultiplex events on a given set of file
 * descriptors.  Unlike select(), `/dev/poll' and `/dev/epoll' have no
 * hard-coded limit on the number of file descriptors that may be
 * handled at any given time.  As such, the ACE_Dev_Poll_Reactor can
 * generally handle a much larger number of file descriptors than
 * select()-based reactors.  Furthermore, since `/dev/poll' and
 * `/dev/epoll' both return a set of file descriptors that are active,
 * there is no need to "walk" the set of file descriptors to determine
 * which ones are active, such as what is done with the select() and
 * poll() system calls.  All returned file descriptors are active.
 * This makes event dispatching very efficient.
 *
 * @note In general, this reactor may only be used to demultiplex
 *       events on sockets.  Demultiplexing events on pipes, for
 *       example may not work.  This is due to a limitation in the
 *       underlying `/dev/poll' device driver.
 *
 * @note It is only possible to achieve millisecond timeout
 *       resolutions with the ACE_Dev_Poll_Reactor.  However, the
 *       timeout resolution for timers is independent of the reactors
 *       timeout resolution.  As such, it may be possible to achieve
 *       sub-millisecond timeout resolutions for timers but that is
 *       entirely platform dependent.
 */
class ACE_Export ACE_Dev_Poll_Reactor : public ACE_Reactor_Impl
{
public:

  /// Initialize ACE_Dev_Poll_Reactor with the default size.
  /**
   * The default size for the ACE_Dev_Poll_Reactor is the maximum
   * number of open file descriptors for the process.
   */
  ACE_Dev_Poll_Reactor (ACE_Sig_Handler * = 0,
                        ACE_Timer_Queue * = 0,
                        int disable_notify_pipe = 0,
                        ACE_Reactor_Notify *notify = 0,
                        int mask_signals = 1);

  /// Initialize ACE_Dev_Poll_Reactor with size "size."
  /**
   * @note On Unix platforms, the size parameter should be as large as
   *       the maximum number of file descriptors allowed for a given
   *       process.  This is necessary since a file descriptor is used
   *       to directly index the array of event handlers maintained by
   *       the Reactor's handler repository.  Direct indexing is used
   *       for efficiency reasons.  If the size parameter is less than
   *       the process maximum, then the process maximum will be
   *       decreased in order to prevent potential access violations.
   */
  ACE_Dev_Poll_Reactor (size_t size,
                        int restart = 0,
                        ACE_Sig_Handler * = 0,
                        ACE_Timer_Queue * = 0,
                        int disable_notify_pipe = 0,
                        ACE_Reactor_Notify *notify = 0,
                        int mask_signals = 1);

  /// Close down and release all resources.
  virtual ~ACE_Dev_Poll_Reactor (void);

  /// Initialization.
  virtual int open (size_t size,
                    int restart = 0,
                    ACE_Sig_Handler * = 0,
                    ACE_Timer_Queue * = 0,
                    int disable_notify_pipe = 0,
                    ACE_Reactor_Notify * = 0);

  /**
   * Returns 0, if the size of the current message has been put in
   * size.  Returns -1, if not.  ACE_HANDLE allows the reactor to
   * check if the caller is valid.
   */
  virtual int current_info (ACE_HANDLE, size_t & /* size */);

  /// Use a user specified signal handler instead.
  virtual int set_sig_handler (ACE_Sig_Handler *signal_handler);

  /// Set a user specified timer queue.
  /**
   * @note This method is deprecated.
   * @see timer_queue
   */
  virtual int set_timer_queue (ACE_Timer_Queue *tq);

  /// Set a user-specified timer queue.
  virtual int timer_queue (ACE_Timer_Queue *tq);

  /// Return the current ACE_Timer_Queue.
  virtual ACE_Timer_Queue *timer_queue (void) const;

  /// Close down and release all resources.
  virtual int close (void);

  // = Event loop drivers.
  /**
   * Returns non-zero if there are I/O events "ready" for dispatching,
   * but does not actually dispatch the event handlers.  By default,
   * don't block while checking this, i.e., "poll".
   *
   * @note It is only possible to achieve millisecond timeout
   *       resolutions with the ACE_Dev_Poll_Reactor.
   */
  virtual int work_pending (
    const ACE_Time_Value &max_wait_time = ACE_Time_Value::zero);

  /**
   * This event loop driver blocks for up to max_wait_time before
   * returning.  It will return earlier if events occur.  Note that
   * max_wait_time can be 0, in which case this method blocks
   * indefinitely until events occur.
   * @par
   * max_wait_time is decremented to reflect how much time this call
   * took.  For instance, if a time value of 3 seconds is passed to
   * handle_events and an event occurs after 2 seconds,
   * max_wait_time will equal 1 second.  This can be used if an
   * application wishes to handle events for some fixed amount of
   * time.
   * @par
   * Returns the total number of ACE_Event_Handlers that were
   * dispatched, 0 if the max_wait_time elapsed without dispatching
   * any handlers, or -1 if an error occurs.
   * @par
   * The only difference between alertable_handle_events and
   * handle_events is that in the alertable case, the event loop
   * will return when the system queues an I/O completion routine or
   * an Asynchronous Procedure Call.
   *
   * @note It is only possible to achieve millisecond timeout
   *       resolutions with the ACE_Dev_Poll_Reactor.
   */
  virtual int handle_events (ACE_Time_Value *max_wait_time = 0);
  virtual int alertable_handle_events (ACE_Time_Value *max_wait_time = 0);

  /**
   * This method is just like the one above, except the
   * max_wait_time value is a reference and can therefore never be
   * NULL.
   *
   * @note It is only possible to achieve millisecond timeout
   *       resolutions with the ACE_Dev_Poll_Reactor.
   */
  virtual int handle_events (ACE_Time_Value &max_wait_time);
  virtual int alertable_handle_events (ACE_Time_Value &max_wait_time);

  // = Event handling control.

  /**
   * Return the status of Reactor.  If this function returns 0, the
   * reactor is actively handling events.  If it returns non-zero,
   * handle_events() and handle_alertable_events() return -1
   * immediately.
   */
  virtual int deactivated (void);

  /**
   * Control whether the Reactor will handle any more incoming events
   * or not.  If do_stop == 1, the Reactor will be disabled.  By
   * default, a reactor is in active state and can be
   * deactivated/reactived as desired.
   */
  virtual void deactivate (int do_stop);

  // = Register and remove Handlers.

  /// Register event_handler with mask.  The I/O handle will always
  /// come from get_handle on the event_handler.
  virtual int register_handler (ACE_Event_Handler *event_handler,
                                ACE_Reactor_Mask mask);

  /// Register event_handler with mask.  The I/O handle is provided
  /// through the io_handle parameter.
  virtual int register_handler (ACE_HANDLE io_handle,
                                ACE_Event_Handler *event_handler,
                                ACE_Reactor_Mask mask);

  /**
   * Register an <event_handler> that will be notified when
   * <event_handle> is signaled.  <mask> specifies the network events
   * that the <event_handler> is interested in.
   */
  virtual int register_handler (ACE_HANDLE event_handle,
                                ACE_HANDLE io_handle,
                                ACE_Event_Handler *event_handler,
                                ACE_Reactor_Mask mask);

  /// Register <event_handler> with all the <handles> in the <Handle_Set>.
  virtual int register_handler (const ACE_Handle_Set &handles,
                                ACE_Event_Handler *event_handler,
                                ACE_Reactor_Mask mask);

  /**
   * Register <new_sh> to handle the signal <signum> using the
   * <new_disp>.  Returns the <old_sh> that was previously registered
   * (if any), along with the <old_disp> of the signal handler.
   */
  virtual int register_handler (int signum,
                                ACE_Event_Handler *new_sh,
                                ACE_Sig_Action *new_disp = 0,
                                ACE_Event_Handler **old_sh = 0,
                                ACE_Sig_Action *old_disp = 0);

  /// Registers <new_sh> to handle a set of signals <sigset> using the
  /// <new_disp>.
  virtual int register_handler (const ACE_Sig_Set &sigset,
                                ACE_Event_Handler *new_sh,
                                ACE_Sig_Action *new_disp = 0);

  /**
   * Removes <event_handler>.  Note that the I/O handle will be
   * obtained using <get_handle> method of <event_handler> .  If
   * <mask> == <ACE_Event_Handler::DONT_CALL> then the <handle_close>
   * method of the <event_handler> is not invoked.
   */
  virtual int remove_handler (ACE_Event_Handler *event_handler,
                              ACE_Reactor_Mask mask);

  /**
   * Removes <handle>.  If <mask> == <ACE_Event_Handler::DONT_CALL>
   * then the <handle_close> method of the associated <event_handler>
   * is not invoked.
   */
  virtual int remove_handler (ACE_HANDLE handle,
                              ACE_Reactor_Mask mask);

  /**
   * Removes all handles in <handle_set>.  If <mask> ==
   * <ACE_Event_Handler::DONT_CALL> then the <handle_close> method of
   * the associated <event_handler>s is not invoked.
   */
  virtual int remove_handler (const ACE_Handle_Set &handle_set,
                              ACE_Reactor_Mask mask);

  /**
   * Remove the ACE_Event_Handler currently associated with <signum>.
   * Install the new disposition (if given) and return the previous
   * disposition (if desired by the caller).  Returns 0 on success and
   * -1 if <signum> is invalid.
   */
  virtual int remove_handler (int signum,
                              ACE_Sig_Action *new_disp,
                              ACE_Sig_Action *old_disp = 0,
                              int sigkey = -1);

  /// Calls <remove_handler> for every signal in <sigset>.
  virtual int remove_handler (const ACE_Sig_Set &sigset);

  // = Suspend and resume Handlers.

  /// Suspend event_handler temporarily.  Use
  /// ACE_Event_Handler::get_handle() to get the handle.
  virtual int suspend_handler (ACE_Event_Handler *event_handler);

  /// Suspend handle temporarily.
  virtual int suspend_handler (ACE_HANDLE handle);

  /// Suspend all handles in handle set temporarily.
  virtual int suspend_handler (const ACE_Handle_Set &handles);

  /// Suspend all handles temporarily.
  virtual int suspend_handlers (void);

  /// Resume event_handler. Use ACE_Event_Handler::get_handle() to
  /// get the handle.
  virtual int resume_handler (ACE_Event_Handler *event_handler);

  /// Resume handle.
  virtual int resume_handler (ACE_HANDLE handle);

  /// Resume all handles in handle set.
  virtual int resume_handler (const ACE_Handle_Set &handles);

  /// Resume all handles.
  virtual int resume_handlers (void);

  /// Does the reactor allow the application to resume the handle on
  /// its own, i.e., can it pass on the control of handle resumption to
  /// the application.
  virtual int resumable_handler (void);

  /// Return 1 if we any event associations were made by the reactor
  /// for the handles that it waits on, 0 otherwise.
  virtual int uses_event_associations (void);

  // = Timer management.

  /**
   * Schedule an ACE_Event_Handler that will expire after an amount
   * of time.  The return value of this method, a timer_id value,
   * uniquely identifies the event_handler in the ACE_Reactor's
   * internal list of timers.
   * This timer_id value can be used to cancel the timer
   * with the cancel_timer() call.
   *
   * @see cancel_timer()
   * @see reset_timer_interval()
   *
   * @param event_handler  event handler to schedule on reactor
   * @param arg   argument passed to the handle_timeout() method of  event_handler
   * @param delay  time interval after which the timer will expire
   * @param interval  time interval after which the timer will be automatically rescheduled
   * @return -1 on failure, a timer_id value on success
   */
  virtual long schedule_timer (ACE_Event_Handler *event_handler,
                               const void *arg,
                               const ACE_Time_Value &delay,
                               const ACE_Time_Value &interval = ACE_Time_Value::zero);

  /**
   * Resets the interval of the timer represented by <timer_id> to
   * <interval>, which is specified in relative time to the current
   * <gettimeofday>.  If <interval> is equal to
   * <ACE_Time_Value::zero>, the timer will become a non-rescheduling
   * timer.  Returns 0 if successful, -1 if not.
   */
  virtual int reset_timer_interval (long timer_id,
                                    const ACE_Time_Value &interval);

  /// Cancel all Event_Handlers that match the address of
  /// <event_handler>.  Returns number of handlers cancelled.
  virtual int cancel_timer (ACE_Event_Handler *event_handler,
                            int dont_call_handle_close = 1);

  /**
   * Cancel the single Event_Handler that matches the <timer_id> value
   * (which was returned from the schedule method).  If arg is
   * non-NULL then it will be set to point to the ``magic cookie''
   * argument passed in when the Event_Handler was registered.  This
   * makes it possible to free up the memory and avoid memory leaks.
   * Returns 1 if cancellation succeeded and 0 if the <timer_id>
   * wasn't found.
   */
  virtual int cancel_timer (long timer_id,
                            const void **arg = 0,
                            int dont_call_handle_close = 1);

  // = High-level Event_Handler scheduling operations

  /// Add <masks_to_be_added> to the <event_handler>'s entry.
  /// <event_handler> must already have been registered.
  virtual int schedule_wakeup (ACE_Event_Handler *event_handler,
                               ACE_Reactor_Mask masks_to_be_added);

  /// Add <masks_to_be_added> to the <handle>'s entry.  <event_handler>
  /// associated with <handle> must already have been registered.
  virtual int schedule_wakeup (ACE_HANDLE handle,
                               ACE_Reactor_Mask masks_to_be_added);

  /// Clear <masks_to_be_cleared> from the <event_handler>'s entry.
  virtual int cancel_wakeup (ACE_Event_Handler *event_handler,
                             ACE_Reactor_Mask masks_to_be_cleared);

  /// Clear <masks_to_be_cleared> from the <handle>'s entry.
  virtual int cancel_wakeup (ACE_HANDLE handle,
                             ACE_Reactor_Mask masks_to_be_cleared);

  // = Notification methods.

  /**
   * Notify <event_handler> of <mask> event.  The <ACE_Time_Value>
   * indicates how long to blocking trying to notify.  If <timeout> ==
   * 0, the caller will block until action is possible, else will wait
   * until the relative time specified in <timeout> elapses).
   */
  virtual int notify (ACE_Event_Handler *event_handler = 0,
                      ACE_Reactor_Mask mask = ACE_Event_Handler::EXCEPT_MASK,
                      ACE_Time_Value * = 0);

  /**
   * Set the maximum number of times that ACE_Reactor_Impl will
   * iterate and dispatch the <ACE_Event_Handlers> that are passed in
   * via the notify queue before breaking out of its
   * <ACE_Message_Queue::dequeue> loop.  By default, this is set to
   * -1, which means "iterate until the queue is empty."  Setting this
   * to a value like "1 or 2" will increase "fairness" (and thus
   * prevent starvation) at the expense of slightly higher dispatching
   * overhead.
   */
  virtual void max_notify_iterations (int);

  /**
   * Get the maximum number of times that the ACE_Reactor_Impl will
   * iterate and dispatch the <ACE_Event_Handlers> that are passed in
   * via the notify queue before breaking out of its
   * <ACE_Message_Queue::dequeue> loop.
   */
  virtual int max_notify_iterations (void);

  /**
   * Purge any notifications pending in this reactor for the specified
   * <ACE_Event_Handler> object. Returns the number of notifications
   * purged. Returns -1 on error.
   */
  virtual int purge_pending_notifications (ACE_Event_Handler * = 0,
                                           ACE_Reactor_Mask    = ACE_Event_Handler::ALL_EVENTS_MASK);

  /**
   * Return the Event_Handler associated with <handle>.  Return 0 if
   * <handle> is not registered.
   */
  virtual ACE_Event_Handler *find_handler (ACE_HANDLE handle);

  /**
   * Check to see if <handle> is associated with a valid Event_Handler
   * bound to <mask>.  Return the <event_handler> associated with this
   * <handler> if <event_handler> != 0.
   */
  virtual int handler (ACE_HANDLE handle,
                       ACE_Reactor_Mask mask,
                       ACE_Event_Handler **event_handler = 0);

  /**
   * Check to see if <signum> is associated with a valid Event_Handler
   * bound to a signal.  Return the <event_handler> associated with
   * this <handler> if <event_handler> != 0.
   */
  virtual int handler (int signum,
                       ACE_Event_Handler ** = 0);

  /// Returns true if Reactor has been successfully initialized, else
  /// false.
  virtual int initialized (void);

  /// Returns the current size of the Reactor's internal descriptor
  /// table.
  virtual size_t size (void) const;

  /// Returns a reference to the Reactor's internal lock.
  virtual ACE_Lock &lock (void);

  /// Wake up all threads waiting in the event loop.
  virtual void wakeup_all_threads (void);

  /// Transfers ownership of Reactor_Impl to the new_owner.
  /**
   * @note There is no need to set the owner of the event loop for the
   *       ACE_Dev_Poll_Reactor.  Multiple threads may invoke the
   *       event loop simulataneously.  As such, this method is a
   *       no-op.
   */
  virtual int owner (ACE_thread_t new_owner, ACE_thread_t *old_owner = 0);

  /// Return the ID of the "owner" thread.
  /**
   * @note There is no need to set the owner of the event loop for the
   *       ACE_Dev_Poll_Reactor.  Multiple threads may invoke the
   *       event loop simulataneously.  As such, this method is a
   *       no-op.
   */
  virtual int owner (ACE_thread_t *owner);

  /// Get the existing restart value.
  virtual int restart (void);

  /// Set a new value for restart and return the original value.
  /**
   * @param r If zero, then the event loop will not be automatically
   *          restarted if the underlying poll is interrupted via the
   *          INTR (interrupt) signal.
   *
   * @return Returns the previous "restart" value.
   */
  virtual int restart (int r);

  /// Set position of the owner thread.
  /**
   * @note This is currently a no-op.
   */
  virtual void requeue_position (int);

  /// Get position of the owner thread.
  /**
   * @note This is currently a no-op.
   */
  virtual int requeue_position (void);

  /**
   * @name Low-level wait_set mask manipulation methods
   *
   * Low-level methods to manipulate the event/reactor mask associated
   * with a handle and event handler when polling for events.
   * @par
   * The "interest set," i.e. the wait set, can be directly
   * manipulated with these methods.
   */
  //@{

  /// GET/SET/ADD/CLR the dispatch mask "bit" bound with the
  /// event_handler and mask.
  virtual int mask_ops (ACE_Event_Handler *event_handler,
                        ACE_Reactor_Mask mask,
                        int ops);

  /// GET/SET/ADD/CLR the dispatch MASK "bit" bound with the handle
  /// and mask.
  virtual int mask_ops (ACE_HANDLE handle,
                        ACE_Reactor_Mask mask,
                        int ops);

  //@}

  /**
   * @name Low-level ready_set mask manipulation methods
   *
   * These methods are unimplemented.
   */
  //@{

  /// GET/SET/ADD/CLR the ready "bit" bound with the event_handler
  /// and mask.
  virtual int ready_ops (ACE_Event_Handler *event_handler,
                         ACE_Reactor_Mask mask,
                         int ops);

  /// GET/SET/ADD/CLR the ready "bit" bound with the handle and mask.
  virtual int ready_ops (ACE_HANDLE handle,
                         ACE_Reactor_Mask,
                         int ops);

  //@}

  /// Dump the state of an object.
  virtual void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:

  /// Non-locking version of wait_pending().
  /**
   * Returns non-zero if there are I/O events "ready" for dispatching,
   * but does not actually dispatch the event handlers.  By default,
   * don't block while checking this, i.e., "poll".
   *
   * @note It is only possible to achieve millisecond timeout
   *       resolutions with the ACE_Dev_Poll_Reactor.
   */
  int work_pending_i (ACE_Time_Value *max_wait_time);

  /// Poll for events and return the number of event handlers that
  /// were dispatched.
  /**
   * This is a helper method called by all handle_events() methods.
   */
  int handle_events_i (ACE_Time_Value *max_wait_time);

  /// Perform the upcall with the given event handler method.
  int upcall (ACE_Event_Handler *event_handler,
              int (ACE_Event_Handler::*callback)(ACE_HANDLE),
              ACE_HANDLE handle);

  /**
   * Dispatch ACE_Event_Handlers for time events, I/O events, and
   * signal events.  Returns the total number of ACE_Event_Handlers
   * that were dispatched or -1 if something goes wrong.
   */
  int dispatch (void);

  ///
  int dispatch_timer_handlers (int &number_of_handlers_dispatched);

  /// Dispatch all IO related events to their corresponding event
  /// handlers.
  int dispatch_io_events (int &io_handlers_dispatched);

  /// Register the given event handler with the reactor.
  int register_handler_i (ACE_HANDLE handle,
                          ACE_Event_Handler *eh,
                          ACE_Reactor_Mask mask);

  /// Remove the event handler associated with the given handle and
  /// event mask from the "interest set."
  int remove_handler_i (ACE_HANDLE handle, ACE_Reactor_Mask mask);

  /// Temporarily remove the given handle from the "interest set."
  int suspend_handler_i (ACE_HANDLE handle);

  /// Place the given handle that was temporarily removed from the
  /// "interest set," i.e that was suspended, back in to the interest
  /// set.  The given handle will once again be polled for events.
  int resume_handler_i (ACE_HANDLE handle);

  /// GET/SET/ADD/CLR the dispatch MASK "bit" bound with the handle
  /// and mask.  This internal helper method acquires no lock.
  int mask_ops_i (ACE_HANDLE handle,
                  ACE_Reactor_Mask mask,
                  int ops);

  /// Convert a reactor mask to its corresponding poll() event mask.
  short reactor_mask_to_poll_event (ACE_Reactor_Mask mask);

protected:

  /// Has the reactor been initialized.
  char initialized_;

  /// The file descriptor associated with the open `/dev/poll' or
  /// `/dev/epoll' device.
  /**
   * All interactions with the `/dev/poll' or `/dev/epoll' device are
   * done through this file descriptor.
   */
  ACE_HANDLE poll_fd_;

  /// The maximum number of file descriptors over which demultiplexing
  /// will occur.
  size_t size_;

  /// Track HANDLES we are interested in for various events that must
  /// be dispatched *without* polling.
  /// ACE_Dev_Poll_Ready_Set ready_set_;

#if defined (ACE_HAS_EVENT_POLL)
  /// The memory map that `/dev/epoll' will feed its results to.
  char *mmap_;
#else
  /// The pollfd array that `/dev/poll' will feed its results to.
  struct pollfd *dp_fds_;
#endif  /* ACE_HAS_EVENT_POLL */

  /// Pointer to the next pollfd array element that contains the next
  /// event to be dispatched.
  struct pollfd *start_pfds_;

  /// The last element in the pollfd array plus one.
  /**
   * The loop that dispatches IO events stops when this->start_pfds ==
   * this->end_pfds_.
   */
  struct pollfd *end_pfds_;

  /// This flag is used to keep track of whether we are actively handling
  /// events or not.
  sig_atomic_t deactivated_;

  /// Lock used for synchronization of reactor state.
  ACE_SYNCH_MUTEX lock_;

  /// Adapter used to return internal lock to outside world.
  ACE_Lock_Adapter<ACE_SYNCH_MUTEX> lock_adapter_;

  /// The repository that contains all registered event handlers.
  ACE_Dev_Poll_Reactor_Handler_Repository handler_rep_;

  /// Defined as a pointer to allow overriding by derived classes...
  ACE_Timer_Queue *timer_queue_;

  /// Keeps track of whether we should delete the timer queue (if we
  /// didn't create it, then we don't delete it).
  int delete_timer_queue_;

  /// Handle signals without requiring global/static variables.
  ACE_Sig_Handler *signal_handler_;

  /// Keeps track of whether we should delete the signal handler (if we
  /// didn't create it, then we don't delete it).
  int delete_signal_handler_;

  /// Callback object that unblocks the <ACE_Select_Reactor> if it's
  /// sleeping.
  ACE_Reactor_Notify *notify_handler_;

  /// Keeps track of whether we need to delete the notify handler (if
  /// we didn't create it, then we don't delete it).
  int delete_notify_handler_;

  /// Flag that determines if signals are masked during event
  /// dispatching.
  /**
   * If 0 then the Reactor will not mask the signals during the event
   * dispatching.  This is useful for applications that do not
   * register any signal handlers and want to reduce the overhead
   * introduce by the kernel level locks required to change the mask.
   */
  int mask_signals_;

  /// Restart the handle_events event loop method automatically when
  /// polling function in use (ioctl() in this case) is interrupted
  /// via an EINTR signal.
  int restart_;

};


/**
 * @class ACE_Dev_Poll_Handler_Guard
 *
 * @brief Class used to make event handler reference count
 *        manipulation exception-safe.
 *
 * This class makes the reference count manipulation that occurs
 * during an upcall exception-safe.  Prior to dispatching the event
 * handler, the reference count is increased.  Once the upcall for the
 * given event handler is complete, its reference count will be decreased.
 */
class ACE_Dev_Poll_Handler_Guard
{
public:

  /// Constructor
  /**
   * The constructor increments the reference count on the event
   * handler corresponding to the given handle.
   */
  ACE_Dev_Poll_Handler_Guard (
    ACE_Dev_Poll_Reactor_Handler_Repository &repository,
    ACE_HANDLE handle);

  /// Destructor
  /**
   * The destructor decrements the reference count on the event
   * handler corresponding to the given handle.
   */
  ~ACE_Dev_Poll_Handler_Guard (void);

private:

  /// Reference to the handler repository containing the event handler
  /// used during the upcall.
  ACE_Dev_Poll_Reactor_Handler_Repository &repository_;

  /// Handle corresponding to the event being dispatched.
  ACE_HANDLE handle_;

};


#if defined (__ACE_INLINE__)
# include "ace/Dev_Poll_Reactor.inl"
#endif /* __ACE_INLINE__ */

#endif  /* ACE_HAS_EVENT_POLL || ACE_HAS_DEV_POLL */

#include /**/ "ace/post.h"

#endif  /* ACE_DEV_POLL_REACTOR_H */
