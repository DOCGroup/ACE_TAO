/* -*- C++ -*- */
// $Id$

// This configuration file is designed to be included by another,
// specific configuration file.  It provides config information common
// to all CygWin platforms.  It automatically determines the CPU
// architecture, compiler (g++ or egcs), and libc (libc5/glibc/newlib),
// and configures based on those.

#ifndef ACE_CYGWIN32_COMMON_H
#define ACE_CYGWIN32_COMMON_H
#include "ace/pre.h"

#define CYGWIN32

// We trust this file will get included before <sys/types.h>
#if !defined(FD_SETSIZE)
# define FD_SETSIZE 1024
#endif

#if !defined (ACE_IOV_MAX)
# define ACE_IOV_MAX 64
#endif /* ACE_IOV_MAX */

// Define custom export macros for export/import of symbols from/of dll's
#define ACE_HAS_CUSTOM_EXPORT_MACROS
#define ACE_Proper_Export_Flag __declspec (dllexport)
#define ACE_Proper_Import_Flag __declspec (dllimport)
#define ACE_EXPORT_SINGLETON_DECLARATION(T) template class __declspec (dllexport) T
#define ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) template class __declspec (dllexport) SINGLETON_TYPE<CLASS, LOCK>;
#define ACE_IMPORT_SINGLETON_DECLARATION(T) extern template class T
#define ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) extern template class SINGLETON_TYPE <CLASS, LOCK>;

#define ACE_LACKS_UNIX_DOMAIN_SOCKETS
#define ACE_LACKS_SYSV_MSG_H
#define ACE_HAS_SIG_MACROS
#define ACE_LACKS_SYSTIME_H
#define ACE_LACKS_TELLDIR
#define ACE_LACKS_SYSV_SHMEM

#define ACE_LACKS_MMAP
#define ACE_LACKS_MPROTECT

#define ACE_LACKS_SEMBUF_T
#define ACE_LACKS_NAMED_POSIX_SEM
#define ACE_LACKS_SENDMSG
#define ACE_LACKS_RECVMSG
#define ACE_LACKS_READDIR_R
#define ACE_LACKS_RLIMIT
#define ACE_LACKS_SOCKETPAIR
#define ACE_LACKS_SEEKDIR
#define ACE_LACKS_MKTEMP

#if ! defined (__ACE_INLINE__)
# define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

// Needed to differentiate between libc 5 and libc 6 (aka glibc).
// It's there on all libc 5 systems I checked.
#include /**/ <features.h>


// First the machine specific part
// #define ACE_HAS_CYGWIN32_SOCKET_H

// Then the compiler specific parts

// config-g++-common.h undef's ACE_HAS_STRING_CLASS with -frepo, so
// this must appear before its #include.
#define ACE_HAS_STRING_CLASS

#if defined (__GNUG__)
# include "ace/config-g++-common.h"
#else
# error unsupported compiler in ace/config-cygwin32-common.h
#endif /* __GNUG__ */


// Completely common part :-)

// Platform/compiler has the sigwait(2) prototype
#define ACE_HAS_SIGWAIT
#define ACE_HAS_POSIX_TIME
#define ACE_LACKS_TIMESPEC_T
#define ACE_HAS_MSG
#define ACE_DEFAULT_BASE_ADDR ((char *) 0x80000000)
#define ACE_LACKS_SETREUID
#define ACE_LACKS_SETREGID
#define ACE_LACKS_GETHOSTENT
#define ACE_HAS_NONCONST_SELECT_TIMEVAL
#define ACE_LACKS_GETPGID_PROTOTYPE

// Compiler/platform supports alloca().
// Although ACE does have alloca() on this compiler/platform combination, it is
// disabled by default since it can be dangerous.  Uncomment the following line
// if you ACE to use it.
//#define ACE_HAS_ALLOCA

// Compiler/platform has the getrusage() system call.
#define ACE_HAS_GETRUSAGE
#define ACE_HAS_GETRUSAGE_PROTO

#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

// Optimize ACE_Handle_Set for select().
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

#if !defined(ACE_HAS_IP_MULTICAST)
# define ACE_HAS_IP_MULTICAST
#endif /* #if ! defined(ACE_HAS_IP_MULTICAST) */

#define ACE_HAS_BIG_FD_SET

#define ACE_HAS_TERM_IOCTLS

#define ACE_DEFAULT_MAX_SOCKET_BUFSIZ 65535

#define ACE_DEFAULT_SELECT_REACTOR_SIZE 256

#define ACE_HAS_GETPAGESIZE 1

// Platform lacks POSIX prototypes for certain System V functions
// like shared memory and message queues.
#define ACE_LACKS_SOME_POSIX_PROTOTYPES


#define ACE_LACKS_STRRECVFD

#define ACE_LACKS_MSYNC
#define ACE_LACKS_MADVISE

#define ACE_HAS_SUNOS4_GETTIMEOFDAY

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// Compiler supports the ssize_t typedef.
#define ACE_HAS_SSIZE_T

// Compiler/platform defines the sig_atomic_t typedef.
#define ACE_HAS_SIG_ATOMIC_T

#define ACE_HAS_GPERF

#define ACE_HAS_DIRENT
#define ACE_HAS_STRPTIME

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
# define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#define ACE_LACKS_MKFIFO

#include "ace/post.h"
#endif /* ACE_CYGWIN32_COMMON_H */
