/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Reactor.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_REACTOR_H
#define ACE_REACTOR_H
#include "ace/pre.h"

class ACE_Reactor_Impl;

// Need the class def for ACE_Handle_Set to compile references to it in
// programs.
#include "ace/Handle_Set.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Timer Queue is a complicated template class. A simple forward
// declaration will not work
#include "ace/Timer_Queue.h"

// Event_Handler.h contains the definition of ACE_Reactor_Mask
#include "ace/Event_Handler.h"

// We are using 4 or 5 signal classes, we could forward declare
// them.... But Timer_Queue_T.h includes Signal.h, so I don't think
// forward declaration will be useful here
#include "ace/Signal.h"

/**
 * @class ACE_Reactor
 *
 * @brief The responsibility of this class is to forward all methods to
 * its delegation/implementation class, e.g.,
 * <ACE_Select_Reactor> or <ACE_WFMO_Reactor>.
 */
class ACE_Export ACE_Reactor
{
public:
  /// Operations on the "ready" mask and the "dispatch" mask.
  enum
  {
    /// Retrieve current value of the the "ready" mask or the
    /// "dispatch" mask.
    GET_MASK = 1,
    /// Set value of bits to new mask (changes the entire mask).
    SET_MASK = 2,
    /// Bitwise "or" the value into the mask (only changes enabled
    /// bits).
    ADD_MASK = 3,
    /// Bitwise "and" the negation of the value out of the mask (only
    /// changes enabled bits).
    CLR_MASK = 4
  };

  /**
   * You can add a hook to various run_event methods and the hook will
   * be called after handling every reactor event.  If this function
   * returns 0, <run_reactor_event_loop> will check for the return
   * value of <handle_event>.  If it is -1, the
   * <run_reactor_event_loop> will return (pre-maturely.)
   */
  typedef int (*REACTOR_EVENT_HOOK)(ACE_Reactor *);

  /// Get pointer to a process-wide <ACE_Reactor>.
  static ACE_Reactor *instance (void);

  /**
   * Set pointer to a process-wide <ACE_Reactor> and return existing
   * pointer.  If <delete_reactor> != 0 then we'll delete the Reactor
   * at destruction time.
   */
  static ACE_Reactor *instance (ACE_Reactor *,
                                int delete_reactor = 0);

  /// Delete the dynamically allocated Singleton
  static void close_singleton (void);

  // = Singleton reactor event loop management methods.

  // Note that these method ONLY work on the "Singleton Reactor,"
  // i.e., the one returned from <ACE_Reactor::instance>.
  /**
   * Run the event loop until the
   * <ACE_Reactor::handle_events/ACE_Reactor::alertable_handle_events>
   * method returns -1 or the <end_event_loop> method is invoked.
   * Note that this method can only be used by the singleton
   * <ACE_Reactor::instance>.  Thus, to run another reactor use
   * <ACE_Reactor::run_reactor_event_loop>.
   */
  static int run_event_loop (void);
  static int run_alertable_event_loop (void);

  /**
   * Run the event loop until the <ACE_Reactor::handle_events> or
   * <ACE_Reactor::alertable_handle_events> methods returns -1, the
   * <end_event_loop> method is invoked, or the <ACE_Time_Value>
   * expires.  Note that this method can only be used by the singleton
   * <ACE_Reactor::instance>.  Thus, to run another reactor use
   * <ACE_Reactor::run_reactor_event_loop>.
   */
  static int run_event_loop (ACE_Time_Value &tv);
  static int run_alertable_event_loop (ACE_Time_Value &tv);

  /**
   * Instruct the <ACE_Reactor::instance> to terminate its event loop
   * and notifies the <ACE_Reactor::instance> so that it can wake up
   * and close down gracefully.  Note that this method can only be
   * used by the singleton <ACE_Reactor::instance>.  Thus, to
   * terminate another reactor, use
   * <ACE_Reactor::end_reactor_event_loop>.
   */
  static int end_event_loop (void);

  /**
   * Report if the <ACE_Reactor::instance>'s event loop is finished.
   * Note that this method can only be used by the singleton
   * <ACE_Reactor::instance>.  Thus, to check another reactor use
   * <ACE_Reactor::reactor_event_loop_done>.
   */
  static int event_loop_done (void);

