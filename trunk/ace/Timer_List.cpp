#if !defined (ACE_TIMER_LIST_C)
#define ACE_TIMER_LIST_C

#define ACE_BUILD_DLL

#include "ace/config.h"
#include "ace/Timer_List.h"

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Timer_List_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_List_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, ACE_SYNCH_RECURSIVE_MUTEX>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

#endif /* ACE_TIMER_LIST_C */
