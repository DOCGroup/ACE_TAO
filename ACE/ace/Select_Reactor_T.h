// -*- C++ -*-

//=============================================================================
/**
 *  @file    Select_Reactor_T.h
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */
//=============================================================================

#ifndef ACE_SELECT_REACTOR_T_H
#define ACE_SELECT_REACTOR_T_H
#include /**/ "ace/pre.h"

#include "ace/Select_Reactor_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Lock_Adapter_T.h"
#include "ace/Token.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
typedef ACE_Token ACE_SELECT_TOKEN;
#else
typedef ACE_Noop_Token ACE_SELECT_TOKEN;
#endif /* ACE_MT_SAFE && ACE_MT_SAFE != 0 */

/**
 * @class ACE_Select_Reactor_T
 *
 * @brief An object oriented event demultiplexor and event handler
 * dispatcher.
 *
 * The ACE_Select_Reactor is an object-oriented event
 * demultiplexor and event handler dispatcher.  The sources of
 * events that the ACE_Select_Reactor waits for and dispatches
 * includes I/O events, signals, and timer events.  All public
 * methods acquire the main ACE_Reactor_Token lock and
 * call down to private or protected methods, which assume that
 * the lock is held and so therefore don't (re)acquire the lock.
 */
template <class ACE_SELECT_REACTOR_TOKEN>
class ACE_Select_Reactor_T : public ACE_Select_Reactor_Impl
{
public:
  /// If @a disable_notify_pipe is non-0 then the reactor will
  /// not create a notification pipe, which will save two I/O handles
  /// but will elide the notify() feature.  If @a mask_signals is
  /// true the reactor is "signal-safe" when dispatching handlers to
  /// signal events, whereas if @a mask_signals is false the reactor will
  /// be more efficient, but not signal-safe (which may be perfectly
  /// fine if your application doesn't use the reactor to handle signals).
  ACE_Select_Reactor_T (ACE_Sig_Handler * = 0,
                        ACE_Timer_Queue * = 0,
                        int disable_notify_pipe = ACE_DISABLE_NOTIFY_PIPE_DEFAULT,
                        ACE_Reactor_Notify *notify = 0,
                        bool mask_signals = true,
                        int s_queue = ACE_SELECT_TOKEN::FIFO);

  /// Initialize @c ACE_Select_Reactor with size @arg size.
  /// If @arg disable_notify_pipe is non-0 then the reactor will
  /// not create a notification pipe, which will save two I/O handles
  /// but will elide the notification feature.  If @arg mask_signals is
  /// true the reactor is "signal-safe" when dispatching handlers to
  /// signal events, whereas if @arg mask_signals is false the reactor will
  /// be more efficient, but not signal-safe (which may be perfectly
  /// fine if your application doesn't use the reactor to handle signals).
  /**
   * @note On Unix platforms, the size parameter should be as large as
   *       the maximum number of file descriptors allowed for a given
   *       process.  This is necessary since a file descriptor is used
   *       to directly index the array of event handlers maintained by
   *       the Reactor's handler repository.  Direct indexing is used
   *       for efficiency reasons.
   */
  ACE_Select_Reactor_T (size_t size,
                        bool restart = false,
                        ACE_Sig_Handler * = 0,
                        ACE_Timer_Queue * = 0,
                        int disable_notify_pipe = ACE_DISABLE_NOTIFY_PIPE_DEFAULT,
                        ACE_Reactor_Notify *notify = 0,
                        bool mask_signals = true,
                        int s_queue = ACE_SELECT_TOKEN::FIFO);

  /**
   * Initialize the @c ACE_Select_Reactor to manage
   * @arg max_number_of_handles.  If @arg restart is non-0 then the
   * @c ACE_Reactor's @c handle_events method will be restarted
   * automatically when @c EINTR occurs.  If @arg signal_handler or
   * @arg timer_queue are non-0 they are used as the signal handler and
   * timer queue, respectively.  If @arg disable_notify_pipe is non-0 the
   * notification pipe is not created, thereby saving two I/O handles.
   *
   * @note On Unix platforms, the @a maximum_number_of_handles parameter
   *       should be as large as the maximum number of file
   *       descriptors allowed for a given process.  This is necessary
   *       since a file descriptor is used to directly index the array
   *       of event handlers maintained by the Reactor's handler
   *       repository.  Direct indexing is used for efficiency
   *       reasons.
   */
  int open (size_t max_number_of_handles = DEFAULT_SIZE,
                    bool restart = false,
                    ACE_Sig_Handler * = 0,
                    ACE_Timer_Queue * = 0,
                    int disable_notify_pipe = ACE_DISABLE_NOTIFY_PIPE_DEFAULT,
                    ACE_Reactor_Notify * = 0) override;