  /**
   * Resets the <ACE_Reactor::end_event_loop_> static so that the
   * <run_event_loop> method can be restarted.  Note that this method
   * can only be used by the singleton <ACE_Reactor::instance>.  Thus,
   * to reset another reactor use <ACE_Reactor::reset_reactor_event_loop>.
   */
  static void reset_event_loop (void);

  /**
   * The singleton reactor is used by the <ACE_Service_Config>.
   * Therefore, we must check for the reconfiguration request and
   * handle it after handling an event.
   */
  static int check_reconfiguration (ACE_Reactor *);

  // = Reactor event loop management methods.

  // These methods work with an instance of a reactor.
  /**
   * Run the event loop until the
   * <ACE_Reactor::handle_events/ACE_Reactor::alertable_handle_events>
   * method returns -1 or the <end_reactor_event_loop> method is invoked.
   */
  virtual int run_reactor_event_loop (REACTOR_EVENT_HOOK = 0);
  virtual int run_alertable_reactor_event_loop (REACTOR_EVENT_HOOK = 0);

  /**
   * Run the event loop until the <ACE_Reactor::handle_events> or
   * <ACE_Reactor::alertable_handle_events> methods returns -1, the
   * <end_reactor_event_loop> method is invoked, or the <ACE_Time_Value>
   * expires.
   */
  virtual int run_reactor_event_loop (ACE_Time_Value &tv,
                                      REACTOR_EVENT_HOOK = 0);
  virtual int run_alertable_reactor_event_loop (ACE_Time_Value &tv,
                                                REACTOR_EVENT_HOOK = 0);

  /**
   * Instruct the Reactor to terminate its event loop and notifies the
   * Reactor so that it can wake up and deactivate
   * itself. Deactivating the Reactor would allow the Reactor to be
   * shutdown gracefully. Internally the Reactor calls deactivate ()
   * on the underlying implementation.
   * Any queued notifications remain queued on return from this method.
   * If the event loop is restarted in the future, the notifications
   * will be dispatched then. If the reactor is closed or deleted without
   * further dispatching, the notifications will be lost.
   */
  virtual int end_reactor_event_loop (void);

  /// Indicate if the Reactor's event loop has been ended.
  virtual int reactor_event_loop_done (void);

  /// Resets the <ACE_Reactor::end_event_loop_> static so that the
  /// <run_event_loop> method can be restarted.
  virtual void reset_reactor_event_loop (void);

  /**
   * Create the Reactor using <implementation>.  The flag
   * <delete_implementation> tells the Reactor whether or not to
   * delete the <implementation> on destruction.
   */
  ACE_Reactor (ACE_Reactor_Impl *implementation = 0,
               int delete_implementation = 0);

  /// Close down and release all resources.
  /**
   * Any notifications that remain queued on this reactor instance are
   * lost.
   */
  virtual ~ACE_Reactor (void);

  /**
   * Initialize the <ACE_Reactor> to manage <max_number_of_handles>.
   * If <restart> is non-0 then the <ACE_Reactor>'s <handle_events>
   * method will be restarted automatically when <EINTR> occurs.  If
   * <signal_handler> or <timer_queue> are non-0 they are used as the
   * signal handler and timer queue, respectively.
   */
  virtual int open (size_t max_number_of_handles,
                    int restart = 0,
                    ACE_Sig_Handler *signal_handler = 0,
                    ACE_Timer_Queue *timer_queue = 0);

  /// Use a user specified signal handler instead.
  virtual int set_sig_handler (ACE_Sig_Handler *signal_handler);

  // = The following method is deprecated.  Use <timer_queue> instead.
  /// Set a user specified timer queue.
  virtual int set_timer_queue (ACE_Timer_Queue *tq);

  /// Set a user-specified timer queue.
  /// Return the current <ACE_Timer_Queue>.
  virtual int timer_queue (ACE_Timer_Queue *tq);
  virtual ACE_Timer_Queue *timer_queue (void) const;

  /// Close down and release all resources.
  virtual int close (void);

