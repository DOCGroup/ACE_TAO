// $Id$

#define ACE_BUILD_DLL
#include "ace/IPC_SAP.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/IPC_SAP.i"
#endif

ACE_RCSID(ace, IPC_SAP, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_IPC_SAP)

void
ACE_IPC_SAP::dump (void) const
{
  ACE_TRACE ("ACE_IPC_SAP::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("handle_ = %d"), this->handle_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\npid_ = %d"), this->pid_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// Cache for the process ID.
pid_t ACE_IPC_SAP::pid_ = 0;

// This is the do-nothing constructor.  It does not perform a
// ACE_OS::socket system call.

ACE_IPC_SAP::ACE_IPC_SAP (void)
  : handle_ (ACE_INVALID_HANDLE)
{
//  ACE_TRACE ("ACE_IPC_SAP::ACE_IPC_SAP");
}

// Make the HANDLE_ available for asynchronous I/O (SIGIO), urgent
// data (SIGURG), or non-blocking I/O (ACE_NONBLOCK).

int
ACE_IPC_SAP::enable (int signum) const
{
  ACE_TRACE ("ACE_IPC_SAP::enable");

  // First-time in initialization.
  if (ACE_IPC_SAP::pid_ == 0)
    ACE_IPC_SAP::pid_ = ACE_OS::getpid ();

#if defined (ACE_WIN32) || defined (VXWORKS)
  switch (signum)
    {
    case ACE_NONBLOCK:
    // nonblocking argument (1)
    // blocking:            (0)
      {
        u_long nonblock = 1;
        return ACE_OS::ioctl (this->handle_, FIONBIO, &nonblock);
      }
    default:
      ACE_NOTSUP_RETURN (-1);
    }
#else  /* ! ACE_WIN32 && ! VXWORKS */
  switch (signum)
    {
#if defined (SIGURG)
    case SIGURG:
#if defined (F_SETOWN)
      return ACE_OS::fcntl (this->handle_, F_SETOWN, ACE_IPC_SAP::pid_);
#else
      return -1;
#endif /* F_SETOWN */
#endif /* SIGURG */
#if defined (SIGIO)             // <==
    case SIGIO:
#if defined (F_SETOWN) && defined (FASYNC)
      if (ACE_OS::fcntl (this->handle_, F_SETOWN, ACE_IPC_SAP::pid_) == -1 ||
          ACE::set_flags (this->handle_, FASYNC) == -1)
        return -1;
      else
        return 0;
#else
      return -1;
#endif /* F_SETOWN && FASYNC */
#endif /* SIGIO <== */
    case ACE_NONBLOCK:
      if (ACE::set_flags (this->handle_, ACE_NONBLOCK) == ACE_INVALID_HANDLE)
        return -1;
      else
        return 0;
    default:
      return -1;
    }
#endif /* ! ACE_WIN32 && ! VXWORKS */

  /* NOTREACHED */
}

// Restore the IPC_SAPet by turning off synchronous I/O or urgent
// delivery.

int
ACE_IPC_SAP::disable (int signum) const
{
  ACE_TRACE ("ACE_IPC_SAP::disable");

#if defined (ACE_WIN32) || defined (VXWORKS)
  switch (signum)
    {
    case ACE_NONBLOCK:
      // nonblocking argument (1)
      // blocking:            (0)
      {
        u_long nonblock = 0;
        return ACE_OS::ioctl (this->handle_, FIONBIO, &nonblock);
      }
    default:
      ACE_NOTSUP_RETURN (-1);
    }
#else  /* ! ACE_WIN32 && ! VXWORKS */
  switch (signum)
    {
#if defined (SIGURG)
    case SIGURG:
#if defined (F_SETOWN)
      return ACE_OS::fcntl (this->handle_, F_SETOWN, 0);
#else
      return -1;
#endif /* F_SETOWN */
#endif /* SIGURG */
#if defined (SIGIO)             // <==
    case SIGIO:
#if defined (F_SETOWN) && defined (FASYNC)
      if (ACE_OS::fcntl (this->handle_, F_SETOWN, 0) == -1 ||
          ACE::clr_flags (this->handle_, FASYNC) == -1)
        return -1;
      else
        return 0;
#else
      return -1;
#endif /* F_SETOWN && FASYNC */
#endif /* SIGIO <== */
    case ACE_NONBLOCK:
      if (ACE::clr_flags (this->handle_, ACE_NONBLOCK) == -1)
        return -1;
      else
        return 0;
    default:
      return -1;
    }
#endif /* ! ACE_WIN32 && ! VXWORKS */

  /* NOTREACHED */
}
