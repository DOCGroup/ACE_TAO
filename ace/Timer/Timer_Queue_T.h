/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Timer_Queue_T.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt <schmidt@cs.wustl.edu>
 *  @author Irfan Pyarali <irfan@cs.wustl.edu> and
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_TIMER_QUEUE_T_H
#define ACE_TIMER_QUEUE_T_H
#include "ace/pre.h"

#include "ace/Utils/Templates/Free_List.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Utils/Test_and_Set.h"

/**
 * @class ACE_Timer_Node_Dispatch_Info_T
 *
 * @brief Maintains generated dispatch information for Timer nodes.
 *
 */
template <class TYPE>
class ACE_Timer_Node_Dispatch_Info_T
{
public:
  // The type of object held in the queue
  TYPE type_;

  /// Asynchronous completion token associated with the timer.
  const void *act_;
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
  ACE_Timer_Node_T (void);

  /// Dtor.
  ~ACE_Timer_Node_T (void);

  /// Useful typedef ..
  typedef ACE_Timer_Node_Dispatch_Info_T <TYPE> DISPATCH_INFO;

  /// singly linked list
  void set (const TYPE &type,
            const void *a,
            const ACE_Time_Value &t,
            const ACE_Time_Value &i,
            ACE_Timer_Node_T<TYPE> *n,
            long timer_id);

  /// doubly linked list version
  void set (const TYPE &type,
            const void *a,
            const ACE_Time_Value &t,
            const ACE_Time_Value &i,
            ACE_Timer_Node_T<TYPE> *p,
            ACE_Timer_Node_T<TYPE> *n,
            long timer_id);

  // = Accessors

  /// Get the type.
  TYPE &get_type (void);

  /// Set the type.
  void set_type (TYPE &type);

  /// Get the asynchronous completion token.
  const void *get_act (void);

  /// set the asynchronous completion token.
  void set_act (void *act);

  /// get the timer value.
  const ACE_Time_Value &get_timer_value (void) const;

  /// set the timer value.
  void set_timer_value (const ACE_Time_Value &timer_value);

  /// get the timer interval.
  const ACE_Time_Value &get_interval (void) const;

  /// Set the timer interval.
  void set_interval (const ACE_Time_Value &interval);

  /// get the previous pointer.
  ACE_Timer_Node_T<TYPE> *get_prev (void);

  /// set the previous pointer.
  void set_prev (ACE_Timer_Node_T<TYPE> *prev);

  /// get the next pointer.
  ACE_Timer_Node_T<TYPE> *get_next (void);

  /// set the next pointer.
  void set_next (ACE_Timer_Node_T<TYPE> *next);

  /// get the timer_id.
  long get_timer_id (void) const;

  /// set the timer_id.
  void set_timer_id (long timer_id);

  /// Get the dispatch info. The dispatch information is got
  /// through <info>. This form helps us in preventing allocation and
  /// deleting data along the criticl path.
  /// @@TODO: We may want to have a copying version too, so that our
  /// interface will be complete..
  void get_dispatch_info (ACE_Timer_Node_Dispatch_Info_T <TYPE> &info);


  /// Dump the state of an TYPE.
  void dump (void) const;

private:
  /// Type of object stored in the Queue
  TYPE type_;

  /// Asynchronous completion token associated with the timer.
  const void *act_;

  /// Time until the timer expires.
  ACE_Time_Value timer_value_;

  /// If this is a periodic timer this holds the time until the next
  /// timeout.
  ACE_Time_Value interval_;

  /// Pointer to previous timer.
  ACE_Timer_Node_T<TYPE> *prev_;

  /// Pointer to next timer.
  ACE_Timer_Node_T<TYPE> *next_;

  /// Id of this timer (used to cancel timers before they expire).
  long timer_id_;
};

