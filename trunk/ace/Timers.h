/* -*- C++ -*- */

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Timers.h
//
// = AUTHOR
//    Irfan Pyarali
// 
// ============================================================================

#if !defined (ACE_TIMERS_H)
#define ACE_TIMERS_H

#include "ace/Timer_Queue.h"
#include "ace/Timer_List.h"
#include "ace/Timer_Heap.h"

class ACE_Export ACE_Event_Handler_Handle_Timeout_Upcall
  // = TITLE 
  //      Functor for Timer_Queues.
  //
  // = DESCRIPTION
  //
  //      This class implements the functor required by the Timer
  //      Queue to call <handle_timeout> on ACE_Event_Handlers.
{
public:
  typedef ACE_Timer_Queue_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall> TIMER_QUEUE;
  
  int operator () (TIMER_QUEUE &timer_queue,
		   ACE_Event_Handler *handler,
		   const void *arg,
		   const ACE_Time_Value &cur_time);
  // This method is called when the timer expires
  
  int operator () (TIMER_QUEUE &timer_queue,
		   ACE_Event_Handler *handler);
  // This method is called when the timer is canceled
};

// The following typedef are there mostly for ease of use and backward compatibility

typedef ACE_Timer_Node_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall> ACE_Timer_Node;

typedef ACE_Timer_Queue_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall> ACE_Timer_Queue;
typedef ACE_Timer_Queue_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall> ACE_Timer_Queue_Iterator;

typedef ACE_Timer_List_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall> ACE_Timer_List;
typedef ACE_Timer_List_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall> ACE_Timer_List_Iterator;

typedef ACE_Timer_Heap_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall> ACE_Timer_Heap;
typedef ACE_Timer_Heap_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall> ACE_Timer_Heap_Iterator;

#if defined (__ACE_INLINE__)
#include "ace/Timers.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_TIMERS_H */
