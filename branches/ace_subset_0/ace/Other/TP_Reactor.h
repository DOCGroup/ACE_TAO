/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    TP_Reactor.h
 *
 *  $Id$
 *
 *  The <ACE_TP_Reactor> (aka, Thread Pool Reactor) uses the
 *  Leader/Followers pattern to demultiplex events among a pool of
 *  threads.  When using a thread pool reactor, an application
 *  pre-spawns a _fixed_ number of threads.  When these threads
 *  invoke the <ACE_TP_Reactor>'s <handle_events> method, one thread
 *  will become the leader and wait for an event.  The other
 *  follower threads will queue up waiting for their turn to become
 *  the leader.  When an event occurs, the leader will pick a
 *  follower to become the leader and go on to handle the event.
 *  The consequence of using <ACE_TP_Reactor> is the amortization of
 *  the costs used to creating threads.  The context switching cost
 *  will also reduce.  More over, the total resources used by
 *  threads are bounded because there are a fixed number of threads.
 *
 *
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_TP_REACTOR_H
#define ACE_TP_REACTOR_H
#include "ace/pre.h"

#include "ace/Demux/Select_Reactor.h"

#ifdef ACE_SUBSET_0
#include "ace/Logging/Log_Msg.h"
#endif

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_EH_Dispatch_Info
 *
 * @brief This structure contains information of the activated event
 * handler.
 */
class ACE_Export ACE_EH_Dispatch_Info
{
public:

  ACE_EH_Dispatch_Info (void);

  void set (ACE_HANDLE handle,
            ACE_Event_Handler *event_handler,
            ACE_Reactor_Mask mask,
            ACE_EH_PTMF callback);

  void reset (void);

  int dispatch (void) const;

  ACE_HANDLE handle_;
  ACE_Event_Handler *event_handler_;
  ACE_Reactor_Mask mask_;
  ACE_EH_PTMF callback_;

  int dispatch_;

private:
  ACE_UNIMPLEMENTED_FUNC (ACE_EH_Dispatch_Info (const ACE_EH_Dispatch_Info &))
  ACE_UNIMPLEMENTED_FUNC (ACE_EH_Dispatch_Info &operator= (const ACE_EH_Dispatch_Info &))
};


/**
 * @class ACE_TP_Token_Guard
 *
 * @brief A helper class that helps grabbing, releasing  and waiting
 * on tokens for a thread that tries calling handle_events ().
 *
 * In short, this class will be owned by one thread by creating on the
 * stack. This class gives the status of the ownership of the token
 * and manages the ownership
 */

class ACE_Export ACE_TP_Token_Guard
{
public:

  /// Constructor that will grab the token for us
  ACE_TP_Token_Guard (ACE_Select_Reactor_Token &token);

  /// Destructor. This will release the token if it hasnt been
  /// released till this point
  ~ACE_TP_Token_Guard (void);

  /// Release the token ..
  void release_token (void);

  /// Returns whether the thread that created this object ownes the
  /// token or not.
  int is_owner (void);

  /// A helper method that grabs the token for us, after which the
  /// thread that owns that can do some actual work.
  /// @@todo: Should probably be called acquire_read_token ()
  int grab_token (ACE_Time_Value *max_wait_time = 0);

  /* A helper method that grabs the token for us, after which the
   * thread that owns that can do some actual work. This differs from
   * grab_token () as it uses acquire () to get the token instead of
   * acquire_read ()
   */
  int acquire_token (ACE_Time_Value *max_wait_time = 0);

private:

  /// The Select Reactor token.
  ACE_Select_Reactor_Token &token_;

  /// Flag that indicate whether the thread that created this object
  /// owns the token or not. A value of 0 indicates that this class
  /// hasnt got the token (and hence the thread) and a value of 1
  /// vice-versa.
  int owner_;

private:

  ACE_UNIMPLEMENTED_FUNC (ACE_TP_Token_Guard (void))
};