  /// Returns -1 (not used in this implementation);
  int current_info (ACE_HANDLE, size_t &size) override;

  /// Use a user specified signal handler instead.
  int set_sig_handler (ACE_Sig_Handler *signal_handler) override;

  /// Set a user-specified timer queue.
  int timer_queue (ACE_Timer_Queue *tq) override;

  /// Return the current ACE_Timer_Queue.
  ACE_Timer_Queue *timer_queue () const override;

  /// Close down the select_reactor and release all of its resources.
  int close (void) override;

  /// Close down the select_reactor and release all of its resources.
  ~ACE_Select_Reactor_T (void) override;

  // = Event loop drivers.

  /**
   * Returns non-zero if there are I/O events "ready" for dispatching,
   * but does not actually dispatch the event handlers.  By default,
   * don't block while checking this, i.e., "poll".
   */
  int work_pending (const ACE_Time_Value &max_wait_time = ACE_Time_Value::zero) override;

  //@{
  /**
   * This event loop driver that blocks for @a max_wait_time before
   * returning.  It will return earlier if timer events, I/O events,
   * or signal events occur.  Note that @a max_wait_time can be 0, in
   * which case this method blocks indefinitely until events occur.
   *
   * @a max_wait_time is decremented to reflect how much time this call
   * took.  For instance, if a time value of 3 seconds is passed to
   * handle_events and an event occurs after 2 seconds,
   * @a max_wait_time will equal 1 second.  This can be used if an
   * application wishes to handle events for some fixed amount of
   * time.
   *
   * Returns the total number of I/O and Timer ACE_Event_Handler's
   * that were dispatched, 0 if the @a max_wait_time elapsed without
   * dispatching any handlers, or -1 if something goes wrong.
   *
   * Current alertable_handle_events() is identical to
   * handle_events().
   */
  int handle_events (ACE_Time_Value *max_wait_time = 0) override;
  int alertable_handle_events (ACE_Time_Value *max_wait_time = 0) override;
  //@}

  //@{
  /**
   * This method is just like the one above, except the
   * @a max_wait_time value is a reference and can therefore never be
   * NULL.
   *
   * Current <alertable_handle_events> is identical to
   * <handle_events>.
   */
  int handle_events (ACE_Time_Value &max_wait_time) override;
  int alertable_handle_events (ACE_Time_Value &max_wait_time) override;
  //@}

  // = Event handling control.

  /**
   * Return the status of Reactor.  If this function returns 0, the reactor is
   * actively handling events.  If it returns non-zero, <handling_events> and
   * <handle_alertable_events> return -1 immediately.
   */
  int deactivated (void) override;

  /**
   * Control whether the Reactor will handle any more incoming events or not.
   * If @a do_stop == 1, the Reactor will be disabled.  By default, a reactor
   * is in active state and can be deactivated/reactived as wish.
   */
  void deactivate (int do_stop) override;

  // = Register and remove ACE_Event_Handlers.
  /**
   * Register a @a eh with a particular @a mask.  Note that the
   * <Select_Reactor> will call ACE_Event_Handler::get_handle() to
   * extract the underlying I/O handle.
   */
  int register_handler (ACE_Event_Handler *eh,
                                ACE_Reactor_Mask mask) override;

  /**
   * Register a @a eh with a particular @a mask.  Note that since the
   * @a handle is given the Select_Reactor will *not* call
   * ACE_Event_Handler::get_handle() to extract the underlying I/O
   * handle.
   */
  int register_handler (ACE_HANDLE handle,
                                ACE_Event_Handler *eh,
                                ACE_Reactor_Mask mask) override;

#if defined (ACE_WIN32)
  // Originally this interface was available for all platforms, but
  // because ACE_HANDLE is an int on non-Win32 platforms, compilers
  // are not able to tell the difference between
  // register_handler(ACE_Event_Handler*,ACE_Reactor_Mask) and
  // register_handler(ACE_Event_Handler*,ACE_HANDLE). Therefore, we
  // have restricted this method to Win32 only.

  /// Not implemented.
  virtual int register_handler (ACE_Event_Handler *event_handler,
                                ACE_HANDLE event_handle = ACE_INVALID_HANDLE);
#endif /* ACE_WIN32 */

