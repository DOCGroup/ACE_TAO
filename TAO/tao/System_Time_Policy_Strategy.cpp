// $Id$

#include "tao/System_Time_Policy_Strategy.h"

#include "ace/Timer_Heap_T.h"
#include "ace/Event_Handler_Handle_Timeout_Upcall.h"

#if (TAO_HAS_TIME_POLICY == 1)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_Time_Policy_T<ACE_System_Time_Policy>  TAO_System_Time_Policy_Strategy::time_policy_;

TAO_System_Time_Policy_Strategy::~TAO_System_Time_Policy_Strategy ()
{
}

ACE_Timer_Queue * TAO_System_Time_Policy_Strategy::create_timer_queue (void)
{
  ACE_Timer_Queue * tmq = 0;

  typedef ACE_Timer_Heap_T<ACE_Event_Handler *,
                           ACE_Event_Handler_Handle_Timeout_Upcall,
                           ACE_SYNCH_RECURSIVE_MUTEX,
                           ACE_System_Time_Policy> timer_queue_type;
  ACE_NEW_RETURN (tmq, timer_queue_type (), 0);

  return tmq;
}

void
TAO_System_Time_Policy_Strategy::destroy_timer_queue (ACE_Timer_Queue *tmq)
{
  delete tmq;
}

ACE_Dynamic_Time_Policy_Base * TAO_System_Time_Policy_Strategy::get_time_policy (void)
{
  return &time_policy_;
}


ACE_STATIC_SVC_DEFINE (TAO_System_Time_Policy_Strategy,
                       ACE_TEXT ("TAO_SYSTEM_TIME_POLICY"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_System_Time_Policy_Strategy),
                       ACE_Service_Type::DELETE_THIS |
                                  ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO, TAO_System_Time_Policy_Strategy)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_TIME_POLICY */
