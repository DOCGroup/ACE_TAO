#ifndef ACE_CONFIG_EMSCRIPTEN_H
#define ACE_CONFIG_EMSCRIPTEN_H

#define ACE_EMSCRIPTEN

#define ACE_HAS_CPP11

// Threading
#define ACE_MT_SAFE 1
#define ACE_HAS_THREADS 1
#define ACE_HAS_PTHREADS
#define ACE_HAS_PTHREADS_UNIX98_EXT
#define ACE_HAS_THREAD_SPECIFIC_STORAGE

// Basic Types
#define ACE_HAS_SSIZE_T
#define ACE_HAS_WCHAR

// Time
#define ACE_HAS_POSIX_TIME
#define ACE_LACKS_TIMESPEC_T
#define ACE_HAS_NONCONST_SELECT_TIMEVAL
#define ACE_HAS_VOIDPTR_GETTIMEOFDAY

// Everything else
#define ACE_HAS_UCONTEXT_T
#define ACE_HAS_MSG
#define ACE_HAS_CPU_SET_T
#define ACE_LACKS_ISCTYPE
#define ACE_HAS_STRBUF_T
#define ACE_HAS_SOCKLEN_T
#define ACE_LACKS_AUTO_PTR
#define ACE_HAS_SIGINFO_T
#define ACE_LACKS_SIGINFO_H
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES
#define ACE_HAS_3_PARAM_WCSTOK
#define ACE_LACKS_ITOW
#define ACE_LACKS_WCSICMP
#define ACE_LACKS_WCSNICMP
#define ACE_HAS_DIRENT
#define ACE_HAS_4_4BSD_SENDMSG_RECVMSG

// These exist, but don't seem to work correctly
#define ACE_LACKS_PWD_FUNCTIONS

#include "config-posix.h"
#include "config-g++-common.h"

// Not supported: https://github.com/emscripten-core/emscripten/issues/18050
#ifdef ACE_HAS_AIO_CALLS
#  undef ACE_HAS_AIO_CALLS
#endif

#endif
