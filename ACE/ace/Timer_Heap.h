/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Timer_Heap.h
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */
//=============================================================================

#ifndef ACE_TIMER_HEAP_H
#define ACE_TIMER_HEAP_H
#include /**/ "ace/pre.h"

#include "ace/Timer_Heap_T.h"
#include "ace/Event_Handler_Handle_Timeout_Upcall.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// The following typedefs are here for ease of use and backward
// compatibility.

using ACE_Timer_Heap = ACE_Timer_Heap_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall, ACE_MT_SYNCH::RECURSIVE_MUTEX>;

using ACE_Timer_Heap_Iterator = ACE_Timer_Heap_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall, ACE_MT_SYNCH::RECURSIVE_MUTEX>;

using ACE_Timer_Heap_Variable_Time_Source = ACE_Timer_Heap_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall, ACE_MT_SYNCH::RECURSIVE_MUTEX, ACE_FPointer_Time_Policy>;

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* ACE_TIMER_HEAP_H */
