/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for the SGI
// Indigo2EX running Irix 5.2 platform using the gcc v2.6.x compiler
// and libg++ v2.6.x.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H
#include /**/ "ace/pre.h"

// Platform supports STREAM pipes (note that this is disabled by
// default, see the manual page on pipe(2) to find out how to enable
// it).
//#define ACE_HAS_STREAM_PIPES

// Platform supports getpagesize() call.
#define ACE_HAS_GETPAGESIZE
// Optimize ACE_Handle_Set for select().
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

#define ACE_HAS_SIGWAIT

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC

// Platform requires void * for mmap().
#define ACE_HAS_VOIDPTR_MMAP

// Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYSCALL_H

// Platform supports recvmsg and sendmsg.
#define ACE_HAS_MSG

#define ACE_SIZEOF_LONG_DOUBLE 8

#define IRIX5
// Although ACE does have alloca() on this compiler/platform combination, it is
// disabled by default since it can be dangerous.  Uncomment the following line
// if you ACE to use it.
//#define ACE_HAS_ALLOCA
// Compiler/platform has <alloca.h>
#define ACE_HAS_ALLOCA_H
#define ACE_HAS_BSTRING
#define ACE_HAS_GETRUSAGE
#define ACE_HAS_POSIX_NONBLOCK
#define ACE_HAS_POSIX_TIME
#define ACE_HAS_CPLUSPLUS_HEADERS
#define ACE_HAS_POLL
#define ACE_HAS_PROC_FS
#define ACE_HAS_SIG_ATOMIC_T
#define ACE_HAS_SIGINFO_T
#define ACE_HAS_UCONTEXT_T
#define ACE_HAS_STREAMS
#define ACE_HAS_SSIZE_T
#define ACE_HAS_STRERROR
#define ACE_HAS_STRBUF_T
#define ACE_HAS_SVR4_DYNAMIC_LINKING
#define ACE_HAS_SVR4_SIGNAL_T
#define ACE_HAS_SYS_SIGLIST
#define ACE_HAS_SYS_FILIO_H
#define ACE_HAS_SEMUN
#define ACE_NEEDS_DEV_IO_CONVERSION

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
# define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_H */