/**
 * @class ACE_TP_Reactor
 *
 * @brief Specialization of Select Reactor to support thread-pool based
 * event dispatching.
 *
 * One of the short comings of the Select_Reactor in ACE is that
 * it did not support a thread pool based event dispatching
 * model, similar to the one in WFMO_Reactor.  In
 * Select_Reactor, only thread can be blocked in <handle_events>
 * at any given time.
 * A new Reactor has been added to ACE that removes this
 * short-coming.  TP_Reactor is a specialization of Select
 * Reactor to support thread-pool based event dispatching. This
 * Reactor takes advantage of the fact that events reported by
 * <select> are persistent if not acted upon immediately.  It
 * works by remembering the event handler that just got
 * activated, releasing the internal lock (so that some other
 * thread can start waiting in the event loop) and then
 * dispatching the event handler outside the context of the
 * Reactor lock.
 * This Reactor is best suited for situations when the callbacks
 * to event handlers can take arbitrarily long and/or a number
 * of threads are available to run the event loops.
 * Note that callback code in Event Handlers
 * (e.g. Event_Handler::handle_input) does not have to be
 * modified or made thread-safe for this Reactor.  This is
 * because an activated Event Handler is suspended in the
 * Reactor before the upcall is made and resumed after the
 * upcall completes.  Therefore, one Event Handler cannot be
 * called by multiple threads simultaneously.
 */
class ACE_Export ACE_TP_Reactor : public ACE_Select_Reactor
{
public:

  // = Initialization and termination methods.

  /// Initialize <ACE_TP_Reactor> with the default size.
  ACE_TP_Reactor (ACE_Sig_Handler * = 0,
                  ACE_Timer_Queue * = 0,
                  int mask_signals = 1);

  /**
   * Initialize the <ACE_TP_Reactor> to manage
   * <max_number_of_handles>.  If <restart> is non-0 then the
   * <ACE_Reactor>'s <handle_events> method will be restarted
   * automatically when <EINTR> occurs.  If <signal_handler> or
   * <timer_queue> are non-0 they are used as the signal handler and
   * timer queue, respectively.
   */
  ACE_TP_Reactor (size_t max_number_of_handles,
                  int restart = 0,
                  ACE_Sig_Handler * = 0,
                  ACE_Timer_Queue * = 0,
                  int mask_signals = 1);

  // = Event loop drivers.

  /**
   * This event loop driver that blocks for <max_wait_time> before
   * returning.  It will return earlier if timer events, I/O events,
   * or signal events occur.  Note that <max_wait_time> can be 0, in
   * which case this method blocks indefinitely until events occur.
   *
   * <max_wait_time> is decremented to reflect how much time this call
   * took.  For instance, if a time value of 3 seconds is passed to
   * handle_events and an event occurs after 2 seconds,
   * <max_wait_time> will equal 1 second.  This can be used if an
   * application wishes to handle events for some fixed amount of
   * time.
   *
   * Returns the total number of <ACE_Event_Handler>s that were
   * dispatched, 0 if the <max_wait_time> elapsed without dispatching
   * any handlers, or -1 if something goes wrong.
   */
  virtual int handle_events (ACE_Time_Value *max_wait_time = 0);

  virtual int handle_events (ACE_Time_Value &max_wait_time);


  /// The following two overloaded methods are necessary as we dont
  /// want the TP_Reactor to call handle_close () with the token
  /// held.
  /**
   * Removes the <mask> binding of <eh> from the Select_Reactor.  If
   * there are no more bindings for this <eh> then it is removed from
   * the Select_Reactor.  Note that the Select_Reactor will call
   * <ACE_Event_Handler::get_handle> to extract the underlying I/O
   * handle.
   */
  virtual int remove_handler (ACE_Event_Handler *eh,
                              ACE_Reactor_Mask mask);

  /**
   * Removes the <mask> bind of <Event_Handler> whose handle is
   * <handle> from the Select_Reactor.  If there are no more bindings
   * for this <eh> then it is removed from the Select_Reactor.
   */
  virtual int remove_handler (ACE_HANDLE handle,
                              ACE_Reactor_Mask);

  /**
   * Removes all the <mask> bindings for handles in the <handle_set>
   * bind of <Event_Handler>.  If there are no more bindings for any
   * of these handlers then they are removed from the Select_Reactor.
   */
  virtual int remove_handler (const ACE_Handle_Set &handle_set,
                              ACE_Reactor_Mask);

