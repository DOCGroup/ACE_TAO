/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Timer_Heap.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_TIMER_HEAP_H)
#define ACE_TIMER_HEAP_H

#include "ace/Timer_Queue.h"

// Forward declaration
class ACE_Timer_Heap;

class ACE_Export ACE_Timer_Heap_Iterator : public ACE_Timer_Queue_Iterator
  // = TITLE
  //     Iterates over an <ACE_Timer_Queue>.
  //
  // = DESCRIPTION
  //     This is a special type of iterator that "advances" by moving
  //     the head of the timer queue up by one every time.
{
public:
  ACE_Timer_Heap_Iterator (ACE_Timer_Heap &);
  // Constructor.

  virtual int next (ACE_Timer_Node *&timer_node, 
		    const ACE_Time_Value &cur_time);
  // Pass back the next <timer_node> that hasn't been seen yet, if its
  // <time_value_> <= <cur_time>.  In addition, moves the timer queue
  // forward by one node.  Returns 0 when all <timer_nodes> have been
  // seen, else 1.

protected:
  ACE_Timer_Heap &timer_heap_;
  // Pointer to the <ACE_Timer_Heap> that we are iterating over.
};

class ACE_Timer_Heap : public ACE_Timer_Queue
  // = TITLE 
  //      Provides an interface to timers.
  //
  // = DESCRIPTION
  //      This implementation uses a heap-based callout queue of
  //      absolute times.  Therefore, in the average case, scheduling
  //      and expiring <ACE_Event_Handler> timers is O(log N) (where N
  //      is the total number of timers) and canceling timers is O(N)
  //      (since we need to perform linear search for the timer id).
{
  friend class ACE_Timer_Heap_Iterator;
public:
  // = Initialization and termination methods.
  ACE_Timer_Heap (size_t size = ACE_DEFAULT_MAX_TIMERS);
  // Constructor.

  virtual ~ACE_Timer_Heap (void);
  // Destructor.

  virtual int is_empty (void) const;
  // True if heap is empty, else false.

  virtual const ACE_Time_Value &earliest_time (void) const;
  // Returns the time of the earlier node in the Timer_Queue.

  virtual int schedule (ACE_Event_Handler *event_handler, 
		        const void *arg, 
		        const ACE_Time_Value &delay,
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

  virtual int cancel (ACE_Event_Handler *event_handler);
  // Cancel all <event_handlers> that match the address of
  // <event_handler>.  Returns number of handler's cancelled.

  virtual int cancel (int timer_id, const void **arg = 0);
  // Cancel the single <ACE_Event_Handler> that matches the <timer_id>
  // value (which was returned from the <schedule> method).  If arg is
  // non-NULL then it will be set to point to the ``magic cookie''
  // argument passed in when the <Event_Handler> was registered.  This
  // makes it possible to free up the memory and avoid memory leaks.
  // Returns 1 if cancellation succeeded and 0 if the <timer_id>
  // wasn't found.

  virtual void dump (void) const;
  // Dump the state of an object.

protected:
  virtual void reschedule (ACE_Timer_Node *);
  // Reschedule an "interval" <ACE_Timer_Node>.

  virtual ACE_Timer_Queue_Iterator &iterator (void);
  // Returns a pointer to this <ACE_Timer_Queue>'s iterator.

private:
  ACE_Timer_Node *remove (int index);
  // Remove and return the <index>th <ACE_Timer_Node> and restore the
  // heap property.

  void insert (ACE_Timer_Node *new_node);
  // Insert <new_node> into the heap and restore the heap property.

  void reheap_up (ACE_Timer_Node *new_node);
  // Restore the heap property.

  void reheap_down (ACE_Timer_Node *moved_node, int child_index);
  // Restore the heap property, starting at <child_index>.

  size_t max_size_;
  // Maximum size of the heap.

  size_t cur_size_;
  // Current size of the heap.
  
  ACE_Timer_Heap_Iterator iterator_;
  // Iterator used to expire timers.

  ACE_Timer_Node **heap_;
  // Current contents of the Heap, which is organized as a "heap" of
  // <ACE_Timer_Node> *'s.  In this context, a heap is a "partially
  // ordered, almost complete" binary tree, which is stored in an
  // array.
};

#if defined (__ACE_INLINE__)
#include "ace/Timer_Heap.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_TIMER_HEAP_H */
