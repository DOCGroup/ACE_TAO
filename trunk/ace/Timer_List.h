/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Timer_List.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_TIMER_LIST_H)
#define ACE_TIMER_LIST_H

#include "ace/Timer_Queue.h"

// Forward declaration
class ACE_Timer_List;

class ACE_Export ACE_Timer_List_Iterator : public ACE_Timer_Queue_Iterator
  // = TITLE
  //     Iterates over an <ACE_Timer_Queue>.
  //
  // = DESCRIPTION
  //     This is a special type of iterator that "advances" by moving
  //     the head of the timer queue up by one every time.
{
public:
  ACE_Timer_List_Iterator (ACE_Timer_List &);
  // Constructor.

  virtual int next (ACE_Timer_Node *&timer_node, 
		    const ACE_Time_Value &cur_time);
  // Pass back the next <timer_node> that hasn't been seen yet, if its
  // <time_value_> <= <cur_time>.  In addition, moves the timer queue
  // forward by one node.  Returns 0 when all <timer_nodes> have been
  // seen, else 1.

protected:
  ACE_Timer_List &timer_list_;
  // Pointer to the <ACE_Timer_List> that we are iterating over.
};

class ACE_Export ACE_Timer_List : public ACE_Timer_Queue
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
  //      is a subclass of <ACE_Timer_List> that implements a
  //      heap-based callout queue.
{
  friend class ACE_Timer_List_Iterator;
public: 
  // = Initialization and termination methods.
  ACE_Timer_List (void);
  // Default constructor.

  virtual ~ACE_Timer_List (void);
  // Destructor

  virtual int is_empty (void) const;
  // True if queue is empty, else false.

  virtual const ACE_Time_Value &earliest_time (void) const;
  // Returns the time of the earlier node in the <ACE_Timer_List>.

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

private:
  virtual void reschedule (ACE_Timer_Node *);
  // Reschedule an "interval" <ACE_Timer_Node>.

  virtual ACE_Timer_Queue_Iterator &iterator (void);
  // Returns a pointer to this <ACE_Timer_Queue>'s iterator.

  ACE_Timer_Node *head_; 
  // Pointer to linked list of <ACE_Timer_Handles>.

  ACE_Timer_List_Iterator iterator_;
  // Iterator used to expire timers.
};

#endif /* ACE_TIMER_LIST_H */
