// $Id$

#define ACE_BUILD_DLL

#include "ace/Timer_Queue.h"

int
ACE_Event_Handler_Handle_Timeout_Upcall::operator () (TIMER_QUEUE &timer_queue,
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
ACE_Event_Handler_Handle_Timeout_Upcall::operator () (TIMER_QUEUE &timer_queue,
						      ACE_Event_Handler *handler)
{
  ACE_UNUSED_ARG (timer_queue);

  // Upcall to the <handler>s handle_close method
  handler->handle_close (ACE_INVALID_HANDLE, 
			 ACE_Event_Handler::TIMER_MASK);
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)

template class ACE_Unbounded_Set<ACE_Timer_Node_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall> *>;
template class ACE_Unbounded_Set_Iterator<ACE_Timer_Node_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall> *>;
template class ACE_Set_Node<ACE_Timer_Node_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall> *>;

template class ACE_Timer_Node_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall>;

template class ACE_Timer_Queue_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall>;
template class ACE_Timer_Queue_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall>;

template class ACE_Timer_List_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall>;
template class ACE_Timer_List_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall>;

template class ACE_Timer_Heap_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall>;
template class ACE_Timer_Heap_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall>;

#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