  // = Event loop drivers.

  /**
   * Returns non-zero if there are I/O events "ready" for dispatching,
   * but does not actually dispatch the event handlers.  By default,
   * don't block while checking this, i.e., "poll".
   */
  virtual int work_pending (const ACE_Time_Value &max_wait_time =  ACE_Time_Value::zero);

  /**
   * This event loop driver blocks for up to <max_wait_time> before
   * returning.  It will return earlier if events occur.  Note that
   * <max_wait_time> can be 0, in which case this method blocks
   * indefinitely until events occur.
   *
   * <max_wait_time> is decremented to reflect how much time this call
   * took.  For instance, if a time value of 3 seconds is passed to
   * handle_events and an event occurs after 2 seconds,
   * <max_wait_time> will equal 1 second.  This can be used if an
   * application wishes to handle events for some fixed amount of
   * time.
   *
   * Returns the total number of timers and I/O <ACE_Event_Handler>s
   * that were dispatched, 0 if the <max_wait_time> elapsed without
   * dispatching any handlers, or -1 if an error occurs.
   *
   * The only difference between <alertable_handle_events> and
   * <handle_events> is that in the alertable case, the eventloop will
   * return when the system queues an I/O completion routine or an
   * Asynchronous Procedure Call.
   */
  virtual int handle_events (ACE_Time_Value *max_wait_time = 0);
  virtual int alertable_handle_events (ACE_Time_Value *max_wait_time = 0);

  /**
   * This method is just like the one above, except the
   * <max_wait_time> value is a reference and can therefore never be
   * NULL.
   *
   * The only difference between <alertable_handle_events> and
   * <handle_events> is that in the alertable case, the eventloop will
   * return when the system queues an I/O completion routine or an
   * Asynchronous Procedure Call.
   */
  virtual int handle_events (ACE_Time_Value &max_wait_time);
  virtual int alertable_handle_events (ACE_Time_Value &max_wait_time);

  // = Register and remove Handlers.

  /// Register <event_handler> with <mask>.  The I/O handle will always
  /// come from <get_handle> on the <event_handler>.
  virtual int register_handler (ACE_Event_Handler *event_handler,
                                ACE_Reactor_Mask mask);

  /// Register <event_handler> with <mask>.  The I/O handle is provided
  /// through the <io_handle> parameter.
  virtual int register_handler (ACE_HANDLE io_handle,
                                ACE_Event_Handler *event_handler,
                                ACE_Reactor_Mask mask);

#if defined (ACE_WIN32)
  /**
   * Register an <event_handler> that will be notified when
   * <event_handle> is signaled.  Since no event mask is passed
   * through this interface, it is assumed that the <event_handle>
   * being passed in is an event handle and not an I/O handle.
   *
   * Originally this interface was available for all platforms, but
   * because ACE_HANDLE is an int on non-Win32 platforms, compilers
   * are not able to tell the difference between
   * register_handler(ACE_Event_Handler*,ACE_Reactor_Mask) and
   * register_handler(ACE_Event_Handler*,ACE_HANDLE). Therefore, we
   * have restricted this method to Win32 only.
   */
  virtual int register_handler (ACE_Event_Handler *event_handler,
                                ACE_HANDLE event_handle = ACE_INVALID_HANDLE);
#endif /* ACE_WIN32 */

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
   * <mask> includes <ACE_Event_Handler::DONT_CALL> then the
   * <handle_close> method of the <event_handler> is not invoked.
   */
  virtual int remove_handler (ACE_Event_Handler *event_handler,
                              ACE_Reactor_Mask mask);

