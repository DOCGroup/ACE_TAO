#ifndef ACE_SYNCH_C
#define ACE_SYNCH_C

#include "ace/Thread.h"
#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"
#include "ace/Log_Msg.h"


ACE_RCSID (ace,
           Synch,
           "$Id$")


#if !defined (__ACE_INLINE__)
#include "ace/Synch.i"
#endif /* __ACE_INLINE__ */

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

  //ACE_ALLOC_HOOK_DEFINE(ACE_Null_Mutex)






#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
// These are only instantiated with ACE_HAS_THREADS.
template class ACE_Guard<ACE_Thread_Mutex>;
template class ACE_Guard<ACE_RW_Thread_Mutex>;
template class ACE_Read_Guard<ACE_RW_Thread_Mutex>;
template class ACE_Read_Guard<ACE_Thread_Mutex>;
template class ACE_Write_Guard<ACE_RW_Thread_Mutex>;
template class ACE_Write_Guard<ACE_Thread_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
// These are only instantiated with ACE_HAS_THREADS.
#pragma instantiate ACE_Guard<ACE_Thread_Mutex>
#pragma instantiate ACE_Guard<ACE_RW_Thread_Mutex>
#pragma instantiate ACE_Read_Guard<ACE_RW_Thread_Mutex>
#pragma instantiate ACE_Read_Guard<ACE_Thread_Mutex>
#pragma instantiate ACE_Write_Guard<ACE_RW_Thread_Mutex>
#pragma instantiate ACE_Write_Guard<ACE_Thread_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_SYNCH_C */
