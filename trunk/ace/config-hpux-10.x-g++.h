/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for HP
// platforms running HP/UX 10.x using G++.

#if !defined (ACE_CONFIG_H)
#define ACE_CONFIG_H

// Compiling for HPUX.
#if !defined (HPUX)
#define HPUX
#endif /* HPUX */

#define ACE_HAS_BROKEN_CONVERSIONS
#define ACE_HAS_STREAMS
#define ACE_HAS_STRING_CLASS

// They forgot a const in the prototype of putmsg and putpmsg...
#define ACE_LACKS_CONST_STRBUF_PTR

// Platform defines struct timespec, but not timespec_t
#define ACE_LACKS_TIMESPEC_T

// Compiler's template mechanism must see source code (i.e., .C files).
#define ACE_TEMPLATES_REQUIRE_SOURCE
// Compiler doesn't support static data member templates.
#define ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES
// Must specialize templates due to G++'s lame parameterized type
// support... I added this  Afara
#define ACE_HAS_TEMPLATE_SPECIALIZATION
#define ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION

#define ACE_LACKS_SYSCALL
#define ACE_HAS_POSIX_TIME
#define ACE_HAS_CLOCK_GETTIME

// Compiler/platform supports struct strbuf.
#define ACE_HAS_STRBUF_T
#define ACE_HAS_STREAM_PIPES
#define ACE_HAS_STREAMS

// Platform supports getpagesize() call.
#define ACE_HAS_GETPAGESIZE

// Platform supports reentrant functions (i.e., all the POSIX *_r
// functions).
#define ACE_HAS_REENTRANT_FUNCTIONS

// Platform supports the tid_t type (e.g., AIX and Irix 6.2)
#define ACE_HAS_TID_T

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC
#define ACE_HAS_IP_MULTICAST

// Platform supports recvmsg and sendmsg.
#define ACE_HAS_MSG

// Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYSCALL_H

// Fixes a problem with HP/UX not wrapping the mmap(2) header files
// with extern "C".
#define ACE_HAS_BROKEN_MMAP_H

// Prototypes for both signal() and struct sigaction are consistent.
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

// Compiler/platform has correctly prototyped header files.
#define ACE_HAS_CPLUSPLUS_HEADERS

// Header files lack t_errno for ACE_TLI.
#define ACE_LACKS_T_ERRNO

// Compiler/platform supports poll().
#define ACE_HAS_POLL

// Platform supports POSIX O_NONBLOCK semantics.
#define ACE_HAS_POSIX_NONBLOCK

// Compiler/platform defines the sig_atomic_t typedef
#define ACE_HAS_SIG_ATOMIC_T

// ACE supports POSIX Pthreads.
//#define ACE_HAS_DCETHREADS   //changed by Afara 09/24/96

// Compiler supports the ssize_t typedef.
#define ACE_HAS_SSIZE_T

// Defines the page size of the system.
#define ACE_PAGE_SIZE 4096

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// ???
#define ACE_HAS_SUNOS4_GETTIMEOFDAY

// HP/UX has an undefined syscall for GETRUSAGE...
#define ACE_HAS_SYSCALL_GETRUSAGE

// Note, this only works if the flag is set above!
#define ACE_HAS_GETRUSAGE

#define ACE_HAS_SIGINFO_T
#define ACE_HAS_UCONTEXT_T
#define _CLOCKID_T
#define ACE_LACKS_SI_ADDR

// Platform has the XLI version of ACE_TLI.
// #define ACE_HAS_XLI

// Platform supports ACE_TLI tiuser header.
#define ACE_HAS_TIUSER_H

// Platform provides ACE_TLI function prototypes.
#define ACE_HAS_TLI_PROTOTYPES

// Platform supports ACE_TLI.
#define ACE_HAS_TLI

// Optimize ACE_Handle_Set for select().
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */
#define ACE_HAS_UALARM
#endif /* ACE_CONFIG_H */