  /**
   * Removes the <ACE_Event_Handler> associated with <handle>.  If
   * <mask> includes <ACE_Event_Handler::DONT_CALL> then the
   * <handle_close> method of the associated <event_handler> is not
   * invoked.
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

  /// Suspend <event_handler> temporarily.  Use
  /// <ACE_Event_Handler::get_handle> to get the handle.
  virtual int suspend_handler (ACE_Event_Handler *event_handler);

  /// Suspend <handle> temporarily.
  virtual int suspend_handler (ACE_HANDLE handle);

  /// Suspend all <handles> in handle set temporarily.
  virtual int suspend_handler (const ACE_Handle_Set &handles);

  /// Suspend all <handles> temporarily.
  virtual int suspend_handlers (void);

  /// Resume <event_handler>. Use <ACE_Event_Handler::get_handle> to
  /// get the handle.
  virtual int resume_handler (ACE_Event_Handler *event_handler);

  /// Resume <handle>.
  virtual int resume_handler (ACE_HANDLE handle);

  /// Resume all <handles> in handle set.
  virtual int resume_handler (const ACE_Handle_Set &handles);

  /// Resume all <handles>.
  virtual int resume_handlers (void);

  /// Does the reactor allow the application to resume the handle on
  /// its own ie. can it pass on the control of handle resumption to
  /// the application. A positive value indicates that the handlers
  /// are application resumable. A value of 0 indicates otherwise.
  virtual int resumable_handler (void);

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
   * @param arg  argument passed to the handle_timeout() method of  event_handler
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

  /// Cancel all <Event_Handler>s that match the address of
  /// <event_handler>.  Returns number of handlers cancelled.
  virtual int cancel_timer (ACE_Event_Handler *event_handler,
                            int dont_call_handle_close = 1);

  /**
   * Cancel the single <Event_Handler> that matches the <timer_id>
   * value, which was returned from the schedule method.  If arg is
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
  /// Note that this call does not cause the Reactor to re-examine
  /// its set of handlers - the new masks will be noticed the next
  /// time the Reactor waits for activity. If there is no other
  /// activity expected, or you need immediate re-examination of
  /// the wait masks, either call ACE_Reactor::notify after this
  /// call, or use ACE_Reactor::register_handler instead.
  virtual int schedule_wakeup (ACE_Event_Handler *event_handler,
                               ACE_Reactor_Mask masks_to_be_added);

  /// Add <masks_to_be_added> to the <handle>'s entry.  <event_handler>
  /// associated with <handle> must already have been registered.
  /// Note that this call does not cause the Reactor to re-examine
  /// its set of handlers - the new masks will be noticed the next
  /// time the Reactor waits for activity. If there is no other
  /// activity expected, or you need immediate re-examination of
  /// the wait masks, either call ACE_Reactor::notify after this
  /// call, or use ACE_Reactor::register_handler instead.
  virtual int schedule_wakeup (ACE_HANDLE handle,
                               ACE_Reactor_Mask masks_to_be_added);

  /// Clear <masks_to_be_cleared> from the <event_handler>'s entry.
  /// Note that this call does not cause the Reactor to re-examine
  /// its set of handlers - the new masks will be noticed the next
  /// time the Reactor waits for activity. If there is no other
  /// activity expected, or you need immediate re-examination of
  /// the wait masks, either call ACE_Reactor::notify after this
  /// call, or use ACE_Reactor::register_handler instead.
  virtual int cancel_wakeup (ACE_Event_Handler *event_handler,
                             ACE_Reactor_Mask masks_to_be_cleared);

  /// Clear <masks_to_be_cleared> from the <handle>'s entry.
  /// Note that this call does not cause the Reactor to re-examine
  /// its set of handlers - the new masks will be noticed the next
  /// time the Reactor waits for activity. If there is no other
  /// activity expected, or you need immediate re-examination of
  /// the wait masks, either call ACE_Reactor::notify after this
  /// call, or use ACE_Reactor::register_handler instead.
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
                      ACE_Time_Value *tv = 0);

  /**
   * Set the maximum number of times that ACE_Reactor will
   * iterate and dispatch the <ACE_Event_Handlers> that are passed in
   * via the notify queue before breaking out of its
   * <ACE_Message_Queue::dequeue> loop.  By default, this is set to
   * -1, which means "iterate until the queue is empty."  Setting this
   * to a value like "1 or 2" will increase "fairness" (and thus
   * prevent starvation) at the expense of slightly higher dispatching
   * overhead.
   */
  virtual void max_notify_iterations (int iterations);