/**
 * @class ACE_Timer_Queue_Iterator_T
 *
 * @brief Generic interface for iterating over a subclass of
 * <ACE_Timer_Queue>.
 *
 * This is a generic iterator that can be used to visit every
 * node of a timer queue.  Be aware that it isn't guaranteed
 * that the transversal will be in order of timeout values.
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK>
class ACE_Timer_Queue_Iterator_T
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  ACE_Timer_Queue_Iterator_T (void);

  /// Destructor.
  virtual ~ACE_Timer_Queue_Iterator_T (void);

  /// Positions the iterator at the earliest node in the Timer Queue
  virtual void first (void) = 0;

  /// Positions the iterator at the next node in the Timer Queue
  virtual void next (void) = 0;

  /// Returns true when there are no more nodes in the sequence
  virtual int isdone (void) const = 0;

  /// Returns the node at the current position in the sequence
  virtual ACE_Timer_Node_T<TYPE> *item (void) = 0;
};

/**
 * @class ACE_Timer_Queue_T
 *
 * @brief Provides an interface to timers.
 *
 * This is an abstract base class that provides hook for
 * implementing specialized policies such as <ACE_Timer_List>
 * and <ACE_Timer_Heap>.
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK>
class ACE_Timer_Queue_T
{
public:
  /// Type of Iterator.
  typedef ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR, ACE_LOCK> ITERATOR;

  // = Initialization and termination methods.
  /**
   * Default constructor. <upcall_functor> is the instance of the
   * FUNCTOR to be used by the queue. If <upcall_functor> is 0, Timer
   * Queue will create a default FUNCTOR.  <freelist> the freelist of
   * timer nodes.  If 0, then a default freelist will be created.
   */
  ACE_Timer_Queue_T (FUNCTOR *upcall_functor = 0,
                     ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist = 0);

  /// Destructor - make virtual for proper destruction of inherited
  /// classes.
  virtual ~ACE_Timer_Queue_T (void);

  /// True if queue is empty, else false.
  virtual int is_empty (void) const = 0;

  /// Returns the time of the earlier node in the Timer_Queue.  Must
  /// be called on a non-empty queue.
  virtual const ACE_Time_Value &earliest_time (void) const = 0;

  /**
   * Schedule <type> that will expire after <delay> amount of time,
   * which is specified in absolute time.  If it expires then <act> is
   * passed in as the value to the <functor>.  If <interval> is != to
   * <ACE_Time_Value::zero> then it is used to reschedule the <type>
   * automatically, using relative time to the current <gettimeofday>.
   * This method returns a <timer_id> that uniquely identifies the the
   * <type> entry in an internal list.  This <timer_id> can be used to
   * cancel the timer before it expires.  The cancellation ensures
   * that <timer_ids> are unique up to values of greater than 2
   * billion timers.  As long as timers don't stay around longer than
   * this there should be no problems with accidentally deleting the
   * wrong timer.  Returns -1 on failure (which is guaranteed never to
   * be a valid <timer_id>).
   */
  virtual long schedule (const TYPE &type,
                         const void *act,
                         const ACE_Time_Value &delay,
                         const ACE_Time_Value &interval = ACE_Time_Value::zero) = 0;

  /**
   * Resets the interval of the timer represented by <timer_id> to
   * <interval>, which is specified in relative time to the current
   * <gettimeofday>.  If <interval> is equal to
   * <ACE_Time_Value::zero>, the timer will become a non-rescheduling
   * timer.  Returns 0 if successful, -1 if not.
   */
  virtual int reset_interval (long timer_id,
                              const ACE_Time_Value &interval) = 0;

  /**
   * Cancel all timer associated with <type>.  If
   * <dont_call_handle_close> is 0 then the <functor> will be invoked,
   * which typically invokes the <handle_close> hook.  Returns number
   * of timers cancelled.
   */
  virtual int cancel (const TYPE &type,
                      int dont_call_handle_close = 1) = 0;

  /**
   * Cancel the single timer that matches the <timer_id> value (which
   * was returned from the <schedule> method).  If act is non-NULL
   * then it will be set to point to the ``magic cookie'' argument
   * passed in when the timer was registered.  This makes it possible
   * to free up the memory and avoid memory leaks.  If
   * <dont_call_handle_close> is 0 then the <functor> will be invoked,
   * which typically calls the <handle_close> hook.  Returns 1 if
   * cancellation succeeded and 0 if the <timer_id> wasn't found.
   */
  virtual int cancel (long timer_id,
                      const void **act = 0,
                      int dont_call_handle_close = 1) = 0;

  /**
   * Run the <functor> for all timers whose values are <= <cur_time>.
   * This does not account for <timer_skew>.  Returns the number of
   * timers canceled.
   */
  virtual int expire (const ACE_Time_Value &current_time);

  /**
   * Get the dispatch information for a timer whose value is <= <cur_time>.
   * This does not account for <timer_skew>. Returns 1 if
   * there is a node whose value <= <cur_time> else returns a 0.
   *
   */
  int dispatch_info (const ACE_Time_Value &current_time,
                     ACE_Timer_Node_Dispatch_Info_T<TYPE> &info);

  /**
   * Run the <functor> for all timers whose values are <=
   * <ACE_OS::gettimeofday>.  Also accounts for <timer_skew>.
   *
   * Depending on the resolution of the underlying OS the system calls
   * like select()/poll() might return at time different than that is
   * specified in the timeout. Suppose the OS guarantees a resolution of t ms.
   * The timeline will look like
   *
   *             A                   B
   *             |                   |
   *             V                   V
   *  |-------------|-------------|-------------|-------------|
   *  t             t             t             t             t
   *
   *
   * If you specify a timeout value of A, then the timeout will not occur
   * at A but at the next interval of the timer, which is later than
   * that is expected. Similarly, if your timeout value is equal to B,
   * then the timeout will occur at interval after B. Now depending upon the
   * resolution of your timeouts and the accuracy of the timeouts
   * needed for your application, you should set the value of
   * <timer_skew>. In the above case, if you want the timeout A to fire
   * no later than A, then you should specify your <timer_skew> to be
   * A % t.
   *
   * The timeout value should be specified via the macro ACE_TIMER_SKEW
   * in your config.h file. The default value is zero.
   *
   * Things get interesting if the t before the timeout value B is zero
   * i.e your timeout is less than the interval. In that case, you are
   * almost sure of not getting the desired timeout behaviour. Maybe you
   * should look for a better OS :-)
   *
   *  Returns the number of timers canceled.
   */

  /* virtual */ int expire (void);

  /**
   * Returns the current time of day.  This method allows different
   * implementations of the timer queue to use special high resolution
   * timers.
   */
  /* virtual */ ACE_Time_Value gettimeofday (void);

  /// Allows applications to control how the timer queue gets the time
  /// of day.
  void gettimeofday (ACE_Time_Value (*gettimeofday)(void));

  /// Determine the next event to timeout.  Returns <max> if there are
  /// no pending timers or if all pending timers are longer than max.
  virtual ACE_Time_Value *calculate_timeout (ACE_Time_Value *max);

  /**
   * Determine the next event to timeout.  Returns <max> if there are
   * no pending timers or if all pending timers are longer than max.
   * <the_timeout> should be a pointer to storage for the timeout value,
   * and this value is also returned.
   */
  virtual ACE_Time_Value *calculate_timeout (ACE_Time_Value *max,
                                             ACE_Time_Value *the_timeout);

  // = Set/get the timer skew for the Timer_Queue.
  void timer_skew (const ACE_Time_Value &skew);
  const ACE_Time_Value &timer_skew (void) const;

  /// Synchronization variable used by the queue
  ACE_LOCK &mutex (void);

  /// Accessor to the upcall functor
  FUNCTOR &upcall_functor (void);

  /// Returns a pointer to this <ACE_Timer_Queue>'s iterator.
  virtual ITERATOR &iter (void) = 0;

  /// Removes the earliest node from the queue and returns it
  virtual ACE_Timer_Node_T<TYPE> *remove_first (void) = 0;

  /// Dump the state of a object.
  virtual void dump (void) const;

  /// Reads the earliest node from the queue and returns it.
  virtual ACE_Timer_Node_T<TYPE> *get_first (void) = 0;

  /// Method used to return a timer node to the queue's ownership
  /// after it is returned by a method like <remove_first>.
  virtual void return_node (ACE_Timer_Node_T<TYPE> *);


  /// This method will call the <functor> with the <type>, <act> and
  /// <time>
  /* virtual */ void upcall (TYPE &type,
                       const void *act,
                       const ACE_Time_Value &cur_time);

