
/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Timer_Wheel.h
 *
 *  $Id$
 *
 *  @author Darrell Brunsch (brunsch@cs.wustl.edu)
 */
//=============================================================================


#ifndef ACE_TIMER_WHEEL_H
#define ACE_TIMER_WHEEL_H
#include "ace/pre.h"

#include "ace/Timer/Timer_Wheel_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// The following typedef are here for ease of use and backward
// compatibility.

typedef ACE_Timer_Wheel_T<ACE_Event_Handler *,
                         ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>,
                         ACE_SYNCH_RECURSIVE_MUTEX>
        ACE_Timer_Wheel;

typedef ACE_Timer_Wheel_Iterator_T<ACE_Event_Handler *,
                                   ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>,
                                   ACE_SYNCH_RECURSIVE_MUTEX>
        ACE_Timer_Wheel_Iterator;

#include "ace/post.h"
#endif /* ACE_TIMER_WHEEL_H */
