/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for SunOS4.1.4
// platforms using the SunC++ 4.x compiler.

#if !defined (ACE_CONFIG_H)
#define ACE_CONFIG_H

// Platform supports pread() and pwrite()
#define ACE_HAS_P_READ_WRITE

#define ACE_HAS_CHARPTR_SPRINTF
#define ACE_NEEDS_SYSTIME_H
#define ACE_HAS_UNION_WAIT

// Special addition to handle sunOS 4.1 which is unable to
// handle POSIX Prototypes !
#define ACE_LACKS_POSIX_PROTO

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC			

// Platform supports recvmsg and sendmsg.
#define ACE_HAS_MSG

// Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYSCALL_H

// Compiler/platform has the getrusage() system call.
#define ACE_HAS_GETRUSAGE

// Compiler/platform supports strerror ().
// #define ACE_HAS_STRERROR
#define ACE_HAS_SYS_ERRLIST

// Sockets may be called in multi-threaded programs.
#define ACE_HAS_MT_SAFE_SOCKETS

// Header files lack t_errno for ACE_TLI.
// #define ACE_LACKS_T_ERRNO

// Compiler/platform uses old malloc()/free() prototypes (ugh).
#define ACE_HAS_OLD_MALLOC

// Defines the page size of the system.
#define ACE_PAGE_SIZE 4096

// Compiler/platform supports poll().
#define ACE_HAS_POLL

// Compiler/platform defines a union semun for SysV shared memory.
#define ACE_HAS_SEMUN

// Compiler/platform provides the sockio.h file.
#define ACE_HAS_SOCKIO_H

// Compiler has brain-damaged SPARCwork signal prototype...
#define ACE_HAS_SPARCWORKS_401_SIGNALS

// Compiler/platform supports struct strbuf
#define ACE_HAS_STRBUF_T

// Platform supports STREAMS.
#define ACE_HAS_STREAMS

// SunOS 4 style prototype.
#define ACE_HAS_SUNOS4_GETTIMEOFDAY

// Compiler/platform supports SVR4 dynamic linking semantics.
#define ACE_HAS_SVR4_DYNAMIC_LINKING

// Platform provides <sys/filio.h> header.
#define ACE_HAS_SYS_FILIO_H

// Platform supports ACE_TLI tiuser header.
// #define ACE_HAS_TIUSER_H

// Platform has ACE_TLI.
// #define ACE_HAS_TLI

#define ACE_LACKS_LINEBUFFERED_STREAMBUF
#define ACE_LACKS_SIGNED_CHAR

#define ACE_NEEDS_DEV_IO_CONVERSION

// 10 millisecond fudge factor to account for Solaris timers...
#if !defined (ACE_TIMER_SKEW)
#define ACE_TIMER_SKEW 1000 * 10
#endif /* ACE_TIMER_SKEW */

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#endif /* ACE_CONFIG_H */
