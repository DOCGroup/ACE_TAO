// $Id$

#include "ace/Threads/RW_Process_Mutex.h"

#ifdef ACE_SUBSET_0
#include "ace/Log_Msg.h"
#endif

#include "ace/ACE.h"

ACE_RCSID(ace, RW_Process_Mutex, "$Id$")

#if !defined (__ACE_INLINE__)
#include "ace/Threads/RW_Process_Mutex.inl"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_RW_Process_Mutex)

const ACE_TCHAR *
ACE_RW_Process_Mutex::unique_name (void)
{
  ACE::unique_name (this, this->name_, ACE_UNIQUE_NAME_LEN);
  return this->name_;
}

ACE_RW_Process_Mutex::ACE_RW_Process_Mutex (const ACE_TCHAR *name,
                                            int flags)
  : lock_ (name ? name : this->unique_name (), flags
#if defined (ACE_WIN32)
           , ACE_DEFAULT_OPEN_PERMS)
#else
           , S_IRUSR | S_IWUSR)
#endif /* ACE_WIN32 */
{
// ACE_TRACE ("ACE_RW_Process_Mutex::ACE_RW_Process_Mutex");
}

ACE_RW_Process_Mutex::~ACE_RW_Process_Mutex (void)
{
// ACE_TRACE ("ACE_RW_Process_Mutex::~ACE_RW_Process_Mutex");
}

#ifdef ACE_SUBSET_0
void
ACE_RW_Process_Mutex::dump (void) const
{
// ACE_TRACE ("ACE_RW_Process_Mutex::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->lock_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}
#endif

//
// These are instantiated both with and without ACE_HAS_THREADS.
//
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

// template class ACE_Guard<ACE_RW_Process_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

// #pragma instantiate ACE_Guard<ACE_RW_Process_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
