// $Id$

#if !defined (ACE_TIMER_LIST_C)
#define ACE_TIMER_LIST_C

#include "ace/Timer/Timer_List.h"

ACE_RCSID(ace, Timer_List, "$Id$")

#if defined (ACE_HAS_BROKEN_HPUX_TEMPLATES)
#include "ace/Timer/Timer_Hash.h"
#include "ace/Timer/Timer_List_T.cpp"

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class
    ACE_Timer_List_T<
        ACE_Event_Handler*,
        ACE_Timer_Hash_Upcall<
        ACE_Event_Handler*,
        ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>,
        ACE_Null_Mutex>,
    ACE_Null_Mutex>;

template class
ACE_Timer_List_Iterator_T<
    ACE_Event_Handler*,
    ACE_Timer_Hash_Upcall<
        ACE_Event_Handler*,
        ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>,
        ACE_Null_Mutex>,
    ACE_Null_Mutex>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
#endif /* ACE_HAS_BROKEN_HPUX_TEMPLATES */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Timer_List_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_List_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, ACE_SYNCH_RECURSIVE_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Timer_List_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Timer_List_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, ACE_SYNCH_RECURSIVE_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


#endif /* ACE_TIMER_LIST_C */
