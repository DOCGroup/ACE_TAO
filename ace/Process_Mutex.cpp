// $Id$

#include "ace/Process_Mutex.h"
#include "ace/Synch.h"
#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
#include "ace/Process_Mutex.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Process_Mutex, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Process_Mutex)

void
ACE_Process_Mutex::dump (void) const
{
// ACE_TRACE ("ACE_Process_Mutex::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->lock_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

#if !defined (ACE_WIN32) && !defined (ACE_HAS_POSIX_SEM) && !defined (ACE_PSOS)
const ACE_TCHAR *
ACE_Process_Mutex::unique_name (void)
{
  // For all platforms other than Win32, we are going to create a
  // machine wide unquie name if one is not provided by the user.  On
  // Win32, unnamed synchronization objects are acceptable.
  ACE::unique_name (this, this->name_, ACE_UNIQUE_NAME_LEN);
  return this->name_;
}
#endif /* !ACE_WIN32 && !ACE_HAS_POSIX_SEM && !ACE_PSOS */

ACE_Process_Mutex::ACE_Process_Mutex (const char *name, void *arg)
#if defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM) || defined (ACE_PSOS)
  : lock_ (USYNC_PROCESS, ACE_TEXT_CHAR_TO_TCHAR (name), (ACE_mutexattr_t *) arg)
#else
  : lock_ (name ? ACE_TEXT_CHAR_TO_TCHAR (name) : ACE_Process_Mutex::unique_name ())
#endif /* ACE_WIN32 || ACE_HAS_POSIX_SEM || ACE_PSOS */
{
#if !defined (ACE_WIN32) && !defined (ACE_HAS_POSIX_SEM) && !defined (ACE_PSOS)
  ACE_UNUSED_ARG (arg);
#endif /* !ACE_WIN32 && !ACE_HAS_POSIX_SEM && !ACE_PSOS */
}

#if defined (ACE_HAS_WCHAR)
ACE_Process_Mutex::ACE_Process_Mutex (const wchar_t *name, void *arg)
#if defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM) || defined (ACE_PSOS)
  : lock_ (USYNC_PROCESS, ACE_TEXT_WCHAR_TO_TCHAR (name), (ACE_mutexattr_t *) arg)
#else
  : lock_ (name ? ACE_TEXT_WCHAR_TO_TCHAR (name): ACE_Process_Mutex::unique_name ())
#endif /* ACE_WIN32 || ACE_HAS_POSIX_SEM || ACE_PSOS */
{
#if !defined (ACE_WIN32) && !defined (ACE_HAS_POSIX_SEM) && !defined (ACE_PSOS)
  ACE_UNUSED_ARG (arg);
#endif /* !ACE_WIN32 && !ACE_HAS_POSIX_SEM && !ACE_PSOS */
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
