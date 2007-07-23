// Deadlock_Free_TP_Reactor.cpp,v 4.69 2005/04/23 20:04:39 jwillemsen Exp

#include "ace/Deadlock_Free_TP_Reactor.h"
#include "ace/Thread.h"
#include "ace/Timer_Queue.h"
#include "ace/Signal.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_sys_time.h"


ACE_RCSID (ace,
           Deadlock_Free_TP_Reactor,
           "Deadlock_Free_TP_Reactor.cpp,v 4.69 2005/04/23 20:04:39 jwillemsen Exp")

ACE_ALLOC_HOOK_DEFINE (ACE_Deadlock_Free_TP_Reactor)


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Entry<void *, int>;
template class ACE_Hash_Map_Manager_Ex<void *, int, ACE_Hash<void *>, ACE_Equal_To<void *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<void *, int, ACE_Hash<void *>, ACE_Equal_To<void *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Const_Iterator_Base_Ex<void *, int, ACE_Hash<void *>, ACE_Equal_To<void *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<void *, int, ACE_Hash<void *>, ACE_Equal_To<void *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Const_Iterator_Ex<void *, int, ACE_Hash<void *>, ACE_Equal_To<void *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<void *, int, ACE_Hash<void *>, ACE_Equal_To<void *>, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Entry<void *, int>
#pragma instantiate ACE_Hash_Map_Manager_Ex<void *, int, ACE_Hash<void *>, ACE_Equal_To<void *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<void *, int, ACE_Hash<void *>, ACE_Equal_To<void *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Const_Iterator_Base_Ex<void *, int, ACE_Hash<void *>, ACE_Equal_To<void *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<void *, int, ACE_Hash<void *>, ACE_Equal_To<void *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Const_Iterator_Ex<void *, int, ACE_Hash<void *>, ACE_Equal_To<void *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<void *, int, ACE_Hash<void *>, ACE_Equal_To<void *>, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
