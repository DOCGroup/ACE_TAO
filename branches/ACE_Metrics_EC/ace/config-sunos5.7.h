/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for SunOS 5.7
// (Solaris 7) platforms using the SunC++ 4.x, 5.x, or g++ compilers.

#ifndef ACE_CONFIG_H

// ACE_CONFIG_H is defined by one of the following #included headers.

// #include the SunOS 5.6 config file, then add SunOS 5.7 updates below.

#include "ace/config-sunos5.6.h"

// Sun began distributing <sys/loadavg.h> with SunOS 5.7
#define ACE_HAS_SYS_LOADAVG_H

#if defined (__GNUG__)
# if __GNUC__ <= 2  &&  __GNUC_MINOR__ < 8
    // Assume that later g++ were built on SunOS 5.7, so they don't
    // have these problems . . .

    // Disable the following, because g++ 2.7.2.3 can't handle it.
    // Maybe later g++ versions can?
#   undef ACE_HAS_XPG4_MULTIBYTE_CHAR

    // The Solaris86 g++ 2.7.2.3 sys/types.h doesn't have these . . .
    typedef long          t_scalar_t;  /* historical versions */
    typedef unsigned long t_uscalar_t;
    typedef void          *timeout_id_t;
# endif /* __GNUC__ <= 2  &&  __GNUC_MINOR__ < 8 */

#elif defined (ghs)
  // SunOS 5.7's /usr/include/sys/procfs_isa.h needs uint64_t,
  // but /usr/include/sys/int_types.h doesn't #define it because
  // _NO_LONGLONG is #
# undef ACE_HAS_PROC_FS
# undef ACE_HAS_PRUSAGE_T

#elif defined (__KCC)
typedef unsigned long long uint64_t;
#endif /* __GNUG__ || ghs || __KCC */

// SunOS 5.7 supports SCHED_FIFO and SCHED_RR, as well as SCHED_OTHER.
#undef ACE_HAS_ONLY_SCHED_OTHER

// SunOS 5.7 gets this right . . .
#undef ACE_HAS_BROKEN_T_ERROR

// And doesn't need to set LWP priorities, as shown by
// performance-tests/Misc/preempt.
#undef ACE_NEEDS_LWP_PRIO_SET

// SunOS 5.7 can support Real-Time Signals and POSIX4 AIO operations
// are supported.

#if !defined (ACE_HAS_AIO_CALLS)
#define ACE_HAS_AIO_CALLS
#endif /* !ACE_HAS_AIO_CALLS */

#if defined (ACE_POSIX_AIOCB_PROACTOR)
#undef ACE_POSIX_AIOCB_PROACTOR
#endif /* ACE_POSIX_AIOCB_PROACTOR */

// This is anyway default.
#define ACE_POSIX_SIG_PROACTOR

#ifdef ACE_HAS_LIMITED_SELECT
#undef ACE_HAS_LIMITED_SELECT
#endif /* ACE_HAS_LIMITED_SELECT */

// SunOS 5.7 has socklen_t
#define ACE_HAS_SOCKLEN_T

// snprintf() is available at least from Solaris 7 on. If it can be determined
// that it's also available in Solaris 2.6, this can be moved into
// config-sunos5.6.h.
#define ACE_HAS_SNPRINTF

#if defined (__sparcv9)
#define ERRMAX 256 /* Needed for following define */
#define ACE_LACKS_SYS_NERR
#define _LP64
#define ACE_SIZEOF_LONG 8 /* Needed to circumvent compiler bug #4294969 */
#endif /* __sparcv9 */

#if (defined(_XOPEN_SOURCE) && (_XOPEN_VERSION - 0 == 4)) /* XPG4 or XPG4v2 */
// 2 parameter wcstok()
#else   /* XPG4 or XPG4v2 */
# define ACE_HAS_3_PARAM_WCSTOK
#endif

#endif /* ACE_CONFIG_H */
