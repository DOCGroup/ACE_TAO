// Dynamic.cpp
// $Id$

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
# if (defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)))
    template class ACE_TSS<ACE_Dynamic>;
# endif /* ACE_HAS_THREADS && (ACE_HAS_THREAD_SPECIFIC_STORAGE || ACE_HAS_TSS_EMULATION) */

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
  #pragma instantiate ACE_TSS_Singleton<ACE_Dynamic, ACE_Null_Mutex>

# if (defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)))
    #pragma instantiate ACE_TSS<ACE_Dynamic>
# endif /* ACE_HAS_THREADS && (ACE_HAS_THREAD_SPECIFIC_STORAGE || ACE_HAS_TSS_EMULATION) */

#elif defined (__GNUC__) && (defined (_AIX) || defined (__hpux) || defined (VXWORKS))
template ACE_TSS_Singleton<ACE_Dynamic, ACE_Null_Mutex> *
  ACE_TSS_Singleton<ACE_Dynamic, ACE_Null_Mutex>::singleton_;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