  /* @todo: The following methods are not supported. Support for
   * signals is not available in the TP_Reactor. These methods will be
   * supported once signal handling is supported. We have to include
   * these two methods in the  TP_Reactor to keep some compilers
   * silent.
   */
  /**
   * Remove the ACE_Event_Handler currently associated with <signum>.
   * <sigkey> is ignored in this implementation since there is only
   * one instance of a signal handler.  Install the new disposition
   * (if given) and return the previous disposition (if desired by the
   * caller).  Returns 0 on success and -1 if <signum> is invalid.
   */
  virtual int remove_handler (int signum,
                              ACE_Sig_Action *new_disp,
                              ACE_Sig_Action *old_disp = 0,
                              int sigkey = -1);

  /// Calls <remove_handler> for every signal in <sigset>.
  virtual int remove_handler (const ACE_Sig_Set &sigset);

  /// Does the reactor allow the application to resume the handle on
  /// its own ie. can it pass on the control of handle resumption to
  /// the application.  The TP reactor has can allow applications to
  /// resume handles.  So return a +ve value.
  virtual int resumable_handler (void);

  /// GET/SET/ADD/CLR the dispatch mask "bit" bound with the <eh> and
  /// <mask>.
  virtual int mask_ops (ACE_Event_Handler *eh,
                        ACE_Reactor_Mask mask,
                        int ops);

  /// GET/SET/ADD/CLR the dispatch mask "bit" bound with the <handle>
  /// and <mask>.
  virtual int mask_ops (ACE_HANDLE handle,
                        ACE_Reactor_Mask mask,
                        int ops);

  /// Called from handle events
  static void no_op_sleep_hook (void *);

  // = Any thread can perform a <handle_events>, override the owner()
  //   methods to avoid the overhead of setting the owner thread.

  /// Set the new owner of the thread and return the old owner.
  virtual int owner (ACE_thread_t n_id, ACE_thread_t *o_id = 0);

  /// Return the current owner of the thread.
  virtual int owner (ACE_thread_t *);

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  // = Internal methods that do the actual work.


  /// Dispatch just 1 signal, timer, notification handlers
  int dispatch_i (ACE_Time_Value *max_wait_time,
                  ACE_TP_Token_Guard &guard);

  /// Get the event that needs dispatching.It could be either a
  /// signal, timer, notification handlers or return possibly 1 I/O
  /// handler for dispatching. In the most common use case, this would
  /// return 1 I/O handler for dispatching
  int get_event_for_dispatching (ACE_Time_Value *max_wait_time);

  /// Method to handle signals
  /// NOTE: It is just busted at this point in time.
  int handle_signals (int &event_count,
                      ACE_TP_Token_Guard &g);

  /// Handle timer events
  int handle_timer_events (int &event_count,
                           ACE_TP_Token_Guard &g);

  /// Handle notify events
  int handle_notify_events (int &event_count,
                            ACE_TP_Token_Guard &g);

  /// handle socket events
  int handle_socket_events (int &event_count,
                            ACE_TP_Token_Guard &g);

  /// This method shouldn't get called.
  virtual void notify_handle (ACE_HANDLE handle,
                              ACE_Reactor_Mask mask,
                              ACE_Handle_Set &,
                              ACE_Event_Handler *eh,
                              ACE_EH_PTMF callback);
private:

  /// Get the handle of the notify pipe from the ready set if there is
  /// an event  in the notify pipe.
  ACE_HANDLE get_notify_handle (void);

  /// Get socket event dispatch information.
  int get_socket_event_info (ACE_EH_Dispatch_Info &info);

  /// Notify the appropriate <callback> in the context of the <eh>
  /// associated with <handle> that a particular event has occurred.
  int dispatch_socket_event (ACE_EH_Dispatch_Info &dispatch_info);

private:
  /// Deny access since member-wise won't work...
  ACE_TP_Reactor (const ACE_TP_Reactor &);
  ACE_TP_Reactor &operator = (const ACE_TP_Reactor &);
};

#if defined (__ACE_INLINE__)
#include "ace/Other/TP_Reactor.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_TP_REACTOR_H */
