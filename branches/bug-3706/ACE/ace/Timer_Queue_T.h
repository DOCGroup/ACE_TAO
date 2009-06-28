// -*- C++ -*-

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
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Free_List.h"
#include "ace/Event_Handler.h"
#include "ace/Time_Value.h"
#include "ace/Abstract_Timer_Queue.h"
#include "ace/Timer_Queue_Iterator.h"
#include "ace/Copy_Disabled.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_Timer_Queue_T
 *
 * @brief Provides an interface to timers.
 *
 * This is an abstract base class that provides hook for
 * implementing specialized policies such as ACE_Timer_List
 * and ACE_Timer_Heap.
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK>
class ACE_Timer_Queue_T
  : public ACE_Abstract_Timer_Queue<TYPE>
  , private ACE_Copy_Disabled
{
public:
  // = Initialization and termination methods.
  /**
   * Default constructor. @a upcall_functor is the instance of the
   * FUNCTOR to be used by the queue. If @a upcall_functor is 0, Timer
   * Queue will create a default FUNCTOR.  @a freelist the freelist of
   * timer nodes.  If 0, then a default freelist will be created.
   */
  ACE_Timer_Queue_T (FUNCTOR *upcall_functor = 0,
                     ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist = 0);

  /// Destructor - make virtual for proper destruction of inherited
  /// classes.
  virtual ~ACE_Timer_Queue_T (void);

  /**
   * Implement ACE_Abstract_Timer_Queue<TYPE>::schedule () with the right
   * locking strategy.
   */
  virtual long schedule (const TYPE &type,
                         const void *act,
                         const ACE_Time_Value &future_time,
                         const ACE_Time_Value &interval = ACE_Time_Value::zero);

  //@{
  /**
   * Implement ACE_Abstract_Timer_Queue<TYPE>::expire () with the right
   * locking strategy.
   */
  virtual int expire (const ACE_Time_Value &current_time);
  virtual int expire (void);
  virtual int expire_single(ACE_Command_Base & pre_dispatch_command);
  //@}

  /**
   * Get the dispatch information for a timer whose value is <= @a current_time.
   * This does not account for <timer_skew>. Returns 1 if
   * there is a node whose value <= @a current_time else returns a 0.
   *
   */
  virtual int dispatch_info (const ACE_Time_Value &current_time,
                             ACE_Timer_Node_Dispatch_Info_T<TYPE> &info);



  //@{
  /**
   * Implement the gettimeofday() functions
   */
  virtual ACE_Time_Value gettimeofday (void);
  virtual void gettimeofday (ACE_Time_Value (*gettimeofday)(void));
  //@}

  //@{
  /**
   * Implement calculate_timeout() using the right locking policy
   */
  virtual ACE_Time_Value *calculate_timeout (ACE_Time_Value *max);
  virtual ACE_Time_Value *calculate_timeout (ACE_Time_Value *max,
                                             ACE_Time_Value *the_timeout);
  virtual ACE_Time_Value current_time();
  //@}

  /// Set the timer skew for the Timer_Queue.
  void timer_skew (const ACE_Time_Value &skew);

  /// Get the timer skew for the Timer_Queue.
  const ACE_Time_Value &timer_skew (void) const;

  /// Synchronization variable used by the queue
  ACE_LOCK &mutex (void);

  /// Accessor to the upcall functor
  FUNCTOR &upcall_functor (void);

  /// Dump the state of a object.
  virtual void dump (void) const;

  /// Removes the earliest node from the queue and returns it
  virtual ACE_Timer_Node_T<TYPE> *remove_first (void) = 0;

  /// Reads the earliest node from the queue and returns it.
  virtual ACE_Timer_Node_T<TYPE> *get_first (void) = 0;

  /// Method used to return a timer node to the queue's ownership
  /// after it is returned by a method like <remove_first>.
  virtual void return_node (ACE_Timer_Node_T<TYPE> *);

  /// This method will call the preinvoke() on <functor>.
  void preinvoke (ACE_Timer_Node_Dispatch_Info_T<TYPE> &info,
                  const ACE_Time_Value &cur_time,
                  const void *&upcall_act);

  /// This method will call the timeout() on <functor>.
  void upcall (ACE_Timer_Node_Dispatch_Info_T<TYPE> &info,
               const ACE_Time_Value &cur_time);

  /// This method will call the postinvoke() on <functor>.
  void postinvoke (ACE_Timer_Node_Dispatch_Info_T<TYPE> &info,
                   const ACE_Time_Value &cur_time,
                   const void *upcall_act);

protected:

  /// Schedule a timer.
  virtual long schedule_i (const TYPE &type,
                           const void *act,
                           const ACE_Time_Value &future_time,
                           const ACE_Time_Value &interval) = 0;

  /// Reschedule an "interval" ACE_Timer_Node.
  virtual void reschedule (ACE_Timer_Node_T<TYPE> *) = 0;

  /// Factory method that allocates a new node.
  virtual ACE_Timer_Node_T<TYPE> *alloc_node (void);

  /// Factory method that frees a previously allocated node.
  virtual void free_node (ACE_Timer_Node_T<TYPE> *);

  /// Non-locking version of dispatch_info ()
  virtual int dispatch_info_i (const ACE_Time_Value &current_time,
                               ACE_Timer_Node_Dispatch_Info_T<TYPE> &info);

  /// Synchronization variable for ACE_Timer_Queue.
  /// @note The right name would be lock_, but HP/C++ will choke on that!
  ACE_LOCK mutex_;

  /// Class that implements a free list
  ACE_Free_List<ACE_Timer_Node_T<TYPE> > *free_list_;

  /// Pointer to function that returns the current time of day.
  ACE_Time_Value (*gettimeofday_)(void);

  /// Upcall functor
  FUNCTOR *upcall_functor_;

  /// To delete or not to delete is the question?
  bool const delete_upcall_functor_;

  /// Flag to delete only if the class created the <free_list_>
  bool const delete_free_list_;

private:

  /// Returned by <calculate_timeout>.
  ACE_Time_Value timeout_;

  /// Adjusts for timer skew in various clocks.
  ACE_Time_Value timer_skew_;
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
  : private ACE_Copy_Disabled
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

  /// Flag indicating that reference counting is required for this
  /// event handler upcall.
  int requires_reference_counting_;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/Timer_Queue_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Timer_Queue_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Timer_Queue_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* ACE_TIMER_QUEUE_T_H */