  /// Not implemented.
  int register_handler (ACE_HANDLE event_handle,
                                ACE_HANDLE io_handle,
                                ACE_Event_Handler *event_handler,
                                ACE_Reactor_Mask mask) override;

  /// Register @a eh with all the @a handles in the <Handle_Set>.
  int register_handler (const ACE_Handle_Set &handles,
                                ACE_Event_Handler *eh,
                                ACE_Reactor_Mask mask) override;

  /**
   * Register @a new_sh to handle the signal @a signum using the
   * @a new_disp. Returns the @a old_sh that was previously registered
   * (if any), along with the @a old_disp of the signal handler.
   */
  int register_handler (int signum,
                                ACE_Event_Handler *new_sh,
                                ACE_Sig_Action *new_disp = 0,
                                ACE_Event_Handler **old_sh = 0,
                                ACE_Sig_Action *old_disp = 0) override;

  /// Registers @a new_sh to handle a set of signals @a sigset using the
  /// @a new_disp.
  int register_handler (const ACE_Sig_Set &sigset,
                                ACE_Event_Handler *new_sh,
                                ACE_Sig_Action *new_disp = 0) override;

  /**
   * Removes the @a mask binding of @a eh from the Select_Reactor.  If
   * there are no more bindings for this @a eh then it is removed from
   * the Select_Reactor.  Note that the Select_Reactor will call
   * <ACE_Event_Handler::get_handle> to extract the underlying I/O
   * handle.
   */
  int remove_handler (ACE_Event_Handler *eh,
                              ACE_Reactor_Mask mask) override;

  /**
   * Removes the @a mask bind of Event_Handler whose handle is
   * @a handle from the Select_Reactor.  If there are no more bindings
   * for this @a eh then it is removed from the Select_Reactor.
   */
  int remove_handler (ACE_HANDLE handle,
                              ACE_Reactor_Mask) override;

  /**
   * Removes all the @a mask bindings for handles in the @a handle_set
   * bind of Event_Handler.  If there are no more bindings for any
   * of these handlers then they are removed from the Select_Reactor.
   */
  int remove_handler (const ACE_Handle_Set &handle_set,
                              ACE_Reactor_Mask) override;

  /**
   * Remove the ACE_Event_Handler currently associated with @a signum.
   * @a sigkey is ignored in this implementation since there is only
   * one instance of a signal handler.  Install the new disposition
   * (if given) and return the previous disposition (if desired by the
   * caller).  Returns 0 on success and -1 if @a signum is invalid.
   */
  int remove_handler (int signum,
                              ACE_Sig_Action *new_disp,
                              ACE_Sig_Action *old_disp = 0,
                              int sigkey = -1) override;

  /// Calls <remove_handler> for every signal in @a sigset.
  int remove_handler (const ACE_Sig_Set &sigset) override;

  // = Suspend and resume Handlers.

  /// Temporarily suspend the <Event_Handler> associated with @a eh.
  int suspend_handler (ACE_Event_Handler *eh) override;

  /// Temporarily suspend the Event_Handler associated with @a handle.
  int suspend_handler (ACE_HANDLE handle) override;

  /// Suspend all @a handles in handle set temporarily.
  int suspend_handler (const ACE_Handle_Set &handles) override;

  /// Suspend all the <Event_Handlers> in the Select_Reactor.
  int suspend_handlers (void) override;

  /// Resume a temporarily suspend Event_Handler associated with
  /// @a eh.
  int resume_handler (ACE_Event_Handler *eh) override;

  /// Resume a temporarily suspended Event_Handler associated with
  /// @a handle.
  int resume_handler (ACE_HANDLE handle) override;

  /// Resume all @a handles in handle set.
  int resume_handler (const ACE_Handle_Set &handles) override;

  /// Resume all the <Event_Handlers> in the Select_Reactor.
  int resume_handlers (void) override;

  /**
   * Return true if we any event associations were made by the reactor
   * for the handles that it waits on, false otherwise. Since the
   * Select_Reactor does not do any event associations, this function
   * always return false.
   */
  bool uses_event_associations (void) override;

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
   * @param event_handler  Event handler to schedule on reactor
   * @param arg Argument passed to the handle_timeout() method of event_handler
   * @param delay Time interval after which the timer will expire
   * @param interval Time interval after which the timer will be automatically
   * rescheduled
   * @return -1 on failure, a timer_id value on success
   */
  long schedule_timer (ACE_Event_Handler * event_handler,
                               const void *arg,
                               const ACE_Time_Value &delay,
                               const ACE_Time_Value &interval = ACE_Time_Value::zero) override;

