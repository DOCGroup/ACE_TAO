/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Timer_Hash_T.h
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_TIMER_HASH_T_H
#define ACE_TIMER_HASH_T_H
#include /**/ "ace/pre.h"

#include "ace/Timer_Queue_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Free_List.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declaration.
template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY>
class ACE_Timer_Hash_T;
template <typename TYPE>
class Hash_Token;
class ACE_Event_Handler;

/**
 * @class ACE_Timer_Hash_Upcall
 *
 * @brief Functor for Timer_Hash
 *
 * This class calls up to the Timer Hash's functor from the
 * timer queues in the hash table
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK>
class ACE_Timer_Hash_Upcall
{
public:
  using TIMER_QUEUE = ACE_Timer_Queue_T<
    ACE_Event_Handler *, ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, ACE_LOCK>, ACE_Null_Mutex>;

  /// Default constructor (creates an invalid object, but needs to be here
  /// so timer queues using this functor can be constructed)
  ACE_Timer_Hash_Upcall ();

  /// Constructor that specifies a Timer_Hash to call up to
  explicit ACE_Timer_Hash_Upcall (ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK> *timer_hash);

  ACE_Timer_Hash_Upcall (ACE_Timer_Hash_Upcall const &) = delete;
  ACE_Timer_Hash_Upcall (ACE_Timer_Hash_Upcall &&) = delete;
  ACE_Timer_Hash_Upcall &operator= (ACE_Timer_Hash_Upcall const &) = delete;
  ACE_Timer_Hash_Upcall &operator= (ACE_Timer_Hash_Upcall &&) = delete;

  /// This method is called when a timer is registered.
  int registration (TIMER_QUEUE &timer_queue,
                    ACE_Event_Handler *handler,
                    const void *arg);

  /// This method is called before the timer expires.
  int preinvoke (TIMER_QUEUE &timer_queue,
                 ACE_Event_Handler *handler,
                 const void *arg,
                 int recurring_timer,
                 const ACE_Time_Value &cur_time,
                 const void *&upcall_act);

  /// This method is called when the timer expires.
  int timeout (TIMER_QUEUE &timer_queue,
               ACE_Event_Handler *handler,
               const void *arg,
               int recurring_timer,
               const ACE_Time_Value &cur_time);

  /// This method is called after the timer expires.
  int postinvoke (TIMER_QUEUE &timer_queue,
                  ACE_Event_Handler *handler,
                  const void *arg,
                  int recurring_timer,
                  const ACE_Time_Value &cur_time,
                  const void *upcall_act);

  /// This method is called when a handler is cancelled
  int cancel_type (TIMER_QUEUE &timer_queue,
                   ACE_Event_Handler *handler,
                   int dont_call,
                   int &requires_reference_counting);

  /// This method is called when a timer is cancelled
  int cancel_timer (TIMER_QUEUE &timer_queue,
                    ACE_Event_Handler *handler,
                    int dont_call,
                    int requires_reference_counting);

  /// This method is called when the timer queue is destroyed and
  /// the timer is still contained in it
  int deletion (TIMER_QUEUE &timer_queue,
                ACE_Event_Handler *handler,
                const void *arg);

private:
  /// Timer Queue to do the calling up to
  ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK> *timer_hash_;
};

/**
 * @class ACE_Timer_Hash_Iterator_T
 *
 * @brief Iterates over an ACE_Timer_Hash_T.
 *
 * This is a generic iterator that can be used to visit every
 * node of a timer queue.  Be aware that it doesn't transverse
 * in the order of timeout values.
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY = ACE_Default_Time_Policy>
class ACE_Timer_Hash_Iterator_T : public ACE_Timer_Queue_Iterator_T <TYPE>
{
public:
  /// Constructor.
  using Hash = ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>;
  explicit ACE_Timer_Hash_Iterator_T (Hash &);

  ACE_Timer_Hash_Iterator_T (ACE_Timer_Hash_Iterator_T const &) = delete;
  ACE_Timer_Hash_Iterator_T (ACE_Timer_Hash_Iterator_T &&) = delete;
  ACE_Timer_Hash_Iterator_T &operator= (ACE_Timer_Hash_Iterator_T const &) = delete;
  ACE_Timer_Hash_Iterator_T &operator= (ACE_Timer_Hash_Iterator_T &&) = delete;

  ~ACE_Timer_Hash_Iterator_T () override;

  /// Positions the iterator at the earliest node in the Timer Queue
  void first () override;

  /// Positions the iterator at the next node in the Timer Queue
  void next () override;

  /// Returns true when there are no more nodes in the sequence
  bool isdone () const override;

  /// Returns the node at the current position in the sequence
  ACE_Timer_Node_T<TYPE> *item () override;

protected:
  /// Pointer to the ACE_Timer_Hash that we are iterating over.
  Hash & timer_hash_;

  /// Current position in <timer_hash_>'s table
  size_t position_ {};

  /// Current iterator used on <position>'s bucket
  ACE_Timer_Queue_Iterator_T<TYPE> *iter_ {};
};

/**
 * @class ACE_Timer_Hash_T
 *
 * @brief Provides a hash table of BUCKETs as an implementation for
 * a timer queue.
 *
 * This implementation uses a hash table of BUCKETs.  The hash
 * is based on the time_value of the event.  Unlike other Timer
 * Queues, ACE_Timer_Hash does not expire events in strict order,
 * i.e., all events are expired after their deadline.  But two events
 * may expired out of order as defined by their deadlines.
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY = ACE_Default_Time_Policy>
class ACE_Timer_Hash_T : public ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>
{
public:
  /// Type of iterator
  using HASH_ITERATOR = ACE_Timer_Hash_Iterator_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>;

  /// Iterator is a friend
  friend class ACE_Timer_Hash_Iterator_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>;

  /// Type inherited from
  using Base_Timer_Queue = ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>;

  /**
   * Default constructor. @a table_size determines the size of the
   * hash table.  @a upcall_functor is the instance of the FUNCTOR
   * to be used by the buckets. If @a upcall_functor is nullptr, a default
   * FUNCTOR will be created.
   */
  explicit ACE_Timer_Hash_T (size_t table_size,
                             FUNCTOR *upcall_functor = nullptr,
                             ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist = nullptr,
                             TIME_POLICY const &time_policy = TIME_POLICY());

  /**
   * Default constructor. @a upcall_functor is the instance of the
   * FUNCTOR to be used by the queue. If @a upcall_functor is nullptr, Timer
   * Hash will create a default FUNCTOR.  @a freelist the freelist of
   * timer nodes.  If nullptr, then a default freelist will be created. The default
   * size will be ACE_DEFAULT_TIMERS and there will be no preallocation.
   */
  explicit ACE_Timer_Hash_T (FUNCTOR *upcall_functor = nullptr,
                    ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist = nullptr,
                    TIME_POLICY const &time_policy = TIME_POLICY());

  ACE_Timer_Hash_T (ACE_Timer_Hash_T const &) = delete;
  ACE_Timer_Hash_T (ACE_Timer_Hash_T &&) = delete;
  ACE_Timer_Hash_T &operator= (ACE_Timer_Hash_T const &) = delete;
  ACE_Timer_Hash_T &operator= (ACE_Timer_Hash_T &&) = delete;

  /// Destructor
  ~ACE_Timer_Hash_T () override;

  /// True if queue is empty, else false.
  bool is_empty () const override;

  /// Returns the time of the earlier node in the <ACE_Timer_Hash>.
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
   * Cancel all timer associated with @a type.  If <dont_call> is 0
   * then the <functor> will be invoked.  Returns number of timers
   * cancelled. If any valid timer is not cancelled before destruction
   * of this instance of ACE_Timer_Hash_T then user will get a memory
   * leak.
   */
  int cancel (TYPE const &type,
              int dont_call_handle_close = 1) override;

  /**
   * Cancel the single timer that matches the @a timer_id value (which
   * was returned from the <schedule> method).  If act is non-NULL
   * then it will be set to point to the ``magic cookie'' argument
   * passed in when the timer was registered.  This makes it possible
   * to free up the memory and avoid memory leaks.  If
   * @a dont_call_handle_close is 0 then the <functor> will be invoked.
   * Returns 1 if cancellation succeeded and 0 if the @a timer_id wasn't
   * found.  If any valid timer is not cancelled before destruction of
   * this instance of ACE_Timer_Hash_T then user will get a memory leak.
   */
  int cancel (long timer_id,
              void const **act = nullptr,
              int dont_call_handle_close = 1) override;

  /**
   * Destroy timer queue. Cancels all timers.
   */
  int close () override;

  /**
   * Run the <functor> for all timers whose values are <=
   * gettimeofday.  Also accounts for <timer_skew>.  Returns
   * the number of timers canceled.
   */
  int expire () override;

  /**
   * Run the <functor> for all timers whose values are <= @a current_time.
   * This does not account for <timer_skew>.  Returns the number of
   * timers canceled.
   */
  int expire (ACE_Time_Value const &current_time) override;

  /// Returns a pointer to this ACE_Timer_Queue's iterator.
  ACE_Timer_Queue_Iterator_T<TYPE> &iter () override;

  /// Removes the earliest node from the queue and returns it
  ACE_Timer_Node_T<TYPE> *remove_first () override;

  /// Dump the state of an object.
  void dump () const override;

  /// Reads the earliest node from the queue and returns it.
  ACE_Timer_Node_T<TYPE> *get_first () override;

