// $Id$

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

#include "ace/Timer_Queue_T.h"

// Forward declaration
template <class TYPE, class FUNCTOR, class ACE_LOCK>
class ACE_Timer_Wheel_T;

template <class TYPE, class FUNCTOR, class ACE_LOCK>
class ACE_Timer_Wheel_Iterator_T : public ACE_Timer_Queue_Iterator_T <TYPE, FUNCTOR, ACE_LOCK>
  // = TITLE
  //     Iterates over an <ACE_Timer_Wheel>.
  //
  // = DESCRIPTION
  //     This is a generic iterator that can be used to visit every
  //     node of a timer queue.  Be aware that it doesn't transverse
  //     in the order of timeout values.  
{
public:
  ACE_Timer_Wheel_Iterator_T (ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK> &);
  // Constructor

  virtual void first (void);
  // Positions the iterator at the earliest node in the Timer Queue

  virtual void next (void);
  // Positions the iterator at the next node in the Timer Queue

  virtual int isdone (void);
  // Returns true when there are no more nodes in the sequence

  virtual ACE_Timer_Node_T<TYPE> *item (void);
  // Returns the node at the current position in the sequence

protected:
  ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK> &timer_wheel_;
  // Pointer to the <ACE_Timer_List> that we are iterating over.

  size_t pos_;  
  // Current position in the timing wheel
  
  ACE_Timer_Node_T<TYPE> *list_item_; 
  // Pointer to the position in the the <pos_>th list
};

template <class TYPE, class FUNCTOR, class ACE_LOCK>
class ACE_Timer_Wheel_T : public ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>
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
  typedef ACE_Timer_Wheel_Iterator_T<TYPE, FUNCTOR, ACE_LOCK> WHEEL_ITERATOR;
  // Type of iterator

  friend class ACE_Timer_Wheel_Iterator_T<TYPE, FUNCTOR, ACE_LOCK>;
  // Iterator is a friend

  typedef ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK> INHERITED;
  // Type inherited from

  // = Initialization and termination methods

  ACE_Timer_Wheel_T (size_t wheelsize, 
                     size_t resolution, 
                     size_t prealloc = 0,
                     FUNCTOR *upcall_functor = 0,
                     ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist = 0);
  // Constructor that takes in <wheelsize> - size of the timing wheel, 
  // <resolution> - resolution of time values the hashing function uses,
  // and <upcall_functor> - a functor that will be used instead of creating
  // a default functor.  Also, when the freelist is created, <prealloc> nodes
  // will be allocated. This can also take in a upcall functor and freelist 
  // (if 0, then defaults will be created)

  ACE_Timer_Wheel_T (FUNCTOR *upcall_functor = 0, 
                     ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist = 0);
  // Default constructor. <upcall_functor> is the instance of the
  // FUNCTOR to be used by the queue. If <upcall_functor> is 0, Timer
  // Queue will create a default FUNCTOR.  <freelist> the freelist of
  // timer nodes.  If 0, then a default freelist will be created.  The
  // defaults will be used for size and resolution and no preallocation
  // (ACE_DEFAULT_TIMER_WHEEL_SIZE, ACE_DEFAULT_TIMER_WHEEL_RESOLUTION)


  virtual ~ACE_Timer_Wheel_T (void);
  // Destructor

  virtual int is_empty (void) const;
  // True if queue is empty, else false.

  virtual const ACE_Time_Value &earliest_time (void) const;
  // Returns the time of the earlier node in the <ACE_Timer_Wheel>.

  virtual long schedule (const TYPE &type, 
			 const void *act, 
			 const ACE_Time_Value &delay,
			 const ACE_Time_Value &interval = ACE_Time_Value::zero);
  // Schedule <type> that will expire after <delay> amount of time.
  // If it expires then <act> is passed in as the value to the
  // <functor>.  If <interval> is != to <ACE_Time_Value::zero> then it
  // is used to reschedule the <type> automatically.  This method
  // returns a <timer_id> that uniquely identifies the the timer.
  // This <timer_id> can be used to cancel the timer before it expires.  
  // Returns -1 on failure.

  virtual int cancel (const TYPE &type,
		      int dont_call_handle_close = 1);
  // Cancel all timer associated with <type>.  If <dont_call> is 0
  // then the <functor> will be invoked.  Returns number of timers
  // cancelled.

  virtual int cancel (long timer_id, 
		      const void **act = 0,
		      int dont_call_handle_close = 1);
  // Cancel the single timer that matches the <timer_id> value (which
  // was returned from the <schedule> method).  If act is non-NULL
  // then it will be set to point to the ``magic cookie'' argument
  // passed in when the timer was registered.  This makes it possible
  // to free up the memory and avoid memory leaks.  If <dont_call> is
  // 0 then the <functor> will be invoked.  Returns 1 if cancellation
  // succeeded and 0 if the <timer_id> wasn't found.

  virtual int expire (void);
  // Run the <functor> for all timers whose values are <=
  // <ACE_OS::gettimeofday>.  Also accounts for <timer_skew>.  Returns
  // the number of timers canceled.

  int expire (const ACE_Time_Value &);
  // Run the <functor> for all timers whose values are <= <cur_time>.
  // This does not account for <timer_skew>.  Returns the number of
  // timers canceled.

  virtual ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR, ACE_LOCK> &iter (void);
  // Returns a pointer to this <ACE_Timer_Queue_T>'s iterator.

  virtual ACE_Timer_Node_T<TYPE> *remove_first (void);
  // Removes the earliest node from the queue and returns it

  virtual void dump (void) const;
  // Dump the state of an object.

private:
  virtual void reschedule (ACE_Timer_Node_T<TYPE> *);
  // Reschedule an "interval" node

  ACE_Timer_Node_T<TYPE> **wheel_;
  // Timing Wheel.

  size_t wheel_size_;
  // Size of the timing wheel.

  size_t resolution_;
  // Resolution (in microsoconds) of the timing wheel.

  size_t earliest_pos_;
  // Index of the list with the earliest time

  long size_;
  // Keeps track of the size of the queue

  WHEEL_ITERATOR *iterator_;
  // Iterator used to expire timers.

  ACE_Timer_Node_T<TYPE> *freelist_;
  // Pointer to the freelist of <ACE_Timer_Node_T<TYPE>>.

  // = Don't allow these operations for now.
  ACE_UNIMPLEMENTED_FUNC (ACE_Timer_Wheel_T (const ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK> &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK> &))
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Timer_Wheel_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Timer_Wheel_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_TIMER_WHEEL_T_H */