  /**
   * Resets the interval of the timer represented by @a timer_id to
   * @a interval, which is specified in relative time to the current
   * <gettimeofday>.  If @a interval is equal to
   * ACE_Time_Value::zero, the timer will become a non-rescheduling
   * timer.  Returns 0 if successful, -1 if not.
   */
  int reset_timer_interval (long timer_id,
                                    const ACE_Time_Value &interval) override;

  /**
   * Cancel all <event_handlers> that match the address of
   * @a event_handler.  If @a dont_call_handle_close is 0 then the
   * <handle_close> method of @a event_handler will be invoked.
   * Returns number of handler's cancelled.
   */
  int cancel_timer (ACE_Event_Handler *event_handler,
                            int dont_call_handle_close = 1) override;

  /**
   * Cancel the single ACE_Event_Handler that matches the @a timer_id
   * value (which was returned from the <schedule> method).  If arg is
   * non-NULL then it will be set to point to the ``magic cookie''
   * argument passed in when the Event_Handler was registered.  This
   * makes it possible to free up the memory and avoid memory leaks.
   * If @a dont_call_handle_close is 0 then the <handle_close> method
   * of <event_handler> will be invoked.  Returns 1 if cancellation
   * succeeded and 0 if the @a timer_id wasn't found.
   */
  int cancel_timer (long timer_id,
                            const void **arg = 0,
                            int dont_call_handle_close = 1) override;

  // = High-level Event_Handler scheduling operations

  /// ADD the dispatch MASK "bit" bound with the @a eh and the @a mask.
  int schedule_wakeup (ACE_Event_Handler *eh,
                               ACE_Reactor_Mask mask) override;

  /// ADD the dispatch MASK "bit" bound with the @a handle and the @a mask.
  int schedule_wakeup (ACE_HANDLE handle,
                               ACE_Reactor_Mask mask) override;

  /// CLR the dispatch MASK "bit" bound with the @a eh and the @a mask.
  int cancel_wakeup (ACE_Event_Handler *eh,
                             ACE_Reactor_Mask mask) override;

  /// CLR the dispatch MASK "bit" bound with the @a handle and the @a mask.
  int cancel_wakeup (ACE_HANDLE handle,
                             ACE_Reactor_Mask mask) override;

  // = Notification methods.
  /**
   * Called by a thread when it wants to unblock the Select_Reactor.
   * This wakeups the <ACE_Select_Reactor> if currently blocked in
   * <select>/<poll>.  Pass over both the Event_Handler *and* the
   * @a mask to allow the caller to dictate which <Event_Handler>
   * method the <Select_Reactor> will invoke.  The ACE_Time_Value
   * indicates how long to blocking trying to notify the
   * <Select_Reactor>.  If @a timeout == 0, the caller will block until
   * action is possible, else will wait until the relative time
   * specified in *@a timeout elapses).
   */
  int notify (ACE_Event_Handler * = 0,
                      ACE_Reactor_Mask = ACE_Event_Handler::EXCEPT_MASK,
                      ACE_Time_Value * = 0) override;

  /**
   * Set the maximum number of times that the
   * ACE_Select_Reactor_Notify::handle_input() method will iterate and
   * dispatch the ACE_Event_Handlers that are passed in via the
   * notify pipe before breaking out of its recv loop.  By default,
   * this is set to -1, which means "iterate until the pipe is empty."
   * Setting this to a value like "1 or 2" will increase "fairness"
   * (and thus prevent starvation) at the expense of slightly higher
   * dispatching overhead.
   */
  void max_notify_iterations (int) override;

  /**
   * Get the maximum number of times that the
   * ACE_Select_Reactor_Notify::handle_input() method will iterate and
   * dispatch the ACE_Event_Handlers that are passed in via the
   * notify pipe before breaking out of its recv loop.
   */
  int max_notify_iterations (void) override;

  /// Get the existing restart value.
  bool restart (void) override;

  /// Set a new value for restart and return the original value.
  bool restart (bool r) override;

  /// Set position that the main ACE_Select_Reactor thread is requeued in the
  /// list of waiters during a <notify> callback.
  void requeue_position (int) override;

