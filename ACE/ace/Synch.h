// -*- C++ -*-

//==========================================================================
/**
 *  @file    Synch.h
 *
 *   Wrapper Facades for various synchronization mechanisms.
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */
//==========================================================================

#ifndef ACE_SYNCH_H
#define ACE_SYNCH_H
#include /**/ "pre.h"

#include /**/ "config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (DO_NOT_INCLUDE_SYNCH_H)

/* All the classes have been moved out into their own headers as part of
   the compile-time and footprint reduction effort. */

#include "Auto_Event.h"
#include "Barrier.h"
#include "Condition_Thread_Mutex.h"
#include "Condition_Recursive_Thread_Mutex.h"
#include "Event.h"
#include "Lock.h"
#include "Manual_Event.h"
#include "Mutex.h"
#include "Null_Barrier.h"
#include "Null_Condition.h"
#include "Null_Mutex.h"
#include "Null_Semaphore.h"
#include "RW_Mutex.h"
#include "RW_Thread_Mutex.h"
#include "Recursive_Thread_Mutex.h"
#include "Semaphore.h"
#include "Thread_Mutex.h"
#include "Thread_Semaphore.h"
#include "TSS_Adapter.h"

#endif /* DO_NOT_INCLUDE_SYNCH_H */

#include /**/ "post.h"
#endif /* ACE_SYNCH_H */