  /**
   * Get the maximum number of times that the ACE_Reactor will
   * iterate and dispatch the <ACE_Event_Handlers> that are passed in
   * via the notify queue before breaking out of its
   * <ACE_Message_Queue::dequeue> loop.
   */
  virtual int max_notify_iterations (void);

  /**
   * Purge any notifications pending in this reactor for the specified
   * <ACE_Event_Handler> object. If <eh> == 0, all notifications for all
   * handlers are removed (but not any notifications posted just to wake up
   * the reactor itself). Returns the number of notifications purged.
   * Returns -1 on error.
   */
  virtual int purge_pending_notifications (ACE_Event_Handler *,
                                           ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);

  // = Assorted helper methods.

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
                       ACE_Event_Handler **event_handler = 0);

  /// Returns true if Reactor has been successfully initialized, else
  /// false.
  virtual int initialized (void);

  /// Returns the current size of the Reactor's internal descriptor
  /// table.
  virtual size_t size (void) const;

  /// Returns a reference to the Reactor's internal lock.
  virtual ACE_Lock &lock (void);

  /// Wake up all threads in waiting in the event loop
  virtual void wakeup_all_threads (void);

  /// Transfers ownership of Reactor to the <new_owner>.
  virtual int owner (ACE_thread_t new_owner,
                     ACE_thread_t *old_owner = 0);

  /// Return the ID of the "owner" thread.
  virtual int owner (ACE_thread_t *owner);

  /// Set position of the owner thread.
  virtual void requeue_position (int position);

  /// Get position of the owner thread.
  virtual int requeue_position (void);

  /// Get the existing restart value.
  virtual int restart (void);

  /// Set a new value for restart and return the original value.
  virtual int restart (int r);

  // = Low-level wait_set mask manipulation methods.

  /// GET/SET/ADD/CLR the dispatch mask "bit" bound with the
  /// <event_handler> and <mask>.
  virtual int mask_ops (ACE_Event_Handler *event_handler,
                        ACE_Reactor_Mask mask,
                        int ops);

  /// GET/SET/ADD/CLR the dispatch MASK "bit" bound with the <handle>
  /// and <mask>.
  virtual int mask_ops (ACE_HANDLE handle,
                        ACE_Reactor_Mask mask,
                        int ops);

  // = Low-level ready_set mask manipulation methods.
  /// GET/SET/ADD/CLR the ready "bit" bound with the <event_handler>
  /// and <mask>.
  virtual int ready_ops (ACE_Event_Handler *event_handler,
                         ACE_Reactor_Mask mask,
                         int ops);

  /// GET/SET/ADD/CLR the ready "bit" bound with the <handle> and <mask>.
  virtual int ready_ops (ACE_HANDLE handle,
                         ACE_Reactor_Mask mask,
                         int ops);

  /// Get the implementation class
  virtual ACE_Reactor_Impl *implementation (void) const;

  /**
   * Returns 0, if the size of the current message has been put in
   * <size> returns -1, if not.  ACE_HANDLE allows the reactor to
   * check if the caller is valid.  Used for CLASSIX Reactor
   * implementation.
   */
  virtual int current_info (ACE_HANDLE handle,
                            size_t &msg_size);

  /// Return 1 if we any event associations were made by the reactor
  /// for the handles that it waits on, 0 otherwise.
  virtual int uses_event_associations (void);

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

  /// Dump the state of the object.
  void dump (void) const;

protected:
  /// Set the implementation class.
  virtual void implementation (ACE_Reactor_Impl *implementation);

  /// Delegation/implementation class that all methods will be
  /// forwarded to.
  ACE_Reactor_Impl *implementation_;

  /// Flag used to indicate whether we are responsible for cleaning up
  /// the implementation instance
  int delete_implementation_;

  /// Pointer to a process-wide <ACE_Reactor> singleton.
  static ACE_Reactor *reactor_;

  /// Must delete the <reactor_> singleton if non-0.
  static int delete_reactor_;

  /// Deny access since member-wise won't work...
  ACE_Reactor (const ACE_Reactor &);
  ACE_Reactor &operator = (const ACE_Reactor &);
};

#if defined (__ACE_INLINE__)
#include "ace/Reactor.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_REACTOR_H */
