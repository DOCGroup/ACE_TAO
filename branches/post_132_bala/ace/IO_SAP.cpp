// IO_SAP.cpp
// $Id$

#include "ace/IO_SAP.h"
#include "ace/Log_Msg.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/IO_SAP.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

ACE_RCSID(ace, IO_SAP, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_IO_SAP)

// This is the do-nothing constructor.  It does not perform a
// ACE_OS::open system call.

ACE_IO_SAP::ACE_IO_SAP (void)
  : handle_ (ACE_INVALID_HANDLE)
{
  ACE_TRACE ("ACE_IO_SAP::ACE_IO_SAP");
}

void
ACE_IO_SAP::dump (void) const
{
  ACE_TRACE ("ACE_IO_SAP::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("handle_ = %d"), this->handle_));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\npid_ = %d"), this->pid_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// Cache for the process ID.
pid_t ACE_IO_SAP::pid_ = 0;

int
ACE_IO_SAP::enable (int value) const
{
  ACE_TRACE ("ACE_IO_SAP::enable");
  /* First-time in initialization. */
  if (ACE_IO_SAP::pid_ == 0)
    ACE_IO_SAP::pid_ = ACE_OS::getpid ();

#if !defined(ACE_WIN32) && !defined (VXWORKS)

  switch (value)
    {
#if defined (SIGURG)
    case SIGURG:
    case ACE_SIGURG:
#if defined (F_SETOWN)
      return ACE_OS::fcntl (this->handle_,
                            F_SETOWN,
                            ACE_IO_SAP::pid_);
#else
      ACE_NOTSUP_RETURN (-1);
#endif /* F_SETOWN */
#endif /* SIGURG */
#if defined (SIGIO)
    case SIGIO:
    case ACE_SIGIO:
#if defined (F_SETOWN) && defined (FASYNC)
      if (ACE_OS::fcntl (this->handle_,
                         F_SETOWN,
                         ACE_IO_SAP::pid_) == -1
          || ACE_Flag_Manip::set_flags (this->handle_,
                             FASYNC) == -1)
        return -1;
      break;
#else
      ACE_NOTSUP_RETURN (-1);
#endif /* F_SETOWN && FASYNC */
#else  // <==
      ACE_NOTSUP_RETURN (-1);
#endif /* SIGIO <== */
    case ACE_NONBLOCK:
      if (ACE_Flag_Manip::set_flags (this->handle_,
                          ACE_NONBLOCK) == -1)
        return -1;
      break;
    default:
      return -1;
    }
#else
  ACE_UNUSED_ARG (value);
#endif /* !ACE_WIN32 */

  return 0;
}

int
ACE_IO_SAP::disable (int value) const
{
  ACE_TRACE ("ACE_IO_SAP::disable");

#if !defined(ACE_WIN32) && !defined (VXWORKS)
  switch (value)
    {
#if defined (SIGURG)
    case SIGURG:
    case ACE_SIGURG:
#if defined (F_SETOWN)
      if (ACE_OS::fcntl (this->handle_,
                         F_SETOWN, 0) == -1)
        return -1;
      break;
#else
      ACE_NOTSUP_RETURN (-1);
#endif /* F_SETOWN */
#endif /* SIGURG */
#if defined (SIGIO)
    case SIGIO:
    case ACE_SIGIO:
#if defined (F_SETOWN) && defined (FASYNC)
      if (ACE_OS::fcntl (this->handle_,
                         F_SETOWN,
                         0) == -1
          || ACE_Flag_Manip::clr_flags (this->handle_, FASYNC) == -1)
        return -1;
      break;
#else
      ACE_NOTSUP_RETURN (-1);
#endif /* F_SETOWN && FASYNC */
#else  // <==
      ACE_NOTSUP_RETURN (-1);
#endif /* SIGIO <== */
    case ACE_NONBLOCK:
      if (ACE_Flag_Manip::clr_flags (this->handle_,
                                     ACE_NONBLOCK) == -1)
        return -1;
      break;
    default:
      return -1;
    }
  return 0;
#else
  ACE_UNUSED_ARG (value);
  ACE_NOTSUP_RETURN (-1);
#endif /* !ACE_WIN32 */
}
