// $Id$

#if !defined (ACE_TIMER_QUEUE_C)
#define ACE_TIMER_QUEUE_C

#define ACE_BUILD_DLL

#include "ace/config.h"
#include "ace/Containers.h"
#include "ace/Timer_Queue.h"

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Unbounded_Set<ACE_Timer_Node_T<ACE_Event_Handler *> *>;
template class ACE_Node<ACE_Timer_Node_T<ACE_Event_Handler *> *>;
template class ACE_Unbounded_Set_Iterator<ACE_Timer_Node_T<ACE_Event_Handler *> *>;
template class ACE_Timer_Node_T<ACE_Event_Handler *>;
template class ACE_Timer_Queue_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_Queue_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, ACE_SYNCH_RECURSIVE_MUTEX>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

#endif /* ACE_TIMER_QUEUE_C */
