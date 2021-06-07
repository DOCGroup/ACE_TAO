// -*- C++ -*-
// Config Header file for Android NDK

#ifndef ACE_CONFIG_ANDROID_H
#define ACE_CONFIG_ANDROID_H
#include /**/ "ace/pre.h"

// NOTE: We must be careful from now on to distinguish between the API level
// and NDK version.
// There is a large number of combinations of these two that can lead to
// problems.

#include <android/ndk-version.h>
#include <android/api-level.h>

#define ACE_ANDROID
#define ACE_PLATFORM_CONFIG config-android.h

#include "ace/config-linux-common.h"

/*
 * Android NDK Revision Macros
 *
 * Revisions Scheme Work Like This:
 * Revision | __NDK_MAJOR__ | __NDK_MINOR__ | __NDK__BETA__
 * r16      | 16            | 0             | 0
 * r16b     | 16            | 1             | 0
 * r16c     | 16            | 2             | 0
 * r22-beta1| 22            | 0             | 1
 *
 * __NDK_BETA__ is starts at 1 and increments until release when it is 0.
 */
#define ACE_ANDROID_NDK_AT_LEAST(MAJ, MIN, BET) (\
  (__NDK_MAJOR__ > (MAJ)) || \
  (__NDK_MAJOR__ == (MAJ) && __NDK_MINOR__ >= (MIN)) || \
  (__NDK_MAJOR__ == (MAJ) && __NDK_MINOR__ == (MIN) && \
    (__NDK_BETA__ == 0 || __NDK_BETA__ >= (BET)) \
  ) \
)

#define ACE_ANDROID_NDK_EXACTLY(MAJ, MIN, BET) \
  (__NDK_MAJOR__ == (MAJ) && __NDK_MINOR__ == (MIN) && __NDK_BETA__ == (BETA))

#define ACE_ANDROID_NDK_LESS_THAN(MAJ, MIN, BET) \
  !ACE_ANDROID_NDK_AT_LEAST((MAJ), (MIN), (BET))

#if __ANDROID_API__ < 18
#  define ACE_LACKS_LOG2
#endif

#if __ANDROID_API__ < 21
#  define ACE_LACKS_SEARCH_H
#  define ACE_LACKS_SYS_SEM_H
#  define ACE_LACKS_SEMBUF_T
#  define ACE_LACKS_SYS_MSG_H
#  define ACE_LACKS_SYS_SHM_H
#  define ACE_LACKS_SYSV_SHMEM
#  define ACE_LACKS_RAND_R
#  define ACE_LACKS_WCSTOLL
#  define ACE_LACKS_WCSTOULL
#  define ACE_LACKS_CONDATTR_SETCLOCK
#  ifdef ACE_HAS_EVENT_POLL
#    undef ACE_HAS_EVENT_POLL
#  endif
#else
#  define ACE_HAS_SEMUN
#endif

#if __ANDROID_API__ < 23
#  define ACE_LACKS_TELLDIR
#  define ACE_LACKS_SEEKDIR
#endif

#if __ANDROID_API__ < 24
#  define ACE_LACKS_IF_NAMEINDEX
#endif

// These were available before r18, but in r18 they are restricted to API >= 28 ¯\_(ツ)_/¯
#if __ANDROID_API__ < 28
#  define ACE_LACKS_SETHOSTENT
#  define ACE_LACKS_ENDHOSTENT
#endif

#define ACE_HAS_UCONTEXT_T
#define ACE_HAS_CLOCK_SETTIME
#define ACE_HAS_CPU_SET_T

// system errorno is a volatile int
#define ACE_HAS_VOLATILE_ERRNO
#define ACE_ERRNO_TYPE volatile int

// Android doesn't check is sig is out of range.
#define ACE_HAS_SIGISMEMBER_BUG

#define ACE_HAS_PTHREADS_UNIX98_EXT

// Android does not support pthread concurrency (yet)
#define ACE_LACKS_MUTEXATTR_PSHARED 1

// the android definition of struct stat{} uses
// unsigned long rather than time_t for st_[acm]time
// members of the stat struct used to report file
// status details.
#define ACE_USES_ULONG_FOR_STAT_TIME

#define ACE_LACKS_NEW_H
#define ACE_LACKS_SIGINFO_H
#define ACE_LACKS_STROPTS_H
#define ACE_LACKS_SYS_SYSCTL_H

#define ACE_LACKS_GETLOADAVG
#define ACE_LACKS_ISCTYPE
#define ACE_LACKS_NETDB_REENTRANT_FUNCTIONS
#define ACE_LACKS_PWD_FUNCTIONS
#define ACE_LACKS_STRRECVFD
#define ACE_LACKS_PTHREAD_CANCEL // posix_limits.h explicitly says this
#define ACE_LACKS_SETINHERITSCHED // posix_limits.h explicitly says this
#define ACE_LACKS_SWAB

// Android seems to have 64 keys of which Android itself use 5
#define ACE_DEFAULT_THREAD_KEYS 59

// Android Standalone compiler std library does not include
// wide character support
// Used in tests/Sequence_Unit_Tests/string_sequence_tester.hpp
# define TAO_LACKS_WCHAR_CXX_STDLIB

// @todo JW, test if this works
// #define ACE_HAS_POSIX_SEM
// #define ACE_HAS_POSIX_SEM_TIMEOUT

// AIO support pulls in the rt library, which pulls in the pthread
// library.  Disable AIO in single-threaded builds.
#if defined (ACE_HAS_THREADS)
#  define ACE_HAS_CLOCK_GETTIME
#else
#  undef ACE_HAS_AIO_CALLS
#endif

#define ACE_HAS_SIGINFO_T
#define ACE_HAS_SOCKLEN_T
#define ACE_HAS_4_4BSD_SENDMSG_RECVMSG

#define ACE_LACKS_CUSERID

#define ACE_MMAP_NO_ZERO

#define ACE_HAS_NETLINK
#define ACE_HAS_SIOCGIFCONF

#if !defined ACE_DEFAULT_TEMP_DIR
# define ACE_DEFAULT_TEMP_DIR "/data/tmp"
#endif

#if !defined TEST_DIR
# define TEST_DIR "/data"
#endif

#define ACE_HAS_DLSYM_SEGFAULT_ON_INVALID_HANDLE

#if !defined (ACE_AS_STATIC_LIBS)
# if (__GNUC__ == 4 && __GNUC_MINOR__ == 4)
#  error Shared library support is not possible with GCC 4.4.x
# endif
#endif

// The defines listed below might give compile issues when
// users declare one letter (IDL) methods. To prevent this,
// these defines are undefined here.
// The defines are declared in ctype.h and are used in
// ctype_base.h as well. That's why <ostream> is included
// as well.
#include "ctype.h"
#include <ostream>

#if defined (_U)
# undef _U
#endif
#if defined (_L)
# undef _L
#endif
#if defined (_N)
# undef _N
#endif
#if defined (_S)
# undef _S
#endif
#if defined (_P)
# undef _P
#endif
#if defined (_C)
# undef _C
#endif
#if defined (_X)
# undef _X
#endif
#if defined (_B)
# undef _B
#endif

// Disable newer features, result in runtime failures on Android
#define ACE_LACKS_GETADDRINFO
#define ACE_LACKS_GETNAMEINFO

#include /**/ "ace/post.h"

#endif /* ACE_CONFIG_ANDROID_H */
