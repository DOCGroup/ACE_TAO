// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Timer_Queue_Adapters.h
//
// = AUTHOR
//    Douglas C. Schmidt and Carlos O'Ryan
// 
// ============================================================================

#if !defined (ACE_TIMER_QUEUE_ADAPTERS_H)
#define ACE_TIMER_QUEUE_ADAPTERS_H

#include "ace/Task.h"
#include "ace/Signal.h"

template <class TQ>
class ACE_Export ACE_Async_Timer_Queue_Adapter : public ACE_Event_Handler
  // = TITLE
  //     Adapts a <TQ> to be run asynchronously.
  //
  // = DESCRIPTION
  //     This implementation uses the <ualarm> call, which generates
  //     the SIGARLM signal that is caught by this class.
{
public:
  typedef TQ TIMER_QUEUE;

  ACE_Async_Timer_Queue_Adapter (ACE_Sig_Set *mask = 0);
  // Register the SIGALRM handler.  If <mask> == 0 then block all
  // signals when <SIGALRM> is run.  Otherwise, just block the signals
  // indicated in <mask>.

  long schedule (ACE_Event_Handler *type,
		 const void *act, 
		 const ACE_Time_Value &delay,
		 const ACE_Time_Value &interval = ACE_Time_Value::zero);
  // Schedule the timer according to the semantics of the
  // <ACE_Timer_List>.  However, this timer gets dispatched via a
  // signal, rather than by a user calling <expire>.

  int cancel (long timer_id, const void **act = 0);
  // Cancel the <timer_id> and pass back the <act> if an address is
  // passed in.

  int expire (void);
  // Dispatch all timers whose values are <= <cur_time>.  Returns the
  // number of timers canceled.

  TQ &timer_queue (void);
  // Access the underlying <TIMER_QUEUE>.

private:
  virtual int schedule_ualarm (void);
  // Perform the logic to compute the new ualarm(2) setting.

  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);
  // Called back by <SIGALRM> handler.

  ACE_Sig_Handler sig_handler_;
  // Handler for the <SIGALRM> signal, so that we can access our state
  // without requiring any global variables.

  TQ timer_queue_;
  // Implementation of the timer queue (e.g., <ACE_Timer_List>,
  // <ACE_Timer_Heap>, etc.).

  ACE_Sig_Set mask_;
  // Mask of signals to be blocked when we're servicing <SIGALRM>.
};

template <class TQ>
class ACE_Export ACE_Thread_Timer_Queue_Adapter : public ACE_Task_Base
  // = TITLE
  //   Adapts a Timer_Queue using a separate thread for dispatching.
  //
  // = DESCRIPTION
  //   This implementation of a Timer_Queue uses a separate thread to
  //   dispatch the timers. The base queue need not be thread safe,
  //   this class takes all the necessary locks.
  //
  // = NOTE
  //   This is a case were template parameters will be useful, but
  //   (IMHO) the effort and portability problems discourage their
  //   use.
{
public:
  typedef TQ TIMER_QUEUE;

  ACE_Thread_Timer_Queue_Adapter (void);
  // Creates the timer queue.  Activation of the task is the user's
  // responsibility.  

  long schedule (ACE_Event_Handler* handler,
		 const void *act,
		 const ACE_Time_Value &delay,
		 const ACE_Time_Value &interval = ACE_Time_Value::zero);
  // Schedule the timer according to the semantics of the <TQ>; wakes
  // up the dispatching thread.

  int cancel (long timer_id, const void **act = 0);
  // Cancel the <timer_id> add return the <act> parameter if an
  // address is passed in. Also wakes up the dispatching thread.

  virtual int svc (void);
  // Runs the dispatching thread.

  virtual void deactivate (void);
  // Inform the dispatching thread that it should terminate.

  ACE_SYNCH_MUTEX &lock (void);
  // Access the locking mechanism, useful for iteration.

  TQ &timer_queue (void);
  // Access the implementation queue, useful for iteration.

private:
  TQ timer_queue_;
  // The underlying Timer_Queue.

  ACE_SYNCH_CONDITION condition_;
  // The dispatching thread sleeps on this condition while waiting to
  // dispatch the next timer; it is used to wake it up if there is a
  // change on the timer queue.

  ACE_SYNCH_MUTEX lock_;
  // The mutual exclusion mechanism which is required to use the
  // <condition_>.

  int active_;
  // When deactive is called this variable turns to false and the
  // dispatching thread is signalled, to terminate its main loop.
};

#if defined (__ACE_INLINE__)
#include "ace/Timer_Queue_Adapters.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Timer_Queue_Adapters.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Timer_Queue_Adapters.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_TIMER_QUEUE_ADAPTERS_H */
