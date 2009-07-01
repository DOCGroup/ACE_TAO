// $Id$

#ifndef ACE_TIMER_QUEUE_T_CPP
#define ACE_TIMER_QUEUE_T_CPP

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/*
 * Hook to specialize to add includes
 */
//@@ REACTOR_SPL_INCLUDE_FORWARD_DECL_ADD_HOOK

#include "ace/Timer_Queue_T.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "ace/Null_Mutex.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Functor.h"

#if !defined (__ACE_INLINE__)
#include "ace/Timer_Queue_T.inl"
#endif /* __ACE_INLINE__ */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// This fudge factor can be overriden for timers that need it, such as on
// Solaris, by defining the ACE_TIMER_SKEW symbol in the appropriate config
// header.
#if !defined (ACE_TIMER_SKEW)
#  define ACE_TIMER_SKEW 0
#endif /* ACE_TIMER_SKEW */

template <class TYPE, class FUNCTOR> ACE_INLINE
ACE_Timer_Queue_Upcall_Base<TYPE, FUNCTOR>::ACE_Timer_Queue_Upcall_Base (FUNCTOR * upcall_functor)
  : ACE_Abstract_Timer_Queue<TYPE>()
  , ACE_Copy_Disabled()
  , upcall_functor_(upcall_functor)
  , delete_upcall_functor_ (upcall_functor == 0)
{
  ACE_TRACE ("ACE_Timer_Queue_Upcall_Base::ACE_Timer_Queue_Upcall_Base");

  if (upcall_functor != 0)
    {
      return;
    }

  ACE_NEW (upcall_functor_, FUNCTOR);
}

template <class TYPE, class FUNCTOR> ACE_INLINE
ACE_Timer_Queue_Upcall_Base<TYPE, FUNCTOR>::~ACE_Timer_Queue_Upcall_Base ()
{
  ACE_TRACE ("ACE_Timer_Queue_Upcall_Base::~ACE_Timer_Queue_Upcall_Base");
  if (this->delete_upcall_functor_)
    {
      delete this->upcall_functor_;
    }
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, typename TIME_POLICY> ACE_Time_Value
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>::gettimeofday_abstract()
{
  return this->gettimeofday();
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, typename TIME_POLICY> ACE_Time_Value *
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>::calculate_timeout (ACE_Time_Value *max_wait_time)
{
  ACE_TRACE ("ACE_Timer_Queue_T::calculate_timeout");
  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, max_wait_time));

  if (this->is_empty ())
    // Nothing on the Timer_Queue, so use whatever the caller gave us.
    return max_wait_time;
  else
    {
      ACE_Time_Value const cur_time = this->gettimeofday ();

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

template <class TYPE, class FUNCTOR, class ACE_LOCK, typename TIME_POLICY> ACE_Time_Value *
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>::calculate_timeout (ACE_Time_Value *max_wait_time,
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

template <class TYPE, class FUNCTOR, class ACE_LOCK, typename TIME_POLICY> ACE_Time_Value
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>::current_time()
{
  ACE_Time_Value tv = this->gettimeofday();
  tv += this->timer_skew();
  return tv;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, typename TIME_POLICY> void
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Timer_Queue_T::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->timeout_.dump ();
  this->timer_skew_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, typename TIME_POLICY>
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>::ACE_Timer_Queue_T (FUNCTOR *upcall_functor,
                                                               ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist,
									    TIME_POLICY const & time_policy)
  : ACE_Timer_Queue_Upcall_Base<TYPE,FUNCTOR>(upcall_functor),
    time_policy_ (time_policy),
    delete_free_list_ (freelist == 0),
    timer_skew_ (0, ACE_TIMER_SKEW)
{
  ACE_TRACE ("ACE_Timer_Queue_T::ACE_Timer_Queue_T");

  if (!freelist)
    ACE_NEW (free_list_,
             (ACE_Locked_Free_List<ACE_Timer_Node_T<TYPE>,ACE_Null_Mutex>));
  else
    free_list_ = freelist;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, typename TIME_POLICY>
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>::~ACE_Timer_Queue_T (void)
{
  ACE_TRACE ("ACE_Timer_Queue_T::~ACE_Timer_Queue_T");

  // Cleanup the free_list on the way out
  if (this->delete_free_list_)
    delete this->free_list_;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, typename TIME_POLICY> ACE_Timer_Node_T<TYPE> *
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>::alloc_node (void)
{
  return this->free_list_->remove ();
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, typename TIME_POLICY> void
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>::free_node (ACE_Timer_Node_T<TYPE> *node)
{
  this->free_list_->add (node);
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, typename TIME_POLICY> ACE_LOCK &
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>::mutex (void)
{
  return this->mutex_;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, typename TIME_POLICY> long
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>::schedule (const TYPE &type,
                                                      const void *act,
                                                      const ACE_Time_Value &future_time,
                                                      const ACE_Time_Value &interval)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));

  // Schedule the timer.
  long const result =
    this->schedule_i (type,
                      act,
                      future_time,
                      interval);

  // Return on failure.
  if (result == -1)
    return result;

  // Inform upcall functor of successful registration.
  this->upcall_functor ().registration (*this,
                                        type,
                                        act);

  // Return result;
  return result;
}

// Run the <handle_timeout> method for all Timers whose values are <=
// <cur_time>.
template <class TYPE, class FUNCTOR, class ACE_LOCK, typename TIME_POLICY> int
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>::expire (const ACE_Time_Value &cur_time)
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

  while ((result = this->dispatch_info_i (cur_time, info)) != 0)
    {
      const void *upcall_act = 0;

      this->preinvoke (info, cur_time, upcall_act);

      this->upcall (info, cur_time);

      this->postinvoke (info, cur_time, upcall_act);

      ++number_of_timers_expired;

    }

  ACE_UNUSED_ARG (result);
  return number_of_timers_expired;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, typename TIME_POLICY> int
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>::expire_single (
    ACE_Command_Base & pre_dispatch_command)
{
  ACE_TRACE ("ACE_Timer_Queue_T::expire_single");
  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));

  if (this->is_empty ())
    return 0;

  // Get the current time
  ACE_Time_Value cur_time (this->gettimeofday () +
                           this->timer_skew ());

  // Look for a node in the timer queue whose timer <= the present
  // time.
  ACE_Timer_Node_Dispatch_Info_T<TYPE> info;
  if (this->dispatch_info_i (cur_time, info))
    {
      const void *upcall_act = 0;

      // Preinvoke (handles refcount if needed, etc.)
      this->preinvoke (info, cur_time, upcall_act);

      // Release the token before expiration upcall.
      pre_dispatch_command.execute();

      // call the functor
      this->upcall (info, cur_time);

      // Postinvoke (undo refcount if needed, etc.)
      this->postinvoke (info, cur_time, upcall_act);

      // We have dispatched a timer
      return 1;
    }

  return 0;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, typename TIME_POLICY> int
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>::dispatch_info_i (const ACE_Time_Value &cur_time,
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
            expired->set_timer_value (expired->get_timer_value () +
                                      expired->get_interval ());
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

template <class TYPE, class FUNCTOR, class ACE_LOCK, typename TIME_POLICY> void
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK, TIME_POLICY>::return_node (ACE_Timer_Node_T<TYPE> *node)
{
  ACE_MT (ACE_GUARD (ACE_LOCK, ace_mon, this->mutex_));
  this->free_node (node);
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_TIMER_QUEUE_T_CPP */
