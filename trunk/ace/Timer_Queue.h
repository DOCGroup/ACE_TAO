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

// Forward declaration.
class ACE_Timer_Queue;

// This should be nested within the ACE_Timer_Queue class but some C++
// compilers still don't like this...

class ACE_Export ACE_Timer_Node
  // = TITLE
  //     Maintains the state associated with a Timer entry.
{
  // = The use of friends should be replaced with accessors...
  friend class ACE_Timer_Queue;
  friend class ACE_Timer_List;
  friend class ACE_Timer_List_Iterator;
  friend class ACE_Timer_Heap;
  friend class ACE_Timer_Heap_Iterator;

  // = Initialization methods.
  ACE_Timer_Node (ACE_Event_Handler *h, 
		  const void *a, 
		  const ACE_Time_Value &t, 
		  const ACE_Time_Value &i, 
		  ACE_Timer_Node *n, 
		  int timer_id);
  // Constructor.

  ACE_Timer_Node (void);
  // Default constructor.

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

class ACE_Export ACE_Timer_Queue_Iterator
  // = TITLE
  //     Generic interfae for iterating over a subclass of
  //     <ACE_Timer_Queue>.
  //
  // = DESCRIPTION
  //     This is a special type of iterator that "advances" by moving
  //     the head of the timer queue up by one every time.
{
public:
  ACE_Timer_Queue_Iterator (void);
  virtual ~ACE_Timer_Queue_Iterator (void);

  virtual int next (ACE_Timer_Node *&timer_node, 
		    const ACE_Time_Value &cur_time) = 0;
  // Pass back the next <timer_node> that hasn't been seen yet, if its
  // <time_value_> <= <cur_time>.  In addition, moves the timer queue
  // forward by one node.  Returns 0 when all <timer_nodes> have been
  // seen, else 1.
};

class ACE_Export ACE_Timer_Queue
  // = TITLE 
  //      Provides an interface to timers.
  //
  // = DESCRIPTION
  //      This implementation uses a linked list of absolute times.
  //      Therefore, in the average case, scheduling and canceling
  //      <ACE_Event_Handler> timers is O(N) (where N is the total
  //      number of timers) and expiring timers is O(K) (where K is
  //      the total number of timers that are < the current time of
  //      day).
  //
  //      More clever implementations could use a delta-list, a heap,
  //      or timing wheels, etc.  For instance, <ACE_Timer_Heap>
  //      is a subclass of <ACE_Timer_Queue> that implements a
  //      heap-based callout queue.
{
public: 
  // = Initialization and termination methods.
  ACE_Timer_Queue (void);
  // Default constructor.

  virtual ~ACE_Timer_Queue (void);
  // Destructor - make virtual for proper destruction of inherited
  // classes.

  virtual int is_empty (void) const = 0;
  // True if queue is empty, else false.

  virtual const ACE_Time_Value &earliest_time (void) const = 0;
  // Returns the time of the earlier node in the Timer_Queue.

  virtual int schedule (ACE_Event_Handler *event_handler, 
		        const void *arg, 
		        const ACE_Time_Value &delay,
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

  virtual int cancel (ACE_Event_Handler *event_handler) = 0;
  // Cancel all <event_handlers> that match the address of
  // <event_handler>.  Returns number of handler's cancelled.

  virtual int cancel (int timer_id, const void **arg = 0) = 0;
  // Cancel the single <ACE_Event_Handler> that matches the <timer_id>
  // value (which was returned from the <schedule> method).  If arg is
  // non-NULL then it will be set to point to the ``magic cookie''
  // argument passed in when the <Event_Handler> was registered.  This
  // makes it possible to free up the memory and avoid memory leaks.
  // Returns 1 if cancellation succeeded and 0 if the <timer_id>
  // wasn't found.

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

  // = Set/get the timer skew for the Timer_Queue.
  void timer_skew (const ACE_Time_Value &skew);
  const ACE_Time_Value &timer_skew (void) const;

  virtual void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  virtual void reschedule (ACE_Timer_Node *) = 0;
  // Reschedule an "interval" <ACE_Timer_Node>.

  virtual ACE_Timer_Queue_Iterator &iter (void) = 0;
  // Returns a pointer to this <ACE_Timer_Queue>'s iterator.

  virtual ACE_Timer_Node *alloc_node (void) = 0;
  // Factory method that allocates a new node.

  virtual void free_node (ACE_Timer_Node *) = 0;
  // Factory method that frees a previously allocated node.

#if defined (ACE_MT_SAFE)
  ACE_Recursive_Thread_Mutex lock_; 
  // Synchronization variable for the MT_SAFE ACE_Reactor 
#endif /* ACE_MT_SAFE */

private:
  ACE_Time_Value timeout_;
  // Returned by <calculate_timeout>.

  ACE_Time_Value timer_skew_;
  // Adjusts for timer skew in various clocks.
};

#if defined (__ACE_INLINE__)
#include "ace/Timer_Queue.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_TIMER_QUEUE_H */
