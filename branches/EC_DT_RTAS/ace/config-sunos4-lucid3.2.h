/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work
// for SunOS4 platforms using the Lucid 3.2 compiler.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H
#include /**/ "ace/pre.h"

#define ACE_LACKS_GETPGID
#define ACE_LACKS_SETPGID

#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

#define ACE_HAS_SYS_ERRLIST
#define ACE_HAS_CHARPTR_SPRINTF
#define ACE_HAS_UNION_WAIT

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC

// Platform supports recvmsg and sendmsg.
#define ACE_HAS_MSG

// Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYSCALL_H

// Compiler/platform supports alloca()
// Although ACE does have alloca() on this compiler/platform combination, it is
// disabled by default since it can be dangerous.  Uncomment the following line
// if you ACE to use it.
//#define ACE_HAS_ALLOCA

// Compiler/platform has <alloca.h>
#define ACE_HAS_ALLOCA_H

// Compiler/platform has correctly prototyped header files.
#define ACE_HAS_CPLUSPLUS_HEADERS

// Compiler/platform has the getrusage() system call.
#define ACE_HAS_GETRUSAGE

// Header files lack t_errno for ACE_TLI.
// #define ACE_LACKS_T_ERRNO

// Defines the page size of the system.
#define ACE_PAGE_SIZE 4096

// Compiler/platform supports poll().
#define ACE_HAS_POLL

// Compiler/platform defines a union semun for SysV shared memory.
#define ACE_HAS_SEMUN

// Compiler/platform provides the sockio.h file.
#define ACE_HAS_SOCKIO_H

// Compiler/platform supports struct strbuf
#define ACE_HAS_STRBUF_T

// Platform supports STREAMS.
#define ACE_HAS_STREAMS

// Compiler/platform supports strerror ().
// #define ACE_HAS_STRERROR

// SunOS 4 style prototype.
#define ACE_HAS_SUNOS4_GETTIMEOFDAY

// Compiler/platform supports SVR4 dynamic linking semantics.
#define ACE_HAS_SVR4_DYNAMIC_LINKING

// Platform provides <sysent.h> header.
#define ACE_HAS_SYSENT_H

// Platform provides <sys/filio.h> header.
#define ACE_HAS_SYS_FILIO_H

// Platform supports ACE_TLI tiuser header.
// #define ACE_HAS_TIUSER_H

// Platform has ACE_TLI.
// #define ACE_HAS_TLI

#define ACE_NEEDS_DEV_IO_CONVERSION

#define ACE_LACKS_U_LONGLONG_T

#define ACE_LACKS_DIFFTIME

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_H */
