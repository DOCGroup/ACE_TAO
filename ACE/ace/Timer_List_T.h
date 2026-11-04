/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Timer_List_T.h
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */
//=============================================================================

#ifndef ACE_TIMER_LIST_T_H
#define ACE_TIMER_LIST_T_H
#include /**/ "ace/pre.h"

#include "ace/Timer_Queue_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declaration.
template <class TYPE, class FUNCTOR, class ACE_LOCK, typename TIME_POLICY>
class ACE_Timer_List_T;

/**
 * @class ACE_Timer_List_Iterator_T
 *
 * @brief Iterates over an ACE_Timer_List.
 *
 * This is a generic iterator that can be used to visit every
 * node of a timer queue.
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK, typename TIME_POLICY = ACE_Default_Time_Policy>
class ACE_Timer_List_Iterator_T
: public ACE_Timer_Queue_Iterator_T <TYPE>
{
public:
  using List = ACE_Timer_List_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>;
  /// Constructor.
  explicit ACE_Timer_List_Iterator_T (List &lst);

  ACE_Timer_List_Iterator_T (ACE_Timer_List_Iterator_T const &) = delete;
  ACE_Timer_List_Iterator_T (ACE_Timer_List_Iterator_T &&) = delete;
  ACE_Timer_List_Iterator_T &operator= (ACE_Timer_List_Iterator_T const &) = delete;
  ACE_Timer_List_Iterator_T &operator= (ACE_Timer_List_Iterator_T &&) = delete;

  /// Destructor.
  ~ACE_Timer_List_Iterator_T () override;

  /// Positions the iterator at the earliest node in the Timer Queue
  void first () override;

  /// Positions the iterator at the next node in the Timer Queue
  void next () override;

  /// Returns true when there are no more nodes in the sequence
  bool isdone () const override;

  /// Returns the node at the current position in the sequence
  ACE_Timer_Node_T<TYPE> *item () override;

protected:
  /// Pointer to the ACE_Timer_List that we are iterating over.
  List& list_;

  /// Current position in the ACE_Timer_List
  ACE_Timer_Node_T<TYPE> *current_node_ {};
};

/**
 * @class ACE_Timer_List_T
 *
 * @brief Provides a simple implementation of timers.
 *
 * This implementation uses a linked list of absolute times.
 * Therefore, in the average case, scheduling and canceling
 * timers is O(N) (where N is the total number of timers) and
 * expiring timers is O(K) (where K is the total number of timers
 * that are < the current time of day).
 * More clever implementations could use a delta-list, a heap,
 * or timing wheels, etc.  For instance, ACE_Timer_Heap
 * is a subclass of ACE_Timer_List that implements a
 * heap-based callout queue.  For most applications, the
 * ACE_Timer_Heap will perform substantially faster than the
 * ACE_Timer_List.
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK, typename TIME_POLICY = ACE_Default_Time_Policy>
class ACE_Timer_List_T : public ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>
{
public:
  /// Type of iterator
  using Iterator = ACE_Timer_List_Iterator_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>;

  /// Iterator is a friend
  friend class ACE_Timer_List_Iterator_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>;

  using Node = ACE_Timer_Node_T<TYPE>;
  /// Type inherited from
  using Base_Timer_Queue = ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>;
  using FreeList = ACE_Free_List<Node>;

  /**
   * Default constructor. @a upcall_functor is the instance of the
   * FUNCTOR to be used by the list. If @a upcall_functor is nullptr, a
   * default FUNCTOR will be created.  @a freelist is the freelist of
   * timer nodes.  If nullptr, then a default freelist will be created.
   */
  explicit ACE_Timer_List_T (FUNCTOR *upcall_functor = nullptr, FreeList *freelist = nullptr,
                             TIME_POLICY const &time_policy = TIME_POLICY());

  /// Destructor
  ~ACE_Timer_List_T () override;

  /// True if queue is empty, else false.
  bool is_empty () const override;

  /// Returns the time of the earlier node in the ACE_Timer_List.
  /// Must be called on a non-empty queue.
  ACE_Time_Value const &earliest_time () const override;

  /**
   * Resets the interval of the timer represented by @a timer_id to
   * @a interval, which is specified in relative time to the current
   * <gettimeofday>.  If @a interval is equal to
   * ACE_Time_Value::zero, the timer will become a non-rescheduling
   * timer.  Returns 0 if successful, -1 if not.
   */
  int reset_interval (long timer_id,
                      ACE_Time_Value const &interval) override;

  /**
   * Cancel all timers associated with @a type.  If @a dont_call_handle_close
   * is 0 then the @a functor will be invoked.  Returns the number of timers
   * cancelled.
   */
  int cancel (TYPE const &type,
              int dont_call_handle_close = 1) override;

  /**
   * Cancel the single timer that matches the @a timer_id value (which
   * was returned from the <schedule> method).  If act is non-NULL
   * then it will be set to point to the ``magic cookie'' argument
   * passed in when the timer was registered.  This makes it possible
   * to free up the memory and avoid memory leaks.  If <dont_call> is
   * 0 then the <functor> will be invoked.  Returns 1 if cancellation
   * succeeded and 0 if the @a timer_id wasn't found.
   */
  int cancel (long timer_id,
              void const **act = nullptr,
              int dont_call_handle_close = 1) override;

  /**
   * Destroy timer queue. Cancels all timers.
   */
  int close () override;

  /// Returns a pointer to this ACE_Timer_Queue's iterator.
  ACE_Timer_Queue_Iterator_T<TYPE> &iter () override;

  /// Removes the earliest node from the queue and returns it
  ACE_Timer_Node_T<TYPE> *remove_first () override;

  /// Dump the state of an object.
  void dump () const override;

  /// Reschedule an "interval" ACE_Timer_Node_T.  This should be private
  /// but for now it needs to be public for <ACE_Timer_Hash_T>
  void reschedule (ACE_Timer_Node_T<TYPE> *) override;

  /// Reads the earliest node from the queue and returns it.
  ACE_Timer_Node_T<TYPE> *get_first () override;

