// $Id$

#ifndef ACE_TIMER_QUEUE_T_C
#define ACE_TIMER_QUEUE_T_C

#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Signal.h"
#include "ace/Timer_Queue_T.h"
#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
#include "ace/Timer_Queue_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Timer_Queue_T, "$Id$")

// This fudge factor can be overriden for timers that need it, such as on
// Solaris, by defining the ACE_TIMER_SKEW symbol in the appropriate config
// header.
#if !defined (ACE_TIMER_SKEW)
#  define ACE_TIMER_SKEW 0
#endif /* ACE_TIMER_SKEW */

template <class TYPE> void
ACE_Timer_Node_T<TYPE>::dump (void) const
{
  ACE_TRACE ("ACE_Timer_Node_T::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nact_ = %x"), this->act_));
  this->timer_value_.dump ();
  this->interval_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nprev_ = %x"), this->prev_));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nnext_ = %x"), this->next_));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\ntimer_id_ = %d\n"), this->timer_id_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class TYPE>
ACE_Timer_Node_T<TYPE>::ACE_Timer_Node_T (void)
{
  ACE_TRACE ("ACE_Timer_Node_T::ACE_Timer_Node_T");
}

template <class TYPE>
ACE_Timer_Node_T<TYPE>::~ACE_Timer_Node_T (void)
{
  ACE_TRACE ("ACE_Timer_Node_T::~ACE_Timer_Node_T");
}



template <class TYPE, class FUNCTOR, class ACE_LOCK>
ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR, ACE_LOCK>::ACE_Timer_Queue_Iterator_T (void)
{
}

