// Dynamic.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Dynamic.h"
#include "ace/Singleton.h"
#include "ace/Synch_T.h"

#if !defined (__ACE_INLINE__)
#include "ace/Dynamic.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Dynamic, "$Id$")

ACE_Dynamic::ACE_Dynamic (void)
  : is_dynamic_ (0)
{
  ACE_TRACE ("ACE_Dynamic::ACE_Dynamic");
}

/* static */ ACE_Dynamic *
ACE_Dynamic::instance (void)
{
  return ACE_TSS_Singleton<ACE_Dynamic, ACE_SYNCH_NULL_MUTEX>::instance ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template class ACE_TSS_Singleton<ACE_Dynamic, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
  #pragma instantiate ACE_TSS_Singleton<ACE_Dynamic, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
