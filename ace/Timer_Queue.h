/* -*- C++ -*- */
// $Id$

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

#include "ace/Timer_Queue_T.h"

class ACE_Export ACE_Event_Handler_Handle_Timeout_Upcall
  // = TITLE 
  //      Functor for Timer_Queues.
  //
  // = DESCRIPTION
  //      This class implements the functor required by the Timer
  //      Queue to call <handle_timeout> on ACE_Event_Handlers.
{
public:
  typedef ACE_Timer_Queue_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall> 
          TIMER_QUEUE;
  
  int operator () (TIMER_QUEUE &timer_queue,
		   ACE_Event_Handler *handler,
		   const void *arg,
		   const ACE_Time_Value &cur_time);
  // This method is called when the timer expires
  
  int operator () (TIMER_QUEUE &timer_queue,
		   ACE_Event_Handler *handler);
  // This method is called when the timer is canceled
};

// The following typedef are here for ease of use and backward
// compatibility.

typedef ACE_Timer_Node_T<ACE_Event_Handler *, 
	                 ACE_Event_Handler_Handle_Timeout_Upcall> 
	ACE_Timer_Node;

typedef ACE_Timer_Queue_T<ACE_Event_Handler *, 
	                  ACE_Event_Handler_Handle_Timeout_Upcall> 
	ACE_Timer_Queue;

typedef ACE_Timer_Queue_Iterator_T<ACE_Event_Handler *, 
                                   ACE_Event_Handler_Handle_Timeout_Upcall> 
	ACE_Timer_Queue_Iterator;

#endif /* ACE_TIMER_QUEUE_H */
