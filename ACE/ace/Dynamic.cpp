#include "Dynamic.h"
#include "Singleton.h"
#include "TSS_T.h"
#include "Synch_Traits.h"
#include "Null_Mutex.h"

#if !defined (__ACE_INLINE__)
#include "Dynamic.inl"
#endif /* __ACE_INLINE__ */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_Dynamic::ACE_Dynamic ()
  : is_dynamic_ (false)
{
  ACE_TRACE ("ACE_Dynamic::ACE_Dynamic");
}

ACE_ALLOC_HOOK_DEFINE(ACE_Dynamic)

/* static */ ACE_Dynamic *
ACE_Dynamic::instance ()
{
  return ACE_TSS_Singleton<ACE_Dynamic, ACE_SYNCH_NULL_MUTEX>::instance ();
}

ACE_SINGLETON_TEMPLATE_INSTANTIATE(ACE_TSS_Singleton, ACE_Dynamic, ACE_Null_Mutex);


ACE_END_VERSIONED_NAMESPACE_DECL
