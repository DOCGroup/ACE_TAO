/* -*- C++ -*- */

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Timer_Queue.h
//
// = AUTHOR
//    Doug Schmidt and Irfan Pyarali
// 
// ============================================================================

#if !defined (ACE_TIMER_QUEUE_H)
#define ACE_TIMER_QUEUE_H

#include "ace/Synch.h"
#include "ace/Timer_Queue_T.h"

// The following typedef are here for ease of use and backward
// compatibility.

typedef ACE_Timer_Node_T<ACE_Event_Handler *>
	ACE_Timer_Node;

typedef ACE_Timer_Queue_T<ACE_Event_Handler *, 
	                  ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, 
                          ACE_SYNCH_RECURSIVE_MUTEX> 
	ACE_Timer_Queue;

typedef ACE_Timer_Queue_Iterator_T<ACE_Event_Handler *, 
                                   ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, 
                                   ACE_SYNCH_RECURSIVE_MUTEX> 
	ACE_Timer_Queue_Iterator;

#endif /* ACE_TIMER_QUEUE_H */
