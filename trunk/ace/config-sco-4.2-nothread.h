/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for SCO UNIX
// version 4.2 without threads.

#if !defined (ACE_CONFIG_H)
#define ACE_CONFIG_H

// Compiling for SCO.
#if !defined (SCO)
#define SCO
#endif /* SCO */

#if ! defined (__ACE_INLINE__)
#define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

#if defined (SCO) && !defined (MAXPATHLEN)
#define MAXPATHLEN 1023
#endif /* SCO */

#define ACE_TEMPLATES_REQUIRE_SOURCE
#define ACE_TEMPLATES_REQUIRE_SPECIALIZATION
#define ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES

#define ACE_LACKS_UNIX_DOMAIN_SOCKETS
#define ACE_LACKS_SYSCALL
#define ACE_LACKS_STRRECVFD
#define ACE_LACKS_MMAP
#define ACE_LACKS_SOCKETPAIR
#define ACE_HAS_SEMUN
#define ACE_LACKS_MSYNC
#define ACE_LACKS_MADVISE
#define ACE_NEEDS_WRITEV
#define ACE_NEEDS_READV
#define ACE_NEEDS_FTRUNCATE
#define ACE_LACKS_RLIMIT
#define ACE_LACKS_RECVMSG
#define ACE_LACKS_SENDMSG

// Compiler doesn't support static data member templates.
//#define ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC			

// Platform supports recvmsg and sendmsg.
//#define ACE_HAS_MSG

// Compiler/platform contains the <sys/syscall.h> file.
//#define ACE_HAS_SYSCALL_H

// Fixes a problem with HP/UX not wrapping the mmap(2) header files
// with extern "C".
//#define ACE_HAS_BROKEN_MMAP_H

// Prototypes for both signal() and struct sigaction are consistent.
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

// Compiler/platform has correctly prototyped header files.
#define ACE_HAS_CPLUSPLUS_HEADERS

// Header files lack t_errno for ACE_TLI.
//#define ACE_LACKS_T_ERRNO

// Compiler/platform supports poll().
#define ACE_HAS_POLL

// Platform supports POSIX O_NONBLOCK semantics.
#define ACE_HAS_POSIX_NONBLOCK

// Compiler/platform defines the sig_atomic_t typedef
#define ACE_HAS_SIG_ATOMIC_T

// Compiler supports the ssize_t typedef.
//#define ACE_HAS_SSIZE_T

// Defines the page size of the system.
#define ACE_PAGE_SIZE 4096

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// ???
#define ACE_HAS_SUNOS4_GETTIMEOFDAY

// HP/UX has an undefined syscall for GETRUSAGE...
//#define ACE_HAS_SYSCALL_GETRUSAGE

// Note, this only works if the flag is set above!
//#define ACE_HAS_GETRUSAGE

// Platform uses int for select() rather than fd_set.
#define ACE_SELECT_USES_INT

// Platform has prototypes for ACE_TLI.
//#define ACE_HAS_TLI_PROTOTYPES
// Platform has the XLI version of ACE_TLI.
// #define ACE_HAS_XLI

#define ACE_HAS_GNU_CSTRING_H

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#endif /* ACE_CONFIG_H */
