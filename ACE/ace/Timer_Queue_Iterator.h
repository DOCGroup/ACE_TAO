#ifndef ACE_TIMER_QUEUE_ITERATOR_H
#define ACE_TIMER_QUEUE_ITERATOR_H

#include /**/ "ace/pre.h"

/**
 * @file Timer_Queue_Iterator.h
 *
 * Re-factored from Timer_Queue_T.h
 */

#include "ace/Time_Value.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_Timer_Node_Dispatch_Info_T
 *
 * @brief Maintains generated dispatch information for Timer nodes.
 */
template <class TYPE>
class ACE_Timer_Node_Dispatch_Info_T
{
public:
  /// The type of object held in the queue
  TYPE type_ {};

  /// Asynchronous completion token associated with the timer.
  void const *act_ {};

  /// Flag to check if the timer is recurring.
  int recurring_timer_ {};
};

/**
 * @class ACE_Timer_Node_T
 *
 * @brief Maintains the state associated with a Timer entry.
 */
template <class TYPE>
class ACE_Timer_Node_T
{
public:
  /// Default constructor
  ACE_Timer_Node_T ();

  ACE_Timer_Node_T (ACE_Timer_Node_T const &) = delete;
  ACE_Timer_Node_T (ACE_Timer_Node_T &&) = delete;
  ACE_Timer_Node_T &operator= (ACE_Timer_Node_T const &) = delete;
  ACE_Timer_Node_T &operator= (ACE_Timer_Node_T &&) = delete;

  /// Destructor
  ~ACE_Timer_Node_T ();

  /// Useful typedef ..
  using DISPATCH_INFO = ACE_Timer_Node_Dispatch_Info_T<TYPE>;

  /// Singly linked list
  void set (TYPE const &type,
            void const *a,
            ACE_Time_Value const &t,
            ACE_Time_Value const &i,
            ACE_Timer_Node_T<TYPE> *n,
            long timer_id);

  /// Doubly linked list version
  void set (TYPE const &type,
            void const *a,
            ACE_Time_Value const &t,
            ACE_Time_Value const &i,
            ACE_Timer_Node_T<TYPE> *p,
            ACE_Timer_Node_T<TYPE> *n,
            long timer_id);

  // = Accessors

  /// Get the type.
  TYPE &get_type ();

  /// Set the type.
  void set_type (TYPE const &type);

  /// Get the asynchronous completion token.
  void const *get_act () const;

  /// Set the asynchronous completion token.
  void set_act (void const *act);

  /// Get the timer value.
  ACE_Time_Value const &get_timer_value () const;

  /// Set the timer value.
  void set_timer_value (ACE_Time_Value const &timer_value);

  /// Get the timer interval.
  ACE_Time_Value const &get_interval () const;

  /// Set the timer interval.
  void set_interval (ACE_Time_Value const &interval);

  /// Get the previous pointer.
  ACE_Timer_Node_T<TYPE> *get_prev () const;

  /// Set the previous pointer.
  void set_prev (ACE_Timer_Node_T<TYPE> *prev);

  /// Get the next pointer.
  ACE_Timer_Node_T<TYPE> *get_next () const;

  /// Set the next pointer.
  void set_next (ACE_Timer_Node_T<TYPE> *next);

  /// Get the timer_id.
  long get_timer_id () const;

  /// Set the timer_id.
  void set_timer_id (long timer_id);

  /// Get the dispatch info. The dispatch information is got
  /// through @a info. This form helps us in preventing allocation and
  /// deleting data along the criticl path.
  /// @todo We may want to have a copying version too, so that our
  /// interface will be complete..
  void get_dispatch_info (ACE_Timer_Node_Dispatch_Info_T <TYPE> &info) const;

  /// Dump the state of an TYPE.
  void dump () const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Type of object stored in the Queue
  TYPE type_ {};

  /// Asynchronous completion token associated with the timer.
  void const *act_ {};

  /// Time until the timer expires.
  ACE_Time_Value timer_value_ {};

  /// If this is a periodic timer this holds the time until the next
  /// timeout.
  ACE_Time_Value interval_ {};

  /// Pointer to previous timer.
  ACE_Timer_Node_T<TYPE> *prev_ {};

  /// Pointer to next timer.
  ACE_Timer_Node_T<TYPE> *next_ {};

  /// Id of this timer (used to cancel timers before they expire).
  long timer_id_ {-1};
};

/**
 * @class ACE_Timer_Queue_Iterator_T
 *
 * @brief Generic interface for iterating over a subclass of
 * ACE_Timer_Queue.
 *
 * This is a generic iterator that can be used to visit every
 * node of a timer queue.  Be aware that it isn't guaranteed
 * that the transversal will be in order of timeout values.
 */
template <class TYPE>
class ACE_Timer_Queue_Iterator_T
{
public:
  /// Constructor.
  ACE_Timer_Queue_Iterator_T () = default;

  ACE_Timer_Queue_Iterator_T (ACE_Timer_Queue_Iterator_T const &) = delete;
  ACE_Timer_Queue_Iterator_T (ACE_Timer_Queue_Iterator_T &&) = delete;
  ACE_Timer_Queue_Iterator_T &operator= (ACE_Timer_Queue_Iterator_T const &) = delete;
  ACE_Timer_Queue_Iterator_T &operator= (ACE_Timer_Queue_Iterator_T &&) = delete;

  /// Destructor.
  virtual ~ACE_Timer_Queue_Iterator_T ();

  /// Positions the iterator at the earliest node in the Timer Queue
  virtual void first () = 0;

  /// Positions the iterator at the next node in the Timer Queue
  virtual void next () = 0;

  /// Returns true when there are no more nodes in the sequence
  virtual bool isdone () const = 0;

  /// Returns the node at the current position in the sequence
  virtual ACE_Timer_Node_T<TYPE> *item () = 0;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/Timer_Queue_Iterator.inl"
#endif /* __ACE_INLINE__ */

#include "ace/Timer_Queue_Iterator.cpp"

#include /**/ "ace/post.h"
#endif /* ACE_TIMER_QUEUE_ITERATOR_H */
