// $Id$

#include "ace/Local_Memory_Pool.h"
#include "ace/Malloc_T.h"
#include "ace/Null_Mutex.h"
#include "ace/Synch_Traits.h"
#include "ace/Thread_Mutex.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Malloc<ACE_LOCAL_MEMORY_POOL, ACE_SYNCH_MUTEX>;
template class ACE_Malloc_T<ACE_LOCAL_MEMORY_POOL, ACE_SYNCH_MUTEX, ACE_Control_Block>;
template class ACE_Allocator_Adapter<ACE_Malloc<ACE_LOCAL_MEMORY_POOL, ACE_SYNCH_MUTEX> >;

// Explicitly instantiate these templates in the multithreaded case
// since some classes need them.
# if defined (ACE_HAS_THREADS)
template class ACE_Malloc<ACE_LOCAL_MEMORY_POOL, ACE_Null_Mutex>;
template class ACE_Malloc_T<ACE_LOCAL_MEMORY_POOL, ACE_Null_Mutex, ACE_Control_Block>;
template class ACE_Allocator_Adapter<ACE_Malloc<ACE_LOCAL_MEMORY_POOL, ACE_Null_Mutex> >;
# endif  /* ACE_HAS_THREADS */

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Malloc<ACE_LOCAL_MEMORY_POOL, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Malloc_T<ACE_LOCAL_MEMORY_POOL, ACE_SYNCH_MUTEX, ACE_Control_Block>
#pragma instantiate ACE_Allocator_Adapter<ACE_Malloc<ACE_LOCAL_MEMORY_POOL,ACE_SYNCH_MUTEX> >

// Explicitly instantiate these templates in the multithreaded case
// since some classes need them.
# if defined (ACE_HAS_THREADS)
#  pragma instantiate ACE_Malloc<ACE_LOCAL_MEMORY_POOL, ACE_Null_Mutex>
#  pragma instantiate ACE_Malloc_T<ACE_LOCAL_MEMORY_POOL, ACE_Null_Mutex, ACE_Control_Block>
#  pragma instantiate ACE_Allocator_Adapter<ACE_Malloc<ACE_LOCAL_MEMORY_POOL, ACE_Null_Mutex> >
# endif  /* ACE_HAS_THREADS */

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

ACE_END_VERSIONED_NAMESPACE_DECL
