/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    TP_Reactor.h
//
// = DESCRIPTION
//    TP_Reactor (ala, Thread Pool Reactor) uses the leader-follower
//    model to demultiplex requests among a bunch of threads.
//    Basically, when using thread pool reactor, one will pre-spawn a
//    _fixed_ number of threads.  When you issue the run_event_loop ()
//    method, one of the thread will become the leader thread and wait
//    for an event.  The other threads (followers) will be lined up
//    and waiting for their turns to become the leader.  When an event
//    occurs, the leader will pick a follower to become the leader and
//    go on to handle the event.  The consequence of using TP_Reactor
//    is the amortization of the costs used to creating threads.  The
//    context switching cost will also reduce.  More over, the total
//    resources used by threads are bounded (because there are only so
//    many threads.)
//
// = AUTHOR
//    Irfan Pyarali
//    Nanbor Wang
//
// ============================================================================

#ifndef ACE_TP_REACTOR_H
#define ACE_TP_REACTOR_H

#include "ace/Select_Reactor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_EH_Dispatch_Info
{
  // = TITLE
  //
  //     This structure contains information of the activated event
  //     handler.
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
};

class ACE_Export ACE_TP_Reactor : public ACE_Select_Reactor
{
  // = TITLE
  //
  //     Specialization of Select Reactor to support thread-pool based
  //     event dispatching.
  //
  // = DESCRIPTION
  //
  //     One of the short comings of the Select_Reactor in ACE is that
  //     it did not support a thread pool based event dispatching
  //     model, similar to the one in WFMO_Reactor.  In
  //     Select_Reactor, only thread can be blocked in handle_events()
  //     at any given time.
  //
  //     A new Reactor has been added to ACE that removes this
  //     short-coming.  TP_Reactor is a specialization of Select
  //     Reactor to support thread-pool based event dispatching. This
  //     Reactor takes advantage of the fact that events reported by
  //     select() are persistent if not acted upon immediately.  It
  //     works by remembering the event handler that just got
  //     activated, releasing the internal lock (so that some other
  //     thread can start waiting in the event loop) and then
  //     dispatching the event handler outside the context of the
  //     Reactor lock.
  //
  //     This Reactor is best suited for situations when the callbacks
  //     to event handlers can take arbitrarily long and/or a number
  //     of threads are available to run the event loops.
  //
  //     Note that callback code in Event Handlers
  //     (e.g. Event_Handler::handle_input) does not have to be
  //     modified or made thread-safe for this Reactor.  This is
  //     because an activated Event Handler is suspended in the
  //     Reactor before the upcall is made and resumed after the
  //     upcall completes.  Therefore, one Event Handler cannot be
  //     called by multiple threads simultaneously.
public:

  // = Initialization and termination methods.

  ACE_TP_Reactor (ACE_Sig_Handler * = 0,
                  ACE_Timer_Queue * = 0);
  // Initialize <ACE_TP_Reactor> with the default size.

  ACE_TP_Reactor (size_t max_number_of_handles,
                  int restart = 0,
                  ACE_Sig_Handler * = 0,
                  ACE_Timer_Queue * = 0);
  // Initialize the <ACE_TP_Reactor> to manage
  // <max_number_of_handles>.  If <restart> is non-0 then the
  // <ACE_Reactor>'s <handle_events> method will be restarted
  // automatically when <EINTR> occurs.  If <signal_handler> or
  // <timer_queue> are non-0 they are used as the signal handler and
  // timer queue, respectively.

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

  static void no_op_sleep_hook (void *);
  // Called from handle events

  virtual void wakeup_all_threads (void);
  // Wake up all threads in waiting in the event loop

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  // = Internal methods that do the actual work.

  virtual int dispatch_io_set (int number_of_active_handles,
                               int& number_dispatched,
                               int mask,
                               ACE_Handle_Set& dispatch_mask,
                               ACE_Handle_Set& ready_mask,
                               ACE_EH_PTMF callback);
  // Overwrites ACE_Select_Reactor::dispatch_io_set() to *not*
  // dispatch any event handlers.  The information of one activated
  // event handler is stored away, so that the event handler can be
  // dispatch later.

  virtual void notify_handle (ACE_HANDLE handle,
                              ACE_Reactor_Mask mask,
                              ACE_Handle_Set &,
                              ACE_Event_Handler *eh,
                              ACE_EH_PTMF callback);
  // This method shouldn't get called.

  virtual int notify_handle (ACE_EH_Dispatch_Info &dispatch_info);
  // Notify the appropriate <callback> in the context of the <eh>
  // associated with <handle> that a particular event has occurred.

  ACE_EH_Dispatch_Info dispatch_info_;
  // Dispatch information of the activated event handler

private:
  ACE_TP_Reactor (const ACE_TP_Reactor &);
  ACE_TP_Reactor &operator = (const ACE_TP_Reactor &);
  // Deny access since member-wise won't work...
};

#if defined (__ACE_INLINE__)
#include "ace/TP_Reactor.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_TP_REACTOR_H */