template <class TYPE, class FUNCTOR, class ACE_LOCK>
ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR, ACE_LOCK>::~ACE_Timer_Queue_Iterator_T (void)
{
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_Time_Value *
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::calculate_timeout (ACE_Time_Value *max_wait_time)
{
  ACE_TRACE ("ACE_Timer_Queue_T::calculate_timeout");
  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, max_wait_time));

  if (this->is_empty ())
    // Nothing on the Timer_Queue, so use whatever the caller gave us.
    return max_wait_time;
  else
    {
      ACE_Time_Value cur_time = this->gettimeofday ();

      if (this->earliest_time () > cur_time)
        {
          // The earliest item on the Timer_Queue is still in the
          // future.  Therefore, use the smaller of (1) caller's wait
          // time or (2) the delta time between now and the earliest
          // time on the Timer_Queue.

          this->timeout_ = this->earliest_time () - cur_time;
          if (max_wait_time == 0 || *max_wait_time > timeout_)
            return &this->timeout_;
          else
            return max_wait_time;
        }
      else
        {
          // The earliest item on the Timer_Queue is now in the past.
          // Therefore, we've got to "poll" the Reactor, i.e., it must
          // just check the descriptors and then dispatch timers, etc.
          this->timeout_ = ACE_Time_Value::zero;
          return &this->timeout_;
        }
    }
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_Time_Value *
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::calculate_timeout (ACE_Time_Value *max_wait_time,
                                                               ACE_Time_Value *the_timeout)
{
  ACE_TRACE ("ACE_Timer_Queue_T::calculate_timeout");

  if (the_timeout == 0)
    return 0;

  if (this->is_empty ())
    {
      // Nothing on the Timer_Queue, so use whatever the caller gave us.
      if (max_wait_time)
        *the_timeout = *max_wait_time;
      else
        return 0;
    }
  else
    {
      ACE_Time_Value cur_time = this->gettimeofday ();

      if (this->earliest_time () > cur_time)
        {
          // The earliest item on the Timer_Queue is still in the
          // future.  Therefore, use the smaller of (1) caller's wait
          // time or (2) the delta time between now and the earliest
          // time on the Timer_Queue.

          *the_timeout = this->earliest_time () - cur_time;
          if (!(max_wait_time == 0 || *max_wait_time > *the_timeout))
            *the_timeout = *max_wait_time;
        }
      else
        {
          // The earliest item on the Timer_Queue is now in the past.
          // Therefore, we've got to "poll" the Reactor, i.e., it must
          // just check the descriptors and then dispatch timers, etc.
          *the_timeout = ACE_Time_Value::zero;
        }
    }
  return the_timeout;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> void
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::dump (void) const
{
  ACE_TRACE ("ACE_Timer_Queue_T::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->timeout_.dump ();
  this->timer_skew_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class TYPE, class FUNCTOR, class ACE_LOCK>
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::ACE_Timer_Queue_T (FUNCTOR *upcall_functor,
                                                           ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist)
  : gettimeofday_ (ACE_OS::gettimeofday),
    delete_upcall_functor_ (upcall_functor == 0),
    delete_free_list_ (freelist == 0),
    timer_skew_ (0, ACE_TIMER_SKEW)
{
  ACE_TRACE ("ACE_Timer_Queue_T::ACE_Timer_Queue_T");

  if (!freelist)
    ACE_NEW (free_list_,
             (ACE_Locked_Free_List<ACE_Timer_Node_T<TYPE>,ACE_Null_Mutex>));
  else
    free_list_ = freelist;

  if (!upcall_functor)
    ACE_NEW (upcall_functor_,
             FUNCTOR);
  else
    upcall_functor_ = upcall_functor;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK>
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::~ACE_Timer_Queue_T (void)
{
  ACE_TRACE ("ACE_Timer_Queue_T::~ACE_Timer_Queue_T");

  // Cleanup the functor and free_list on the way out
  if (this->delete_upcall_functor_)
    delete this->upcall_functor_;

  if (this->delete_free_list_)
    delete this->free_list_;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_Timer_Node_T<TYPE> *
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::alloc_node (void)
{
  return this->free_list_->remove ();
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> void
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::free_node (ACE_Timer_Node_T<TYPE> *node)
{
  this->free_list_->add (node);
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_LOCK &
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::mutex (void)
{
  return this->mutex_;
}


// Run the <handle_timeout> method for all Timers whose values are <=
// <cur_time>.
template <class TYPE, class FUNCTOR, class ACE_LOCK> int
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::expire (const ACE_Time_Value &cur_time)
{
  ACE_TRACE ("ACE_Timer_Queue_T::expire");
  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));

  // Keep looping while there are timers remaining and the earliest
  // timer is <= the <cur_time> passed in to the method.

  if (this->is_empty ())
    return 0;

  int number_of_timers_expired = 0;
  int result = 0;

  ACE_Timer_Node_Dispatch_Info_T<TYPE> info;

  while ((result = this->dispatch_info_i (cur_time,
                                          info)) != 0)
    {
      // call the functor
      this->upcall (info.type_, info.act_, cur_time);

      number_of_timers_expired++;

    }

  ACE_UNUSED_ARG (result);
  return number_of_timers_expired;
}


template <class TYPE, class FUNCTOR, class ACE_LOCK> int
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::dispatch_info_i (const ACE_Time_Value &cur_time,
                                                             ACE_Timer_Node_Dispatch_Info_T<TYPE> &info)
{
  ACE_TRACE ("ACE_Timer_Queue_T::dispatch_info_i");

  if (this->is_empty ())
    return 0;

  ACE_Timer_Node_T<TYPE> *expired = 0;

  if (this->earliest_time () <= cur_time)
    {
      expired = this->remove_first ();

      // Get the dispatch info
      expired->get_dispatch_info (info);

      // Check if this is an interval timer.
      if (expired->get_interval () > ACE_Time_Value::zero)
        {
          // Make sure that we skip past values that have already
          // "expired".
          do
            expired->set_timer_value (expired->get_timer_value () + expired->get_interval ());
          while (expired->get_timer_value () <= cur_time);

          // Since this is an interval timer, we need to reschedule
          // it.
          this->reschedule (expired);
        }
      else
        {
          // Call the factory method to free up the node.
          this->free_node (expired);
        }

      return 1;
    }

  return 0;
}


template <class TYPE, class FUNCTOR, class ACE_LOCK> void
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::return_node (ACE_Timer_Node_T<TYPE> *node)
{
  ACE_MT (ACE_GUARD (ACE_LOCK, ace_mon, this->mutex_));
  this->free_node (node);
}


template <class ACE_LOCK>
ACE_Event_Handler_Handle_Timeout_Upcall<ACE_LOCK>::ACE_Event_Handler_Handle_Timeout_Upcall (void)
{
}

template <class ACE_LOCK>
ACE_Event_Handler_Handle_Timeout_Upcall<ACE_LOCK>::~ACE_Event_Handler_Handle_Timeout_Upcall (void)
{
}

template <class ACE_LOCK> int
ACE_Event_Handler_Handle_Timeout_Upcall<ACE_LOCK>::timeout (TIMER_QUEUE &timer_queue,
                                                            ACE_Event_Handler *handler,
                                                            const void *act,
                                                            const ACE_Time_Value &cur_time)
{
  // Upcall to the <handler>s handle_timeout() method.
  if (handler->handle_timeout (cur_time, act) == -1)
    {
#if 0
      // Commented out until we figure out how to break the coupling that results...
      if (handler->reactor ())
        // Call the reactor's cancel_timer() method to minimize locking.
        handler->reactor ()->cancel_timer (handler, 0); // 0 means "call handle_close()".
      else
#endif
        timer_queue.cancel (handler, 0); 
    }

  return 0;
}

template <class ACE_LOCK> int
ACE_Event_Handler_Handle_Timeout_Upcall<ACE_LOCK>::cancellation (TIMER_QUEUE &timer_queue,
                                                                 ACE_Event_Handler *handler)
{
  ACE_UNUSED_ARG (timer_queue);

  // Upcall to the <handler>s handle_close method
  handler->handle_close (ACE_INVALID_HANDLE,
                         ACE_Event_Handler::TIMER_MASK);
  return 0;
}

template <class ACE_LOCK> int
ACE_Event_Handler_Handle_Timeout_Upcall<ACE_LOCK>::deletion (TIMER_QUEUE &timer_queue,
                                                             ACE_Event_Handler *handler,
                                                             const void *arg)
{
  ACE_UNUSED_ARG (timer_queue);
  ACE_UNUSED_ARG (handler);
  ACE_UNUSED_ARG (arg);

  // Does nothing

  return 0;
}

#endif /* ACE_TIMER_QUEUE_T_C*/
