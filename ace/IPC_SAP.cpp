// IPC_SAP.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/IPC_SAP.h"

ACE_ALLOC_HOOK_DEFINE(ACE_IPC_SAP)

void
ACE_IPC_SAP::dump (void) const
{
  ACE_TRACE ("ACE_IPC_SAP::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "handle_ = %d", this->handle_));
  ACE_DEBUG ((LM_DEBUG, "\npid_ = %d", this->pid_));
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

#if defined(ACE_WIN32)
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
      errno = ENOTSUP;
      return -1;
    }
#else
  switch (signum)
    {
#if defined (SIGURG)
    case SIGURG:
#if defined (F_SETOWN)
      if (ACE_OS::fcntl (this->handle_, F_SETOWN, ACE_IPC_SAP::pid_) < 0)
	return -1;
#else
      return -1;
#endif /* F_SETOWN */
      break;
#endif /* SIGURG */
#if defined (SIGIO)		// <==
    case SIGIO:
#if defined (F_SETOWN) && defined (FASYNC)
      if (ACE_OS::fcntl (this->handle_, F_SETOWN, ACE_IPC_SAP::pid_) == -1)
	return -1;
      if (ACE::set_flags (this->handle_, FASYNC) == -1)
	return -1;
#else
      return -1;
#endif /* F_SETOWN && FASYNC */
#else  // <==
      return -1;		// <==
#endif /* SIGIO <== */
       break;
    case ACE_NONBLOCK:
      if (ACE::set_flags (this->handle_, ACE_NONBLOCK) == ACE_INVALID_HANDLE)
	return -1;
      break;
    default:
      return -1;
    }
#endif /* !ACE_WIN32 */
  return 0;
}

// Restore the IPC_SAPet by turning off synchronous I/O or urgent
// delivery.

int
ACE_IPC_SAP::disable (int signum) const
{
  ACE_TRACE ("ACE_IPC_SAP::disable");

#if defined(ACE_WIN32)
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
      errno = ENOTSUP;
      return -1;
    }
#else
  switch (signum)
    {
#if defined (SIGURG)
    case SIGURG:
#if defined (F_SETOWN)
      if (ACE_OS::fcntl (this->handle_, F_SETOWN, 0) == -1)
	return -1;
      break;
#else
      return -1;
#endif /* F_SETOWN */
#endif /* SIGURG */
#if defined (SIGIO)		// <==
    case SIGIO:
#if defined (F_SETOWN) && defined (FASYNC)
      if (ACE_OS::fcntl (this->handle_, F_SETOWN, 0) == -1)
	return -1;
      if (ACE::clr_flags (this->handle_, FASYNC) == -1)
	return -1;
      break;
#else
      return -1;
#endif /* F_SETOWN && FASYNC */
#else  // <==
      return -1;		// <==
#endif /* SIGIO <== */
    case ACE_NONBLOCK:
      if (ACE::clr_flags (this->handle_, ACE_NONBLOCK) == -1)
	return -1;
      break;
    default:
      return -1;
    }
#endif /* !ACE_WIN32 */
  return 0;
}

