/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for pSOSystem V2.2.2,
// using the Diab Data D-C++ 4.2a compiler (or a later version)

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H
#include "ace/pre.h"

// #if ! defined (__ACE_INLINE__)
// # define __ACE_INLINE__
// #endif /* ! __ACE_INLINE__ */
#if defined (__ACE_INLINE__)
# undef __ACE_INLINE__
#endif /* __ACE_INLINE__ */

#define ACE_LACKS_ISATTY

#define ACE_LACKS_DIFFTIME

#define ACE_LACKS_FCNTL

#define ACE_LACKS_FILELOCKS

#define ACE_LACKS_FSYNC

#define ACE_LACKS_INLINE_FUNCTIONS

#define ACE_LACKS_TEMPLATE_AS_TEMPLATE_PARAMETER

#if defined (__GNUG__)
# include "ace/config-g++-common.h"
#endif /* __GNUG__ */

#define ACE_PSOS_LACKS_PREPC

#define ACE_PSOS_HAS_TIME

#define ACE_HAS_STRDUP_EMULATION

#define ACE_HAS_IP_MULTICAST

#define ACE_HAS_CPLUSPLUS_HEADERS

#define ACE_HAS_BROKEN_EXPLICIT_TYPECAST_OPERATOR_INVOCATION

#define ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS

#define ACE_HAS_WORKING_EXPLICIT_TEMPLATE_DESTRUCTOR

# define ACE_HAS_BROKEN_NOOP_DTORS

# define ACE_HAS_DIRENT
# define ACE_LACKS_READDIR_R
# define ACE_LACKS_TELLDIR
# define ACE_LACKS_SEEKDIR
# define ACE_LACKS_REWINDDIR

#define ACE_LACKS_TRUNCATE

// Template instantiation definitions
// #define ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_FILE
#define ACE_TEMPLATES_REQUIRE_SOURCE

// IO Streams stuff
#define ACE_LACKS_IOSTREAM_TOTALLY
#define ACE_LACKS_ACE_IOSTREAM
#define ACE_LACKS_IOSTREAM_FX
#define ACE_DEFAULT_LOG_STREAM 0
#define ostream FILE

#define ACE_LACKS_HRTIME_T

// #define ACE_LACKS_EVENT_T

#define ACE_HAS_VERBOSE_NOTSUP

#define ACE_LACKS_MEMORY_H

// #define ACE_LACKS_MALLOC_H

#define ACE_LACKS_MMAP

#define ACE_LACKS_UNIX_DOMAIN_SOCKETS

#define ACE_HAS_NONSTATIC_OBJECT_MANAGER

#define ACE_LACKS_SEMBUF_T

#define ACE_LACKS_EXEC

#define ACE_LACKS_FORK

#define ACE_LACKS_WRITEV

#define ACE_LACKS_READV

// rename the main entry point
#define ACE_MAIN extern "C" void root

// All this was commented out for the single threaded port

#define ACE_HAS_THREADS

#if !defined (ACE_MT_SAFE)
# define ACE_MT_SAFE 1
#endif

#define ACE_HAS_TSS_EMULATION

#define ACE_DEFAULT_THREAD_KEYS 256

#define ACE_LACKS_COND_T

// #define ACE_HAS_TSS_EMULATION

#define ACE_LACKS_PTHREAD_THR_SIGSETMASK

/* #define ACE_HAS_BROKEN_SENDMSG */

/* #define ACE_HAS_BROKEN_WRITEV  */

#define ACE_HAS_BROKEN_CONVERSIONS

#define ACE_HAS_CHARPTR_SOCKOPT

#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

#define ACE_HAS_MSG

#define ACE_HAS_POSIX_NONBLOCK

#define ACE_HAS_SIGINFO_T
#define ACE_LACKS_SIGINFO_H
#define ACE_SIGINFO_IS_SIGINFO_T
#define ACE_LACKS_SIGSET_DEFINITIONS

#define ACE_HAS_SIGWAIT

#define ACE_HAS_SIG_ATOMIC_T

#define ACE_HAS_STRERROR

#define ACE_LACKS_ACCESS

#define ACE_LACKS_GETHOSTENT

#define ACE_LACKS_GETSERVBYNAME

#define ACE_LACKS_KEY_T

#define ACE_LACKS_LINEBUFFERED_STREAMBUF

#define ACE_LACKS_LONGLONG_T

#define ACE_LACKS_LSTAT

#define ACE_LACKS_MADVISE

#define ACE_LACKS_MKTEMP

#define ACE_LACKS_MPROTECT

#define ACE_LACKS_MSYNC

#define ACE_LACKS_PARAM_H

#define ACE_LACKS_PWD_FUNCTIONS

#define ACE_LACKS_READLINK

#define ACE_LACKS_RLIMIT

#define ACE_LACKS_RWLOCK_T

#define ACE_LACKS_SBRK

#define ACE_LACKS_SIGNED_CHAR

#define ACE_LACKS_SI_ADDR

#define ACE_LACKS_SOCKETPAIR

#define ACE_LACKS_STRCASECMP

#define ACE_LACKS_STRRECVFD

#define ACE_LACKS_SYSCALL

#define ACE_LACKS_SYSV_MSG_H

#define ACE_LACKS_SYSV_SHMEM

#define ACE_LACKS_SYS_NERR

#define ACE_LACKS_SYS_TYPES_H

#define ACE_LACKS_TIMESPEC_T

#define ACE_LACKS_UCONTEXT_H

#define ACE_LACKS_UNIX_SIGNALS

// #define ACE_LACKS_SYSTIME_H

#define ACE_PAGE_SIZE 4096

#if !defined (ACE_NTRACE)
# define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#define ACE_PSOS_CANT_USE_SYS_TYPES

// #define ACE_PSOS_LACKS_PHILE

#define ACE_PSOS_HAS_C_LIBRARY

#define ACE_PSOS_SNARFS_HEADER_INFO

#define ACE_PSOS_LACKS_ARGC_ARGV

#if !defined (ACE_PSOS)
#define ACE_PSOS
#endif /* ACE_PSOS */

#define ACE_MALLOC_ALIGN 8

#if !defined (ACE_PSOS_DIAB)
#define ACE_PSOS_DIAB
#endif /* ACE_PSOS_DIAB */
#define ACE_LACKS_MKFIFO
#include "ace/post.h"
#endif /* ACE_CONFIG_H */
