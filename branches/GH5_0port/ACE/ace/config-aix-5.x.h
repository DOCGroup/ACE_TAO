// $Id$
//
// Config file for AIX 5.1 and higher.

#ifndef ACE_CONFIG_AIX_5_X_H
#define ACE_CONFIG_AIX_5_X_H

// This define is needed for building with Visual Age C++ 5 in incremental
// mode. In the batch mode build, platform_aix_ibm.GNU sets it. The incremental
// mode compiler won't be supported after ACE 5.3, so this will also go away
// in that timeframe, so don't worry about future AIX versions.
#if !defined (ACE_AIX_VERS)
#  define ACE_AIX_VERS 501
#endif

// AIX 5.1 has AIO, but it doesn't have the same API as other POSIX
// systems, and the semantics of operations are a bit different. Will take
// some real work to get this going.
// AIX 5.2, however, has the POSIX API implemented. However, the libc functions
// to implement it aren't exported by default. You need to use smit to enable
// them. So, leave AIO disabled unless the user explicitly enables it.
// config-aix-4.x.h will set ACE_HAS_AIO_CALLS if config-posix.h senses the
// feature-test macros, so set up _ACE_DISABLE_AIO_CALLS_ if the user didn't
// set it. Then check for it after including config-aix-4.x.h and remove
// ACE_HAS_AIO_CALLS if so.
#if !defined (ACE_HAS_AIO_CALLS)
#  define _ACE_DISABLE_AIO_CALLS_
#endif
#include "ace/config-aix-4.x.h"
#if defined (_ACE_DISABLE_AIO_CALLS_)
#  if defined (ACE_HAS_AIO_CALLS)
#    undef ACE_HAS_AIO_CALLS
#  endif
#  undef _ACE_DISABLE_AIO_CALLS_
#endif

// AIX's /usr/include/unistd.h sets _POSIX_SEMAPHORE to indicate the system
// supplies such a facility, but the headers don't enable it unless
// _XOPEN_SOURCE >= 500. So disable semaphores here if _XOPEN_SOURCE isn't
// up to snuff.
#if defined (ACE_HAS_POSIX_SEM) && \
    (!defined (_XOPEN_SOURCE) || (_XOPEN_SOURCE-0 < 500))
#  undef ACE_HAS_POSIX_SEM
#endif

// I think this is correct, but needs to be verified...   -Steve Huston
#define ACE_HAS_SIGTIMEDWAIT

// AIX 5.1 has netinet/tcp.h
#undef ACE_LACKS_NETINET_TCP_H

#define ACE_HAS_POSIX_GETPWNAM_R
#define ACE_HAS_SCANDIR
# define ACE_SCANDIR_CMP_USES_VOIDPTR
# define ACE_SCANDIR_SEL_LACKS_CONST
#define ACE_HAS_SIGSUSPEND
#define ACE_HAS_TIMEZONE  /* Call tzset() to set timezone */

#endif /* ACE_CONFIG_AIX_5_X_H */
