/* -*- C++ -*- */
// $Id$

// for SunOS4 platforms using the GNU g++ compiler

#if !defined (ACE_CONFIG_H)
#define ACE_CONFIG_H

#define ACE_NEEDS_SYSTIME_H

// Must specialize templates due to G++'s lame parameterized type
// support...
#define ACE_TEMPLATES_REQUIRE_SPECIALIZATION

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC

// Platform supports recvmsg and sendmsg.
#define ACE_HAS_MSG

// Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYSCALL_H

// Compiler's template mechanism must see source code (i.e., .C files).
#define ACE_TEMPLATES_REQUIRE_SOURCE

// Compiler/platform has the getrusage() system call.
#define ACE_HAS_GETRUSAGE

// Compiler/platform supports strerror ().
// #define ACE_HAS_STRERROR
#define ACE_HAS_SYS_ERRLIST

// Sockets may be called in multi-threaded programs.
#define ACE_HAS_MT_SAFE_SOCKETS

// Header files lack t_errno for ACE_TLI.
// #define ACE_LACKS_T_ERRNO

// Compiler doesn't support static data member templates.
#define ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES

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

// Compiler has system V signals
#define ACE_HAS_SVR4_SIGNAL_T

// Compiler/platform supports struct strbuf
#define ACE_HAS_STRBUF_T

// Platform supports STREAMS.
#define ACE_HAS_STREAMS

// Compiler/platform supports SVR4 dynamic linking semantics.
#define ACE_HAS_SVR4_DYNAMIC_LINKING

// Platform provides <sys/filio.h> header.
#define ACE_HAS_SYS_FILIO_H
// Platform supports ACE_TLI tiuser header.
// #define ACE_HAS_TIUSER_H

// Platform has ACE_TLI.
// #define ACE_HAS_TLI

#define ACE_HAS_SUNOS4_SIGNAL_T
#define ACE_HAS_CPLUSPLUS_HEADERS
#define ACE_HAS_RTLD_LAZY_V
#if (__GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 6))
#define ACE_HAS_SYSENT_H
#endif
#define ACE_HAS_ALLOCA
// Compiler/platform has <alloca.h>
#define ACE_HAS_ALLOCA_H
#define ACE_HAS_SVR4_GETTIMEOFDAY
// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */


#endif /* ACE_CONFIG_H */
