/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Timer_Heap.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef ACE_TIMER_HEAP_H
#define ACE_TIMER_HEAP_H
#include "ace/pre.h"

#include "ace/Timer_Heap_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// The following typedef are here for ease of use and backward
// compatibility.

typedef ACE_Timer_Heap_T<ACE_Event_Handler *,
                         ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>,
                         ACE_SYNCH_RECURSIVE_MUTEX>
        ACE_Timer_Heap;

typedef ACE_Timer_Heap_Iterator_T<ACE_Event_Handler *,
                                  ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>,
                                  ACE_SYNCH_RECURSIVE_MUTEX>
        ACE_Timer_Heap_Iterator;

#include "ace/post.h"
#endif /* ACE_TIMER_HEAP_H */
