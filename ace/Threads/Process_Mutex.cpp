// $Id$

#include "ace/Threads/Process_Mutex.h"
#include "ace/Threads/Synch.h"

#ifdef ACE_SUBSET_0
#include "ace/Log_Msg.h"
#endif

#if !defined (__ACE_INLINE__)
#include "ace/Process_Mutex.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Process_Mutex, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Process_Mutex)

#ifdef ACE_SUBSET_0
void
ACE_Process_Mutex::dump (void) const
{
// ACE_TRACE ("ACE_Process_Mutex::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->lock_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}
#endif /* ACE_SUBSET_0 */

#if defined (_ACE_USE_SV_SEM)
const ACE_TCHAR *
ACE_Process_Mutex::unique_name (void)
{
  // For all platforms other than Win32, we are going to create a
  // machine-wide unique name if one is not provided by the user.  On
  // Win32, unnamed synchronization objects are acceptable.
  ACE::unique_name (this, this->name_, ACE_UNIQUE_NAME_LEN);
  return this->name_;
}
#endif /* _ACE_USE_SV_SEM */

ACE_Process_Mutex::ACE_Process_Mutex (const char *name, void *arg)
#if defined (_ACE_USE_SV_SEM)
  : lock_ (name ? ACE_TEXT_CHAR_TO_TCHAR (name) :this->unique_name ())
#else
  : lock_ (USYNC_PROCESS, ACE_TEXT_CHAR_TO_TCHAR (name), (ACE_mutexattr_t *) arg)
#endif /* _ACE_USE_SV_SEM */
{
#if defined (_ACE_USE_SV_SEM)
  ACE_UNUSED_ARG (arg);
#endif /* !_ACE_USE_SV_SEM */
}

#if defined (ACE_HAS_WCHAR)
ACE_Process_Mutex::ACE_Process_Mutex (const wchar_t *name, void *arg)
#if defined (_ACE_USE_SV_SEM)
  : lock_ (name ? ACE_TEXT_WCHAR_TO_TCHAR (name): this->unique_name ())
#else
  : lock_ (USYNC_PROCESS, ACE_TEXT_WCHAR_TO_TCHAR (name), (ACE_mutexattr_t *) arg)
#endif /* _ACE_USE_SV_SEM */
{
#if defined (_ACE_USE_SV_SEM)
  ACE_UNUSED_ARG (arg);
#endif /* _ACE_USE_SV_SEM */
}
#endif /* ACE_HAS_WCHAR */
ACE_Process_Mutex::~ACE_Process_Mutex (void)
{
}

//
// These are instantiated both with and without ACE_HAS_THREADS.
//
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Guard<ACE_Process_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Guard<ACE_Process_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
