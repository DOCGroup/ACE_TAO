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

template <class LOCK>
class ACE_Event_Handler_Handle_Timeout_Upcall
  // = TITLE 
  //      Functor for Timer_Queues.
  //
  // = DESCRIPTION
  //      This class implements the functor required by the Timer
  //      Queue to call <handle_timeout> on ACE_Event_Handlers.
{
public:
  typedef ACE_Timer_Queue_T<ACE_Event_Handler *, 
                            ACE_Event_Handler_Handle_Timeout_Upcall<LOCK>, 
                            LOCK> 
          TIMER_QUEUE;
  
  int timeout (TIMER_QUEUE &timer_queue,
	       ACE_Event_Handler *handler,
	       const void *arg,
	       const ACE_Time_Value &cur_time);
  // This method is called when the timer expires
  
  int cancellation (TIMER_QUEUE &timer_queue,
		    ACE_Event_Handler *handler);
  // This method is called when the timer is canceled

  int deletion (TIMER_QUEUE &timer_queue,
                ACE_Event_Handler *handler,
                const void *arg);
  // This method is called when the timer queue is destroyed and
  // the timer is still contained in it
};

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


#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Timer_Queue.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Timer_Queue.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_TIMER_QUEUE_H */