protected:
  /// Reschedule an "interval" <ACE_Timer_Node>.
  virtual void reschedule (ACE_Timer_Node_T<TYPE> *) = 0;

  /// Factory method that allocates a new node.
  virtual ACE_Timer_Node_T<TYPE> *alloc_node (void);

  /// Factory method that frees a previously allocated node.
  virtual void free_node (ACE_Timer_Node_T<TYPE> *);

  /// Non-locking version of dispatch_info ()
  int dispatch_info_i (const ACE_Time_Value &current_time,
                       ACE_Timer_Node_Dispatch_Info_T<TYPE> &info);

  /// Synchronization variable for <ACE_Timer_Queue>.
  /// NOTE: the right name would be lock_, but HP/C++ will choke on that!
  ACE_LOCK mutex_;

  /// Class that implements a free list
  ACE_Free_List<ACE_Timer_Node_T<TYPE> > *free_list_;

  /// Pointer to function that returns the current time of day.
  ACE_Time_Value (*gettimeofday_)(void);

  /// Upcall functor
  FUNCTOR *upcall_functor_;

  /// To delete or not to delete is the question?
  int delete_upcall_functor_;

  /// Flag to delete only if the class created the <free_list_>
  int delete_free_list_;

private:

  /// Returned by <calculate_timeout>.
  ACE_Time_Value timeout_;

  /// Adjusts for timer skew in various clocks.
  ACE_Time_Value timer_skew_;

  // = Don't allow these operations for now.
  ACE_UNIMPLEMENTED_FUNC (ACE_Timer_Queue_T (const ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK> &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK> &))
};

