/* -*- C++ -*- */

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Timer_Wheel.h
//
// = AUTHOR
//    Darrell Brunsch <brunsch@cs.wustl.edu>
// 
// ============================================================================

#if !defined (ACE_TIMER_WHEEL_T_H)
#define ACE_TIMER_WHEEL_T_H

#include "ace/Timer_Queue.h"

// Forward declaration
template <class TYPE, class FUNCTOR>
class ACE_Timer_Wheel_T;

template <class TYPE, class FUNCTOR>
class ACE_Timer_Wheel_Iterator_T : public ACE_Timer_Queue_Iterator_T <TYPE, FUNCTOR>
  // = TITLE
  //     Iterates over an <ACE_Timer_Wheel>.
  //
  // = DESCRIPTION
  //     This is a special type of iterator that "advances" by moving
  //     the head of the timer queue up by one every time.
{
public:
  ACE_Timer_Wheel_Iterator_T (ACE_Timer_Wheel_T<TYPE, FUNCTOR> &);
  // Constructor.

  virtual int next (ACE_Timer_Node_T<TYPE, FUNCTOR> *&timer_node, 
                    const ACE_Time_Value &cur_time);
  // Pass back the next <timer_node> that hasn't been seen yet, if its
  // <time_value_> <= <cur_time>.  In addition, moves the timer queue
  // forward by one node.  Returns 0 when all <timer_nodes> have been
  // seen, else 1.

  void reset (void);
  // Resets the iterator

protected:
  ACE_Timer_Wheel_T<TYPE, FUNCTOR> &timer_wheel_;
  // Pointer to the <ACE_Timer_List> that we are iterating over.

private:
  size_t pos_;  // Current position in the timing wheel
  ACE_Time_Value time_; // Corresponding time of <pos_>
};

template <class TYPE, class FUNCTOR>
class ACE_Timer_Wheel_T : public ACE_Timer_Queue_T<TYPE, FUNCTOR>
  // = TITLE 
  //      Provides a Timing Wheel version of Timer Queue
  //
  // = DESCRIPTION
  //      This implementation uses a hash table of ordered doubly-
  //      linked lists of absolute times.  The other enhancements 
  //      to Timer List include using the pointer to the node as the
  //      timer id (to speed up removing), adding a free list and 
  //      the ability to preallocate nodes.  Timer Wheel is based on
  //      the timing wheel implementation used in Adam M. Costello and
  //      George Varghese's paper "Redesigning the BSD Callout and
  //      Timer Facilities" 
  //      (http://dworkin.wustl.edu/~varghese/PAPERS/newbsd.ps.Z)
  //        
{
public: 
  typedef ACE_Timer_Wheel_Iterator_T<TYPE, FUNCTOR> WHEEL_ITERATOR;
  // Type of iterator

  friend class ACE_Timer_Wheel_Iterator_T<TYPE, FUNCTOR>;
  // Iterator is a friend.

  typedef ACE_Timer_Queue_T<TYPE, FUNCTOR> INHERITED;
  // Type inherited from.

  // = Initialization and termination methods.
  ACE_Timer_Wheel_T (size_t wheelsize = ACE_DEFAULT_TIMER_WHEEL_SIZE, 
                     size_t resolution = ACE_DEFAULT_TIMER_WHEEL_RESOLUTION, 
                     size_t prealloc = 0,
                     FUNCTOR *upcall_functor = 0);
  // Constructor that takes in a size for the timing wheel and a
  // resolution for placement in the timing wheel lists (in
  // microseconds).

  virtual ~ACE_Timer_Wheel_T (void);
  // Destructor

  virtual int is_empty (void) const;
  // True if queue is empty, else false.

  virtual const ACE_Time_Value &earliest_time (void) const;
  // Returns the time of the earlier node in the <ACE_Timer_Wheel>.

  virtual int schedule (const TYPE &type, 
		        const void *act, 
		        const ACE_Time_Value &delay,
		        const ACE_Time_Value &interval = ACE_Time_Value::zero);
  // Schedule <type> that will expire after <delay> amount of time.
  // If it expires then <act> is passed in as the value to the
  // <functor>.  If <interval> is != to <ACE_Time_Value::zero> then it
  // is used to reschedule the <type> automatically.  This method
  // returns a <timer_id> that uniquely identifies the the <type>
  // entry in an internal list.  This <timer_id> can be used to cancel
  // the timer before it expires.  The cancellation ensures that
  // <timer_ids> are unique up to values of greater than 2 billion
  // timers.  As long as timers don't stay around longer than this
  // there should be no problems with accidentally deleting the wrong
  // timer.  Returns -1 on failure (which is guaranteed never to be a
  // valid <timer_id>).

  virtual int cancel (const TYPE &type,
		      int dont_call_handle_close = 1);
  // Cancel all timer associated with <type>.  If <dont_call> is 0
  // then the <functor> will be invoked.  Returns number of timers
  // cancelled.

  virtual int cancel (int timer_id, 
		      const void **act = 0,
		      int dont_call_handle_close = 1);
  // Cancel the single timer that matches the <timer_id> value (which
  // was returned from the <schedule> method).  If act is non-NULL
  // then it will be set to point to the ``magic cookie'' argument
  // passed in when the timer was registered.  This makes it possible
  // to free up the memory and avoid memory leaks.  If <dont_call> is
  // 0 then the <functor> will be invoked.  Returns 1 if cancellation
  // succeeded and 0 if the <timer_id> wasn't found.

  virtual void dump (void) const;
  // Dump the state of an object.

protected:
  virtual ACE_Timer_Node_T<TYPE, FUNCTOR> *alloc_node (void);
  // Factory method that allocates a new node (uses operator new).

  virtual void free_node (ACE_Timer_Node_T<TYPE, FUNCTOR> *);
  // Factory method that frees a previously allocated node (uses
  // operator delete).

private:
  ACE_Timer_Node_T<TYPE, FUNCTOR> *remove (void);
  // Removes the earliest node and returns a pointer to it.

  virtual void reschedule (ACE_Timer_Node_T<TYPE, FUNCTOR> *);
  // Reschedule an "interval" node

  virtual ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR> &iter (void);
  // Returns a pointer to this <ACE_Timer_Queue_T>'s iterator.

  ACE_Timer_Node_T<TYPE, FUNCTOR> **wheel_;
  // Timing Wheel.

  size_t wheel_size_;
  // Size of the timing wheel.

  size_t resolution_;
  // Resolution (in microsoconds) of the timing wheel.

  size_t current_pos_;
  // Current position in the timing wheel.

  ACE_Time_Value current_time_;
  // Keeps track of the previous time <current_pos_> was updated.

  long size_;
  // Keeps track of the size of the queue

  ACE_Timer_Wheel_Iterator_T<TYPE, FUNCTOR> iterator_;
  // Iterator used to expire timers.

  ACE_Timer_Node_T<TYPE, FUNCTOR> *freelist_;
  // Pointer to the freelist of <ACE_Timer_Node_T<TYPE, FUNCTOR>>.

  // = Don't allow these operations for now.
  ACE_Timer_Wheel_T (const ACE_Timer_Wheel_T<TYPE, FUNCTOR> &);
  void operator= (const ACE_Timer_Wheel_T<TYPE, FUNCTOR> &);
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Timer_Wheel_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Timer_Wheel_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_TIMER_WHEEL_T_H */