  /// Get position that the main ACE_Select_Reactor thread is requeued in the
  /// list of waiters during a <notify> callback.
  int requeue_position (void) override;

  // = Low-level wait_set mask manipulation methods.
  /// GET/SET/ADD/CLR the dispatch mask "bit" bound with the @a eh and
  /// @a mask.
  int mask_ops (ACE_Event_Handler *eh,
                        ACE_Reactor_Mask mask,
                        int ops) override;

  /// GET/SET/ADD/CLR the dispatch MASK "bit" bound with the @a handle
  /// and @a mask.
  int mask_ops (ACE_HANDLE handle,
                        ACE_Reactor_Mask mask,
                        int ops) override;

  // = Low-level ready_set mask manipulation methods.
  /// GET/SET/ADD/CLR the ready "bit" bound with the @a eh and @a mask.
  int ready_ops (ACE_Event_Handler *eh,
                         ACE_Reactor_Mask mask,
                         int ops) override;

  /// GET/SET/ADD/CLR the ready "bit" bound with the @a handle and @a mask.
  int ready_ops (ACE_HANDLE handle,
                         ACE_Reactor_Mask,
                         int ops) override;

  /// Wake up all threads in waiting in the event loop
  void wakeup_all_threads (void) override;

  // = Only the owner thread can perform a <handle_events>.

  /// Set the new owner of the thread and return the old owner.
  int owner (ACE_thread_t n_id, ACE_thread_t *o_id = 0) override;

  /// Return the current owner of the thread.
  int owner (ACE_thread_t *) override;

  // = Miscellaneous Handler operations.

  /**
   * Return the Event_Handler associated with @a handle.  Return 0 if
   * @a handle is not registered.
   */
  ACE_Event_Handler *find_handler (ACE_HANDLE handle) override;

  /**
   * Check to see if @a handle is associated with a valid Event_Handler
   * bound to @a mask.  Return the @a eh associated with this @a handler
   * if @a eh != 0.
   */
  int handler (ACE_HANDLE handle,
                       ACE_Reactor_Mask mask,
                       ACE_Event_Handler **eh = 0) override;

  /**
   * Check to see if @a signum is associated with a valid Event_Handler
   * bound to a signal.  Return the @a eh associated with this
   * handler if @a eh != 0.
   */
  int handler (int signum,
                       ACE_Event_Handler ** = 0) override;

  /// Returns true if we've been successfully initialized, else false.
  bool initialized (void) override;

  /// Returns the current size of the Reactor's internal descriptor
  /// table.
  size_t size () const override;

  /**
   * Returns a reference to the ACE_Reactor_Token that is
   * used to serialize the internal processing logic.
   * This can be useful for situations where you need to avoid
   * deadlock efficiently when ACE_Event_Handlers are used in
   * multiple threads.
   */
  ACE_Lock &lock (void) override;

  /// Dump the state of an object.
  void dump () const override;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  // = Internal methods that do the actual work.

  // All of these methods assume that the token
  // lock is held by the public methods that call down to them.

  /// Do the work of actually binding the @a handle and @a eh with the
  /// @a mask.
  virtual int register_handler_i (ACE_HANDLE handle,
                                  ACE_Event_Handler *eh,
                                  ACE_Reactor_Mask mask);

  /// Register a set of @a handles.
  virtual int register_handler_i (const ACE_Handle_Set &handles,
                                  ACE_Event_Handler *handler,
                                  ACE_Reactor_Mask mask);

  /// Do the work of actually unbinding the @a handle and @a eh with the
  /// @a mask.
  virtual int remove_handler_i (ACE_HANDLE handle,
                                ACE_Reactor_Mask);

  /// Remove a set of @a handles.
  virtual int remove_handler_i (const ACE_Handle_Set &handles,
                                ACE_Reactor_Mask);

  /// Suspend the <Event_Handler> associated with @a handle
  virtual int suspend_i (ACE_HANDLE handle);

  /// Check to see if the <Event_Handler> associated with @a handle is
  /// suspended. Returns 0 if not, 1 if so.
  int is_suspended_i (ACE_HANDLE handle) override;

  /// Resume the <Event_Handler> associated with @a handle
  virtual int resume_i (ACE_HANDLE handle);

  /// Implement the public handler method.
  virtual ACE_Event_Handler *find_handler_i (ACE_HANDLE handle);

  /// Implement the public handler method.
  virtual int handler_i (ACE_HANDLE handle,
                         ACE_Reactor_Mask,
                         ACE_Event_Handler ** = 0);

