// $Id$

#define ACE_BUILD_DLL

#include "ace/Timer_Queue.h"
#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
#include "ace/Timer_Heap_T.h"
#include "ace/Timer_List_T.h"
#include "ace/Timer_Wheel_T.h"
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

int
ACE_Event_Handler_Handle_Timeout_Upcall::timeout (TIMER_QUEUE &timer_queue,
						  ACE_Event_Handler *handler,
						  const void *act,
						  const ACE_Time_Value &cur_time)
{
  // Upcall to the <handler>s handle_timeout method
  if (handler->handle_timeout (cur_time, act) == -1)
    timer_queue.cancel (handler, 0); // 0 means "call handle_close()".    
  
  return 0;
}

int
ACE_Event_Handler_Handle_Timeout_Upcall::cancellation (TIMER_QUEUE &timer_queue,
						       ACE_Event_Handler *handler)
{
  ACE_UNUSED_ARG (timer_queue);

  // Upcall to the <handler>s handle_close method
  handler->handle_close (ACE_INVALID_HANDLE, 
			 ACE_Event_Handler::TIMER_MASK);
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
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
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
