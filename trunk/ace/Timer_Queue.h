/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Timer_Queue.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_TIMER_QUEUE_H)
#define ACE_TIMER_QUEUE_H

#include "ace/Event_Handler.h"
#include "ace/Time_Value.h"
#include "ace/Synch.h"

// This should be nested within the ACE_Timer_Queue class but some C++
// compilers still don't like this...

struct ACE_Timer_Node
  // = TITLE
  //     Maintains the state associated with a Timer entry.
{
friend class ACE_Timer_Queue;
private:
  ACE_Timer_Node (ACE_Event_Handler *h, 
		  const void *a, 
		  const ACE_Time_Value &t, 
		  const ACE_Time_Value &i, 
		  ACE_Timer_Node *n, 
		  int timer_id);
  // Constructor.

  ACE_Event_Handler *handler_;
  // Handler to invoke <handle_timeout> on when a timeout occurs.

  const void *arg_;
  // Argument to pass to <handle_timeout>.

  ACE_Time_Value timer_value_;
  // Time until the timer expires.

  ACE_Time_Value interval_;
  // If this is a periodic timer this holds the time until the next
  // timeout.

  ACE_Timer_Node *next_;
  // Pointer to next timer.
  
  int timer_id_;
  // Id of this timer (used to cancel timers before they expire).

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  void dump (void) const;
  // Dump the state of an object.
};

class ACE_Export ACE_Timer_Queue
  // = TITLE 
  //      Provides an interface to timers.
  //
  // = DESCRIPTION
  //      This is a simple implementation that uses a linked list of
  //      absolute times.  A more clever implementation would use a
  //      delta-list, a heap, or timing wheels, etc.
{
public: 
  // = Initialization and termination methods.
  ACE_Timer_Queue (void);
  // Default constructor.

  virtual ~ACE_Timer_Queue (void);

  int is_empty (void) const;
  // True if queue is empty, else false.

  const ACE_Time_Value &earliest_time (void) const;
  // Returns the time of the earlier node in the Timer_Queue.

  // = Set/get the timer skew for the Timer_Queue.
  void timer_skew (const ACE_Time_Value &skew);
  const ACE_Time_Value &timer_skew (void) const;

  virtual int schedule (ACE_Event_Handler *event_handler, 
		        const void *arg, 
		        const ACE_Time_Value &delay,
		        const ACE_Time_Value &interval = ACE_Time_Value::zero);
  // Schedule an <event_handler> that will expire after <delay> amount
  // of time.  If it expires then <arg> is passed in as the value to
  // the <event_handler>'s <handle_timeout> callback method.  If
  // <interval> is != to <ACE_Time_Value::zero> then it is used to
  // reschedule the <event_handler> automatically.  This method
  // returns a timer handle that uniquely identifies the
  // <event_handler> in an internal list.  This timer handle can be
  // used to cancel an <event_handler> before it expires.  The
  // cancellation ensures that timer_ids are unique up to values of
  // greater than 2 billion timers.  As long as timers don't stay
  // around longer than this there should be no problems with
  // accidentally deleting the wrong timer.

  virtual int cancel (ACE_Event_Handler *event_handler);
  // Cancel all <event_handlers> that match the address of
  // <event_handler>.

  virtual int cancel (int timer_id, const void **arg = 0);
  // Cancel the single <ACE_Event_Handler> that matches the <timer_id>
  // value (which was returned from the <schedule> method).  If arg is
  // non-NULL then it will be set to point to the ``magic cookie''
  // argument passed in when the <Event_Handler> was registered.  This
  // makes it possible to free up the memory and avoid memory leaks.

  virtual int expire (const ACE_Time_Value &current_time);
  // Run the <handle_timeout> method for all Timers whose values are
  // <= <cur_time>.  This does not account for <timer_skew>.  Returns
  // the number of <Event_Handler>s for which <handle_timeout> was
  // called.

  virtual int expire (void);
  // Run the <handle_timeout> method for all Timers whose values are
  // <= <ACE_OS::gettimeofday>.  Also accounts for <timer_skew>.
  // Returns the number of <Event_Handler>s for which <handle_timeout>
  // was called.

  virtual ACE_Time_Value *calculate_timeout (ACE_Time_Value *max);
  // Determine the next event to timeout.  Returns <max> if there are
  // no pending timers or if all pending timers are longer than max.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Time_Value timeout_;
  // Returned by calculate_timeout.

  virtual void reschedule (ACE_Timer_Node *);
  // Reschedule a "period" Timer_Node.

  ACE_Timer_Node *head_; 
  // Pointer to linked list of ACE_Timer_Handles. 

  int timer_id_;
  // Keeps track of the timer id that uniquely identifies each timer.
  // This id can be used to cancel a timer via the <cancel (int)>
  // method.

  ACE_Time_Value timer_skew_;
  // Adjusts for timer skew in various clocks.

#if defined (ACE_MT_SAFE)
  ACE_Recursive_Thread_Mutex lock_; 
  // Synchronization variable for the MT_SAFE ACE_Reactor 
#endif /* ACE_MT_SAFE */
};

#if defined (__ACE_INLINE__)
#include "ace/Timer_Queue.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_TIMER_QUEUE_H */
