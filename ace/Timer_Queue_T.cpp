#if !defined (ACE_TIMER_QUEUE_T_C)
#define ACE_TIMER_QUEUE_T_C

#define ACE_BUILD_DLL
#include "ace/Synch.h"
#include "ace/Timer_Queue_T.h"

#if !defined (__ACE_INLINE__)
#include "ace/Timer_Queue_T.i"
#endif /* __ACE_INLINE__ */

template <class TYPE> void
ACE_Timer_Node_T<TYPE>::dump (void) const
{
  ACE_TRACE ("ACE_Timer_Node_T::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  // ACE_DEBUG ((LM_DEBUG, "\type_ = %x", this->type_));
  ACE_DEBUG ((LM_DEBUG, "\nact_ = %x", this->act_));
  this->timer_value_.dump ();
  this->interval_.dump ();
  ACE_DEBUG ((LM_DEBUG, "\nprev_ = %x", this->prev_));
  ACE_DEBUG ((LM_DEBUG, "\nnext_ = %x", this->next_));
  ACE_DEBUG ((LM_DEBUG, "\ntimer_id_ = %d", this->timer_id_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class TYPE> 
ACE_Timer_Node_T<TYPE>::ACE_Timer_Node_T (void)
{
  ACE_TRACE ("ACE_Timer_Node_T::ACE_Timer_Node_T");
}

template <class TYPE, class FUNCTOR, class LOCK> 
ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR, LOCK>::ACE_Timer_Queue_Iterator_T (void)
{
}

template <class TYPE, class FUNCTOR, class LOCK> 
ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR, LOCK>::~ACE_Timer_Queue_Iterator_T (void)
{
}

// Determines the minimum amount of time that the Reactor must wait
// before timing out.  This is computed as the smaller of (1) the
// amount the caller requested when calling handle_events() and (2)
// the earliest time registered in the Timer Queue (if any).  Must be
// called with an external lock held since it returns a pointer to a
// Time_Value type stored in the Timer_Queue type itself.  If some
// external lock isn't held we'll have reentrancy problems!

template <class TYPE, class FUNCTOR, class LOCK> ACE_Time_Value *
ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK>::calculate_timeout (ACE_Time_Value *max_wait_time)
{
  ACE_TRACE ("ACE_Timer_Queue_T::calculate_timeout");
  ACE_MT (ACE_GUARD_RETURN (LOCK, ace_mon, this->mutex_, max_wait_time));
  
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

template <class TYPE, class FUNCTOR, class LOCK> void
ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK>::dump (void) const
{
  ACE_TRACE ("ACE_Timer_Queue_T::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->timeout_.dump ();
  this->timer_skew_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}

template <class TYPE, class FUNCTOR, class LOCK> 
ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK>::ACE_Timer_Queue_T (FUNCTOR *upcall_functor, 
                                                           ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist)
  : free_list_ (freelist == 0 ? new ACE_Locked_Free_List<ACE_Timer_Node_T <TYPE>, ACE_Null_Mutex> : freelist),
    gettimeofday_ (ACE_OS::gettimeofday),
    upcall_functor_ (upcall_functor == 0 ? new FUNCTOR : upcall_functor),
    delete_upcall_functor_ (upcall_functor == 0),
    delete_free_list_ (freelist == 0),
    timer_skew_ (0, ACE_TIMER_SKEW)
{
  ACE_TRACE ("ACE_Timer_Queue_T::ACE_Timer_Queue_T");
}

template <class TYPE, class FUNCTOR, class LOCK> 
ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK>::~ACE_Timer_Queue_T (void)
{
  ACE_TRACE ("ACE_Timer_Queue_T::~ACE_Timer_Queue_T");

  // Cleanup the functor and free_list on the way out
  if (this->delete_upcall_functor_)
    delete this->upcall_functor_;

  if (this->delete_free_list_)
    delete this->free_list_;
}

template <class TYPE, class FUNCTOR, class LOCK> ACE_Timer_Node_T<TYPE> *
ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK>::alloc_node (void)
{
  return this->free_list_->remove ();
}

template <class TYPE, class FUNCTOR, class LOCK> void 
ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK>::free_node (ACE_Timer_Node_T<TYPE> *node)
{
  this->free_list_->add (node);
}

// Run the <handle_timeout> method for all Timers whose values are <=
// <cur_time>.

template <class TYPE, class FUNCTOR, class LOCK> int
ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK>::expire (const ACE_Time_Value &cur_time)
{
  ACE_TRACE ("ACE_Timer_Queue_T::expire");
  ACE_MT (ACE_GUARD_RETURN (LOCK, ace_mon, this->mutex_, -1));

  int number_of_timers_expired = 0;

  ACE_Timer_Node_T<TYPE> *expired;

  // Keep looping while there are timers remaining and the earliest
  // timer is <= the <cur_time> passed in to the method.

  while (this->earliest_time () <= cur_time)
    {
      expired = this->remove_first ();
      TYPE &type = expired->get_type ();
      const void *act = expired->get_act ();
      int reclaim = 1;
      
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
	  reclaim = 0;
	}
      
      // call the functor
      this->upcall (type, act, cur_time);
      
      if (reclaim)
	// Call the factory method to free up the node.
	this->free_node (expired);
      
      number_of_timers_expired++;

      if (this->is_empty ())
        break;
    }

  return number_of_timers_expired;
}

template <class LOCK> int
ACE_Event_Handler_Handle_Timeout_Upcall<LOCK>::timeout (ACE_Timer_Queue_T<ACE_Event_Handler *, 
							ACE_Event_Handler_Handle_Timeout_Upcall<LOCK>, 
							LOCK> &timer_queue,
                                                        ACE_Event_Handler *handler,
                                                        const void *act,
                                                        const ACE_Time_Value &cur_time)
{
  // Upcall to the <handler>s handle_timeout method
  if (handler->handle_timeout (cur_time, act) == -1)
    timer_queue.cancel (handler, 0); // 0 means "call handle_close()".    
  
  return 0;
}

template <class LOCK> int
ACE_Event_Handler_Handle_Timeout_Upcall<LOCK>::cancellation (ACE_Timer_Queue_T<ACE_Event_Handler *, 
							     ACE_Event_Handler_Handle_Timeout_Upcall<LOCK>, 
							     LOCK> &timer_queue,
                                                             ACE_Event_Handler *handler)
{
  ACE_UNUSED_ARG (timer_queue);

  // Upcall to the <handler>s handle_close method
  handler->handle_close (ACE_INVALID_HANDLE, 
			 ACE_Event_Handler::TIMER_MASK);
  return 0;
}

template <class LOCK> int
ACE_Event_Handler_Handle_Timeout_Upcall<LOCK>::deletion (ACE_Timer_Queue_T<ACE_Event_Handler *, 
							 ACE_Event_Handler_Handle_Timeout_Upcall<LOCK>, 
							 LOCK> &timer_queue,
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