/**
 * @class ACE_Event_Handler_Handle_Timeout_Upcall
 *
 * @brief Functor for Timer_Queues.
 *
 * This class implements the functor required by the Timer
 * Queue to call <handle_timeout> on ACE_Event_Handlers.
 */
template <class ACE_LOCK>
class ACE_Event_Handler_Handle_Timeout_Upcall
{
public:
  typedef ACE_Timer_Queue_T<ACE_Event_Handler *,
                            ACE_Event_Handler_Handle_Timeout_Upcall<ACE_LOCK>,
                            ACE_LOCK>
          TIMER_QUEUE;

  // = Initialization and termination methods.
  /// Constructor.
  ACE_Event_Handler_Handle_Timeout_Upcall (void);

  /// Destructor.
  ~ACE_Event_Handler_Handle_Timeout_Upcall (void);

  /// This method is called when the timer expires
  int timeout (TIMER_QUEUE &timer_queue,
               ACE_Event_Handler *handler,
               const void *arg,
               const ACE_Time_Value &cur_time);

  /// This method is called when the timer is canceled
  int cancellation (TIMER_QUEUE &timer_queue,
                    ACE_Event_Handler *handler);

  /// This method is called when the timer queue is destroyed and
  /// the timer is still contained in it
  int deletion (TIMER_QUEUE &timer_queue,
                ACE_Event_Handler *handler,
                const void *arg);
private:
  // = Don't allow these operations for now.
  ACE_UNIMPLEMENTED_FUNC (ACE_Event_Handler_Handle_Timeout_Upcall (const ACE_Event_Handler_Handle_Timeout_Upcall<ACE_LOCK> &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Event_Handler_Handle_Timeout_Upcall<ACE_LOCK> &))
};

#if defined (__ACE_INLINE__)
#include "ace/Timer/Timer_Queue_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE) && !defined (ACE_HAS_BROKEN_HPUX_TEMPLATES)
#include "ace/Timer/Timer_Queue_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE && !ACE_HAS_BROKEN_HPUX_TEMPLATES */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Timer_Queue_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* ACE_TIMER_QUEUE_T_H */
