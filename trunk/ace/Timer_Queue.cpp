#if !defined (ACE_TIMER_QUEUE_C)
#define ACE_TIMER_QUEUE_C

#define ACE_BUILD_DLL

#include "ace/Timer_Queue.h"
#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
#include "ace/Timer_Heap_T.h"
#include "ace/Timer_List_T.h"
#include "ace/Timer_Wheel_T.h"
#include "ace/Timer_Hash_T.h"
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
#if 0
template class ACE_Unbounded_Set<ACE_Timer_Node_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX> *>;
template class ACE_Node<ACE_Timer_Node_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX> *>;
template class ACE_Unbounded_Set_Iterator<ACE_Timer_Node_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX> *>;
template class ACE_Timer_Heap_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_Heap_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_Node_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_List_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_List_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_Queue_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_Queue_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_Wheel_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_Wheel_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_Hash_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_Hash_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
#endif
#endif /* ACE_TIMER_QUEUE_C */
