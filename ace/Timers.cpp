#define ACE_BUILD_DLL

#include "ace/Timers.h"

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
  // Upcall to the <handler>s handle_close method
  handler->handle_close (ACE_INVALID_HANDLE, 
			 ACE_Event_Handler::TIMER_MASK);
  return 0;
}
