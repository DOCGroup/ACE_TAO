/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work
// for Unixware platforms running UnixWare 2.1.2 and gcc version 2.7.2.2

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H
#include "ace/pre.h"

// See README for what the ACE_HAS... and ACE_LACKS... macros mean

#if ! defined (__ACE_INLINE__)
# define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

// config-g++-common.h undef's ACE_HAS_STRING_CLASS with -frepo, so
// this must appear before its #include.
#define ACE_HAS_STRING_CLASS

#if defined (__GNUG__)
# include "ace/config-g++-common.h"
#endif /* __GNUG__ */

// Optimize ACE_Handle_Set for select().
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

#define ACE_LACKS_SYSTIME_H
// ualarm is only in BSD compatibility lib, but no header is provided
// #define ACE_HAS_UALARM
#define ACE_HAS_SIZET_SOCKET_LEN
#define ACE_HAS_AUTOMATIC_INIT_FINI
#define ACE_HAS_CPLUSPLUS_HEADERS
#define ACE_HAS_GNU_CSTRING_H
#define ACE_HAS_MSG
#define ACE_HAS_SVR4_GETTIMEOFDAY
#define ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R
#define ACE_HAS_NONCONST_GETBY
#define ACE_HAS_POLL
#define ACE_HAS_POSIX_NONBLOCK
#define ACE_HAS_POSIX_TIME
#define ACE_LACKS_TIMESPEC_T
#define ACE_HAS_REENTRANT_FUNCTIONS
#define ACE_HAS_REGEX
#define ACE_HAS_LAZY_V
#define ACE_HAS_SELECT_H
#define ACE_HAS_SIGINFO_T
#define ACE_HAS_UCONTEXT_T
#define ACE_HAS_SIGWAIT
#define ACE_HAS_SIG_ATOMIC_T
#define ACE_HAS_SOCKIO_H
#define ACE_HAS_SSIZE_T
#define ACE_HAS_STHREADS
#define ACE_HAS_THR_KEYDELETE
#define ACE_HAS_STRBUF_T
#define ACE_HAS_STREAMS
#define ACE_HAS_STREAM_PIPES
#define ACE_HAS_STRERROR
#define ACE_HAS_SVR4_DYNAMIC_LINKING
#define ACE_HAS_SYSCALL_H
#define ACE_HAS_SYSINFO
#define ACE_HAS_SYSV_IPC
#define ACE_HAS_SYS_FILIO_H
#define ACE_HAS_TERM_IOCTLS
#define ACE_HAS_THREADS
#define ACE_HAS_THREAD_SPECIFIC_STORAGE
#define ACE_HAS_THREAD_T
#define ACE_HAS_TIMOD_H
#define ACE_HAS_TIUSER_H
#define ACE_HAS_TLI
#define ACE_HAS_TLI_PROTOTYPES
#define ACE_HAS_UNIXWARE_SVR4_SIGNAL_T
#define ACE_HAS_VOIDPTR_SOCKOPT
#define ACE_HAS_THR_MINSTACK

#define ACE_LACKS_MADVISE
#define ACE_LACKS_STRCASECMP
#define ACE_LACKS_PRI_T
#define ACE_LACKS_NETDB_REENTRANT_FUNCTIONS
#define ACE_LACKS_PWD_REENTRANT_FUNCTIONS

#if !defined (ACE_MT_SAFE)
# define ACE_MT_SAFE 1
#endif

#define ACE_PAGE_SIZE 4096
#define ACE_REDEFINES_XTI_FUNCTIONS

#if !defined (UNIXWARE)
# define UNIXWARE
# define UNIXWARE_2_1
#endif /* ! UNIXWARE */

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
# define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#define ACE_HAS_IDTYPE_T

#include "ace/post.h"
#endif /* ACE_CONFIG_H */
