// $Id$

#include "Custom_Time_Policy_Strategy.h"

#include "ace/Timer_Heap_T.h"
#include "ace/Event_Handler_Handle_Timeout_Upcall.h"
#include "ace/High_Res_Timer.h"

#if (TAO_HAS_TIME_POLICY == 1)

class ACE_Export Custom_Time_Policy
{
public:
  /// Return the current time according to this policy
  ACE_Time_Value operator() () const
  {
    ACE_Time_Value tv = ACE_High_Res_Timer::gettimeofday_hr ();
    return (tv += ACE_Time_Value (10, 0));
  }
  /// Noop. Just here to satisfy backwards compatibility demands.
  void set_gettimeofday (ACE_Time_Value (*)(void)) {}
};

static ACE_Time_Policy_T<Custom_Time_Policy>  custom_time_policy_;

Custom_Time_Policy_Strategy::~Custom_Time_Policy_Strategy ()
{
}

ACE_Timer_Queue * Custom_Time_Policy_Strategy::create_timer_queue (void)
{
  ACE_Timer_Queue * tmq = 0;

  typedef ACE_Timer_Heap_T<ACE_Event_Handler *,
                           ACE_Event_Handler_Handle_Timeout_Upcall,
                           ACE_SYNCH_RECURSIVE_MUTEX,
                           Custom_Time_Policy> timer_queue_type;
  ACE_NEW_RETURN (tmq, timer_queue_type (), 0);

  return tmq;
}

void
Custom_Time_Policy_Strategy::destroy_timer_queue (ACE_Timer_Queue *tmq)
{
  delete tmq;
}

ACE_Dynamic_Time_Policy_Base * Custom_Time_Policy_Strategy::get_time_policy (void)
{
  return &custom_time_policy_;
}


ACE_STATIC_SVC_DEFINE (Custom_Time_Policy_Strategy,
                       ACE_TEXT ("CUSTOM_TIME_POLICY"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (Custom_Time_Policy_Strategy),
                       ACE_Service_Type::DELETE_THIS |
                                  ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TPTEST, Custom_Time_Policy_Strategy)

#endif /* TAO_HAS_TIME_POLICY */
