// IO_SAP.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/IO_SAP.h"

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
  ACE_DEBUG ((LM_DEBUG, "handle_ = %d", this->handle_));
  ACE_DEBUG ((LM_DEBUG, "\npid_ = %d", this->pid_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// Cache for the process ID. 
pid_t ACE_IO_SAP::pid_ = 0;

// Make the HANDLE_ available for asynchronous I/O (SIGIO), urgent
// data (SIGURG), or non-blocking I/O (ACE_NONBLOCK).

int
ACE_IO_SAP::enable (int signum) const
{
  ACE_TRACE ("ACE_IO_SAP::enable");
  /* First-time in initialization. */
  if (ACE_IO_SAP::pid_ == 0)
    ACE_IO_SAP::pid_ = ACE_OS::getpid ();

#if !defined(ACE_WIN32)

  switch (signum)
    {
#if defined (SIGURG)
    case SIGURG:
#if defined (F_SETOWN)
      if (ACE_OS::fcntl (this->handle_, F_SETOWN, ACE_IO_SAP::pid_) < 0)
	return ACE_IO_SAP::INVALID_HANDLE;
      break;
#else
      return ACE_IO_SAP::INVALID_HANDLE;
#endif /* F_SETOWN */
#endif /* SIGURG */
#if defined (SIGIO)		// <==
    case SIGIO:
#if defined (F_SETOWN) && defined (FASYNC)
      if (ACE_OS::fcntl (this->handle_, F_SETOWN, ACE_IO_SAP::pid_) == ACE_IO_SAP::INVALID_HANDLE)
	return ACE_IO_SAP::INVALID_HANDLE;
      if (ACE::set_flags (this->handle_, FASYNC) == ACE_IO_SAP::INVALID_HANDLE)
	return ACE_IO_SAP::INVALID_HANDLE;
#else
      return ACE_IO_SAP::INVALID_HANDLE;
#endif /* F_SETOWN && FASYNC */
#else  // <==
      return ACE_IO_SAP::INVALID_HANDLE; // <==
#endif /* SIGIO <== */
      break;

    case ACE_NONBLOCK:
      if (ACE::set_flags (this->handle_, ACE_NONBLOCK) == ACE_IO_SAP::INVALID_HANDLE)
	return ACE_IO_SAP::INVALID_HANDLE;
      break;
    default:
      return ACE_IO_SAP::INVALID_HANDLE;
    }

#else
	ACE_UNUSED_ARG(signum);
#endif /* !ACE_WIN32 */

  return 0;
}

// Restore the IO_SAP by turning off synchronous I/O or urgent delivery. 

int
ACE_IO_SAP::disable (int signum) const
{
  ACE_TRACE ("ACE_IO_SAP::disable");

#if !defined(ACE_WIN32)

  switch (signum)
    {
#if defined (SIGURG)
    case SIGURG:
#if defined (F_SETOWN)
      if (ACE_OS::fcntl (this->handle_, F_SETOWN, 0) == ACE_IO_SAP::INVALID_HANDLE)
	return ACE_IO_SAP::INVALID_HANDLE;
#else
      return ACE_IO_SAP::INVALID_HANDLE;
#endif /* F_SETOWN */
      break;
#endif /* SIGURG */
#if defined (SIGIO)		// <==
    case SIGIO:
#if defined (F_SETOWN) && defined (FASYNC)
      if (ACE_OS::fcntl (this->handle_, F_SETOWN, 0) == ACE_IO_SAP::INVALID_HANDLE)
	return ACE_IO_SAP::INVALID_HANDLE;
      if (ACE::clr_flags (this->handle_, FASYNC) == ACE_IO_SAP::INVALID_HANDLE)
	return ACE_IO_SAP::INVALID_HANDLE;
#else
      return ACE_IO_SAP::INVALID_HANDLE;
#endif /* F_SETOWN && FASYNC */
#else  // <==
      return ACE_IO_SAP::INVALID_HANDLE; // <==
#endif /* SIGIO <== */
      break;
    case ACE_NONBLOCK:
      if (ACE::clr_flags (this->handle_, ACE_NONBLOCK) == ACE_IO_SAP::INVALID_HANDLE)
	return ACE_IO_SAP::INVALID_HANDLE;
      break;
    default:
      return ACE_IO_SAP::INVALID_HANDLE;
    }

#else
	ACE_UNUSED_ARG(signum);
#endif /* !ACE_WIN32 */

  return 0;
}

