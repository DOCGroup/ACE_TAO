/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Timer_Heap.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_TIMER_HEAP_H)
#define ACE_TIMER_HEAP_H

#include "ace/Timer_Heap_T.h"

// The following typedef are here for ease of use and backward
// compatibility.

typedef ACE_Timer_Heap_T<ACE_Event_Handler *, 
                         ACE_Event_Handler_Handle_Timeout_Upcall, 
                         ACE_SYNCH_RECURSIVE_MUTEX>  
	ACE_Timer_Heap;

typedef ACE_Timer_Heap_Iterator_T<ACE_Event_Handler *, 
                                  ACE_Event_Handler_Handle_Timeout_Upcall, 
                                  ACE_SYNCH_RECURSIVE_MUTEX>   
	ACE_Timer_Heap_Iterator;

#endif /* ACE_TIMER_HEAP_H */
