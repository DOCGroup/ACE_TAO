/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Timer_List.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_TIMER_LIST_H)
#define ACE_TIMER_LIST_H

#include "ace/Timer_List_T.h"

// The following typedef are here for ease of use and backward
// compatibility.

typedef ACE_Timer_List_T<ACE_Event_Handler *, 
                         ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, 
                         ACE_SYNCH_RECURSIVE_MUTEX> 
	ACE_Timer_List;

typedef ACE_Timer_List_Iterator_T<ACE_Event_Handler *, 
                                  ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, 
                                  ACE_SYNCH_RECURSIVE_MUTEX>  
	ACE_Timer_List_Iterator;

#endif /* ACE_TIMER_LIST_H */
