#ifndef ACE_SYNCH_CPP
#define ACE_SYNCH_CPP

#include "ace/Thread.h"
#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// FUZZ: disable check_for_synch_include
#include "ace/Synch.h"
#include "ace/Log_Msg.h"


ACE_RCSID (ace,
           Synch,
           "$Id$")

#include "ace/TSS_Adapter.cpp"
#include "ace/Auto_Event.cpp"
#include "ace/Manual_Event.cpp"
#include "ace/Barrier.cpp"
#include "ace/Condition_Thread_Mutex.cpp"
#include "ace/Condition_Recursive_Thread_Mutex.cpp"
#include "ace/Event.cpp"
#include "ace/Lock.cpp"
#include "ace/Mutex.cpp"
#include "ace/RW_Mutex.cpp"
#include "ace/RW_Thread_Mutex.cpp"
#include "ace/Recursive_Thread_Mutex.cpp"
#include "ace/Semaphore.cpp"
#include "ace/Thread_Mutex.cpp"
#include "ace/Thread_Semaphore.cpp"

// ACE_BEGIN_VERSIONED_NAMESPACE_DECL
// ACE_ALLOC_HOOK_DEFINE(ACE_Null_Mutex)
// ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_SYNCH_CPP */