private:
  /**
   * Schedule @a type that will expire at @a future_time, which is
   * specified in absolute time.  If it expires then @a act is passed
   * in as the value to the <functor>.  If @a interval is != to
   * ACE_Time_Value::zero then it is used to reschedule the @a type
   * automatically, using relative time to the current <gettimeofday>.
   * This method returns a timer_id that uniquely identifies the the
   * @a type entry in an internal list.  This timer_id can be used to
   * cancel the timer before it expires.  The cancellation ensures
   * that <timer_ids> are unique up to values of greater than 2
   * billion timers.  As long as timers don't stay around longer than
   * this there should be no problems with accidentally deleting the
   * wrong timer.  Returns -1 on failure (which is guaranteed never to
   * be a valid <timer_id>).
   */
  long schedule_i (TYPE const &type,
                   void const *act,
                   ACE_Time_Value const &future_time,
                   ACE_Time_Value const &interval) override;

  void schedule_i(ACE_Timer_Node_T<TYPE>* n, const ACE_Time_Value& exp);

  ACE_Timer_Node_T<TYPE>* find_node(long timer_id) const;

  void cancel_i (ACE_Timer_Node_T<TYPE>* n);

  void unlink (ACE_Timer_Node_T<TYPE>* n);

  ACE_Timer_Node_T<TYPE>* get_first_i() const;

private:
  /// Pointer to linked list of <ACE_Timer_Handles>.
  ACE_Timer_Node_T<TYPE>* head_;

  /// Iterator used to expire timers.
  Iterator *iterator_ {};

  /**
   * Keeps track of the timer id that uniquely identifies each timer.
   * This id can be used to cancel a timer via the <cancel(long)>
   * method.
   */
  long id_counter_;

  ACE_Timer_List_T (ACE_Timer_List_T const &) = delete;
  ACE_Timer_List_T (ACE_Timer_List_T &&) = delete;
  ACE_Timer_List_T &operator= (ACE_Timer_List_T const &) = delete;
  ACE_Timer_List_T &operator= (ACE_Timer_List_T &&) = delete;
};

#include "ace/Timer_List_T.cpp"

#include /**/ "ace/post.h"
#endif /* ACE_TIMER_LIST_T_H */