  /// Implement the public handler method.
  virtual int handler_i (int signum, ACE_Event_Handler ** = 0);

  /**
   * Check if there are any HANDLEs enabled in the <ready_set_>, and
   * if so, update the @a handle_set and return the number ready.  If
   * there aren't any HANDLEs enabled return 0.
   */
  virtual int any_ready (ACE_Select_Reactor_Handle_Set &handle_set);

  /// Implement the <any_ready> method, assuming that the Sig_Guard is
  /// being held
  virtual int any_ready_i (ACE_Select_Reactor_Handle_Set &handle_set);

  /// Take corrective action when errors occur.
  virtual int handle_error ();

  /// Make sure the handles are all valid.
  virtual int check_handles ();

  /// Wait for events to occur.
  virtual int wait_for_multiple_events (ACE_Select_Reactor_Handle_Set &,
                                        ACE_Time_Value *);

  // = Dispatching methods.

  /**
   * Template Method that dispatches ACE_Event_Handlers for time
   * events, I/O events, and signal events.  Returns the total number
   * of ACE_Event_Handlers that were dispatched or -1 if something
   * goes wrong.
   */
  virtual int dispatch (int nfound,
                        ACE_Select_Reactor_Handle_Set &);

  /**
   * Dispatch all timer handlers that have expired.  Returns -1 if the
   * state of the <wait_set_> has changed, else 0.
   * <number_dispatched> is set to the number of timer handlers
   * dispatched.
   */
  virtual int dispatch_timer_handlers (int &number_dispatched);

  /**
   * Dispatch any notification handlers.  Returns -1 if the state of
   * the <wait_set_> has changed, else returns number of handlers
   * notified.
   */
  virtual int dispatch_notification_handlers (ACE_Select_Reactor_Handle_Set &dispatch_set,
                                              int &number_of_active_handles,
                                              int &number_of_handlers_dispatched);

  /**
   * Dispatch all the input/output/except handlers that are enabled in
   * the @a dispatch_set.  Updates @a number_of_active_handles and
   * @a number_of_handlers_dispatched according to the behavior of the
   * number Returns -1 if the state of the <wait_set_> has changed,
   * else 0.
   */
  virtual int dispatch_io_handlers (ACE_Select_Reactor_Handle_Set &dispatch_set,
                                    int &number_of_active_handles,
                                    int &number_of_handlers_dispatched);

  /**
   * Factors the dispatching of an io handle set (each WRITE, EXCEPT
   * or READ set of handles).  It updates the
   * @a number_of_handlers_dispatched and invokes this->notify_handle
   * for all the handles in <dispatch_set> using the @a mask,
   * <ready_set> and @a callback parameters.  Must return -1 if
   * this->state_changed otherwise it must return 0.
   */
  virtual int dispatch_io_set (int number_of_active_handles,
                               int &number_of_handlers_dispatched,
                               int mask,
                               ACE_Handle_Set& dispatch_mask,
                               ACE_Handle_Set& ready_mask,
                               ACE_EH_PTMF callback);

  /// Notify the appropriate @a callback in the context of the @a eh
  /// associated with @a handle that a particular event has occurred.
  virtual void notify_handle (ACE_HANDLE handle,
                              ACE_Reactor_Mask mask,
                              ACE_Handle_Set &,
                              ACE_Event_Handler *eh,
                              ACE_EH_PTMF callback);

  /// Enqueue ourselves into the list of waiting threads at the
  /// appropriate point specified by <requeue_position_>.
  void renew (void) override;

  /// Synchronization token for the MT_SAFE ACE_Select_Reactor.
  ACE_SELECT_REACTOR_TOKEN token_;

  /// Adapter used to return internal lock to outside world.
  ACE_Lock_Adapter<ACE_SELECT_REACTOR_TOKEN> lock_adapter_;

  /// Release the token lock when a Win32 structured exception occurs.
  int release_token ();

  /// Stops the VC++ compiler from bitching about exceptions and destructors
  int handle_events_i (ACE_Time_Value *max_wait_time = 0);

  /// This flag is used to keep track of whether we are actively handling
  /// events or not.
  sig_atomic_t deactivated_;

private:
  ACE_Select_Reactor_T (const ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN> &) = delete;
  ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN> &operator=  (const ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN> &) = delete;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/Select_Reactor_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Select_Reactor_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Select_Reactor_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* ACE_SELECT_REACTOR_T_H */
