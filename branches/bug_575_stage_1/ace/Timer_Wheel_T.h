
/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Timer_Wheel_T.h
 *
 *  $Id$
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_TIMER_WHEEL_T_H
#define ACE_TIMER_WHEEL_T_H
#include "ace/pre.h"

#include "ace/Timer_Queue_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declaration
template <class TYPE, class FUNCTOR, class ACE_LOCK>
class ACE_Timer_Wheel_T;

/**
 * @class ACE_Timer_Wheel_Iterator_T
 *
 * @brief Iterates over an <ACE_Timer_Wheel>.
 *
 * This is a generic iterator that can be used to visit every
 * node of a timer queue.  Be aware that it doesn't transverse
 * in the order of timeout values.
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK>
class ACE_Timer_Wheel_Iterator_T
  : public ACE_Timer_Queue_Iterator_T <TYPE, FUNCTOR, ACE_LOCK>
{
public:
  /// Constructor
  ACE_Timer_Wheel_Iterator_T (ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK> &);

  /// Destructor
  ~ACE_Timer_Wheel_Iterator_T (void);

  /// Positions the iterator at the earliest node in the Timer Queue
  virtual void first (void);

  /// Positions the iterator at the next node in the Timer Queue
  virtual void next (void);

  /// Returns true when there are no more nodes in the sequence
  virtual int isdone (void) const;

  /// Returns the node at the current position in the sequence
  virtual ACE_Timer_Node_T<TYPE> *item (void);

protected:
  /// Pointer to the <ACE_Timer_List> that we are iterating over.
  ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK> &timer_wheel_;

  /// Current position in the timing wheel
  size_t pos_;

  /// Pointer to the position in the the <pos_>th list
  ACE_Timer_Node_T<TYPE> *list_item_;
};

/**
 * @class ACE_Timer_Wheel_T
 *
 * @brief Provides a Timing Wheel version of Timer Queue
 *
 * This implementation uses a hash table of ordered doubly-
 * linked lists of absolute times.  The other enhancements
 * to Timer List include using the pointer to the node as the
 * timer id (to speed up removing), adding a free list and
 * the ability to preallocate nodes.  Timer Wheel is based on
 * the timing wheel implementation used in Adam M. Costello and
 * George Varghese's paper "Redesigning the BSD Callout and
 * Timer Facilities"
 * (http://dworkin.wustl.edu/~varghese/PAPERS/newbsd.ps.Z)
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK>
class ACE_Timer_Wheel_T : public ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>
{
public:
  /// Type of iterator
  typedef ACE_Timer_Wheel_Iterator_T<TYPE, FUNCTOR, ACE_LOCK> WHEEL_ITERATOR;

  /// Iterator is a friend
  friend class ACE_Timer_Wheel_Iterator_T<TYPE, FUNCTOR, ACE_LOCK>;

  /// Type inherited from
  typedef ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK> INHERITED;

  /// Default constructor
  ACE_Timer_Wheel_T (FUNCTOR *upcall_functor = 0,
                     ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist = 0);

  /// Constructor with opportunities to set the wheelsize and resolution
  ACE_Timer_Wheel_T (size_t wheelsize,
                     size_t resolution,
                     size_t prealloc = 0,
                     FUNCTOR *upcall_functor = 0,
                     ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist = 0);

  /// Destructor
  virtual ~ACE_Timer_Wheel_T (void);

  /// True if queue is empty, else false.
  virtual int is_empty (void) const;

  /// Returns the time of the earlier node in the <ACE_Timer_Wheel>.
  /// Must be called on a non-empty queue.
  virtual const ACE_Time_Value &earliest_time (void) const;

  /// Schedules a timer.
  virtual long schedule (const TYPE &type,
                         const void *act,
                         const ACE_Time_Value &delay,
                         const ACE_Time_Value &interval
                           = ACE_Time_Value::zero);

  /// Changes the interval of a timer (and can make it periodic or non
  /// periodic by setting it to ACE_Time_Value::zero or not).
  virtual int reset_interval (long timer_id,
                              const ACE_Time_Value &interval);

  /// Cancel all timer associated with <type>.  If <dont_call> is 0
  /// then the <functor> will be invoked.  Returns number of timers
  /// cancelled.
  virtual int cancel (const TYPE &type,
                      int dont_call_handle_close = 1);

  // Cancel a timer, storing the magic cookie in act (if nonzero).
  // Calls the functor if dont_call_handle_close is 0 and returns 1
  // on success
  virtual int cancel (long timer_id,
                      const void **act = 0,
                      int dont_call_handle_close = 1);

  /// Run the <functor> for all timers whose values are <=
  /// <ACE_OS::gettimeofday>.  Also accounts for <timer_skew>.  Returns
  /// the number of timers canceled.
  virtual int expire (void);

  // Run the <functor> for all timers whose values are <= <cur_time>.
  // This does not account for <timer_skew>.  Returns the number of
  // timers canceled.
  int expire (const ACE_Time_Value &);

  /// Returns a pointer to this <ACE_Timer_Queue_T>'s iterator.
  virtual ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR, ACE_LOCK> &iter (void);

  /// Removes the earliest node from the queue and returns it
  virtual ACE_Timer_Node_T<TYPE> *remove_first (void);

  /// Dump the state of an object.
  virtual void dump (void) const;

  /// Reads the earliest node from the queue and returns it.
  virtual ACE_Timer_Node_T<TYPE> *get_first (void);

private:
  /// Reschedule an "interval" node
  virtual void reschedule (ACE_Timer_Node_T<TYPE> *);

  /// Timing Wheel.
  ACE_Timer_Node_T<TYPE> **wheel_;

  /// Size of the timing wheel.
  size_t wheel_size_;

  /// Resolution (in microsoconds) of the timing wheel.
  size_t resolution_;

  /// Index of the list with the earliest time
  size_t earliest_pos_;

  /// Keeps track of the size of the queue
  long size_;

  /// Iterator used to expire timers.
  WHEEL_ITERATOR *iterator_;

  /// Pointer to the freelist of <ACE_Timer_Node_T<TYPE>>.
  ACE_Timer_Node_T<TYPE> *freelist_;

  // = Don't allow these operations for now.
  ACE_UNIMPLEMENTED_FUNC (
    ACE_Timer_Wheel_T (const ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK> &))
  ACE_UNIMPLEMENTED_FUNC (
    void operator= (const ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK> &))
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#if !defined (ACE_HAS_BROKEN_HPUX_TEMPLATES)
#include "ace/Timer_Wheel_T.cpp"
#endif /* !ACE_HAS_BROKEN_HPUX_TEMPLATES */
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Timer_Wheel_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* ACE_TIMER_WHEEL_T_H */