protected:
  /// Factory method that frees a previously allocated node.
  void free_node (ACE_Timer_Node_T<TYPE> *) override;

private:
  /**
   * Schedule @a type that will expire at @a future_time,
   * which is specified in absolute time.  If it expires then @a act is
   * passed in as the value to the <functor>.  If @a interval is != to
   * ACE_Time_Value::zero then it is used to reschedule the @a type
   * automatically, using relative time to the current <gettimeofday>.
   * This method returns a <timer_id> that is a pointer to a token
   * which stores information about the event. This <timer_id> can be
   * used to cancel the timer before it expires.  Returns -1 on
   * failure.
   */
  long schedule_i (TYPE const &type,
                   void const *act,
                   ACE_Time_Value const &future_time,
                   ACE_Time_Value const &interval) override;

  /// Non-locking version of dispatch_info ()
  int dispatch_info_i (ACE_Time_Value const &current_time,
                       ACE_Timer_Node_Dispatch_Info_T<TYPE> &info) override;

  /// Reschedule an "interval" ACE_Timer_Node.
  void reschedule (ACE_Timer_Node_T<TYPE> *) override;

  /// Finds the earliest node
  void find_new_earliest ();

  /// Keeps track of the size of the queue
  size_t size_;

  /// Table of BUCKETS
  BUCKET **table_ {};

  /// Keeps track of the size of table_
  size_t table_size_;

  /// Functor used for the table's timer queues
  ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, ACE_LOCK> table_functor_;

  /// Index to the position with the earliest entry
  size_t earliest_position_;

  /// Iterator used to expire timers.
  HASH_ITERATOR *iterator_ {};

#if defined (ACE_WIN64)
  /// Part of a hack... see comments in schedule().
  /// This is, essentially, the upper 32 bits of a 64-bit pointer on Win64.
  ptrdiff_t pointer_base_;
#endif

  /// Hash_Token is usually allocated in schedule but its
  /// deallocation is problematic and token_list_ helps with this.
  ACE_Locked_Free_List<Hash_Token<TYPE>, ACE_Null_Mutex> token_list_;

};

ACE_END_VERSIONED_NAMESPACE_DECL

#include "ace/Timer_Hash_T.cpp"

#include /**/ "ace/post.h"
#endif /* ACE_TIMER_HASH_T_H */
