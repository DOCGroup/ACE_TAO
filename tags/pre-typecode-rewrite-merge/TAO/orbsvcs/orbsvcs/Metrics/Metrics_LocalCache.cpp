// $Id$

#ifndef METRICS_LOCAL_CACHE_CPP
#define METRICS_LOCAL_CACHE_CPP

#include "Metrics_LocalCache.h"

#if !defined (__ACE_INLINE__)
#include "Metrics_LocalCache.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Entry<int, TAO_Metrics_LocalCache<ACE_Thread_Mutex, ACE_New_Allocator>::TAO_Metrics_Cache_Data *>;
template class ACE_Hash_Map_Entry<int, TAO_Metrics_LocalCache<ACE_Thread_Mutex, ACE_New_Allocator>::TAO_Metrics_Cache_Data_Base *>;
template class ACE_Hash_Map_Manager_Ex<int, TAO_Metrics_LocalCache<ACE_Thread_Mutex, ACE_New_Allocator>::TAO_Metrics_Cache_Data *, ACE_Hash<int>, ACE_Equal_To<int>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<int, TAO_Metrics_LocalCache<ACE_Thread_Mutex, ACE_New_Allocator>::TAO_Metrics_Cache_Data_Base *, ACE_Hash<int>, ACE_Equal_To<int>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<int, TAO_Metrics_LocalCache<ACE_Thread_Mutex, ACE_New_Allocator>::TAO_Metrics_Cache_Data_Base *, ACE_Hash<int>, ACE_Equal_To<int>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<int, TAO_Metrics_LocalCache<ACE_Thread_Mutex, ACE_New_Allocator>::TAO_Metrics_Cache_Data *, ACE_Hash<int>, ACE_Equal_To<int>, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Entry<int, TAO_Metrics_LocalCache<ACE_Thread_Mutex, ACE_New_Allocator>::TAO_Metrics_Cache_Data *>
#pragma instantiate ACE_Hash_Map_Entry<int, TAO_Metrics_LocalCache<ACE_Thread_Mutex, ACE_New_Allocator>::TAO_Metrics_Cache_Data_Base *>
#pragma instantiate ACE_Hash_Map_Manager_Ex<int, TAO_Metrics_LocalCache<ACE_Thread_Mutex, ACE_New_Allocator>::TAO_Metrics_Cache_Data *, ACE_Hash<int>, ACE_Equal_To<int>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<int, TAO_Metrics_LocalCache<ACE_Thread_Mutex, ACE_New_Allocator>::TAO_Metrics_Cache_Data_Base *, ACE_Hash<int>, ACE_Equal_To<int>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<int, TAO_Metrics_LocalCache<ACE_Thread_Mutex, ACE_New_Allocator>::TAO_Metrics_Cache_Data_Base *, ACE_Hash<int>, ACE_Equal_To<int>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<int, TAO_Metrics_LocalCache<ACE_Thread_Mutex, ACE_New_Allocator>::TAO_Metrics_Cache_Data *, ACE_Hash<int>, ACE_Equal_To<int>, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* METRICS_LOCAL_CACHE_CPP */

