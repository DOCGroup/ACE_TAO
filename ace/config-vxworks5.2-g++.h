/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for VxWorks
// 5.2 platforms using the GNU g++ 2.7.2 compiler, without repo patch

#if !defined (ACE_CONFIG_H)
#define ACE_CONFIG_H

#define ACE_HAS_BROKEN_SENDMSG
#define ACE_HAS_BROKEN_WRITEV
#define ACE_HAS_CHARPTR_SOCKOPT
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES
#define ACE_HAS_CPLUSPLUS_HEADERS
#define ACE_HAS_GNU_CSTRING_H
#define ACE_HAS_GREENHILLS_SOCKETS
#define ACE_HAS_MSG
#define ACE_HAS_MT_SAFE_SOCKETS
#define ACE_HAS_POSIX_NONBLOCK
#define ACE_HAS_POSIX_SEM
#define ACE_HAS_POSIX_TIME
#define ACE_HAS_SIG_ATOMIC_T
#define ACE_HAS_SIGINFO_T
#define ACE_HAS_SIGWAIT
#define ACE_HAS_STRERROR
#define ACE_HAS_THREADS
#define ACE_LACKS_MADVISE
#define ACE_LACKS_MALLOC_H
#define ACE_LACKS_MKTEMP
#define ACE_LACKS_MMAP
#define ACE_LACKS_MPROTECT
#define ACE_LACKS_MSYNC
#define ACE_LACKS_PARAM_H
#define ACE_LACKS_RLIMIT
#define ACE_LACKS_SBRK
#define ACE_LACKS_SEMBUF_T
#define ACE_LACKS_SIGINFO_H
#define ACE_LACKS_SI_ADDR
#define ACE_LACKS_SOCKETPAIR
#define ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES
#define ACE_LACKS_STRRECVFD
#define ACE_LACKS_SYSCALL
#define ACE_LACKS_SYSV_SHMEM
#define ACE_LACKS_UCONTEXT_H
#define ACE_LACKS_UTSNAME_T
#define ACE_MT_SAFE
#define ACE_TEMPLATES_REQUIRE_SOURCE
#define ACE_TEMPLATES_REQUIRE_SPECIALIZATION

// Defines the page size of the system.
#define ACE_PAGE_SIZE 4096

#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */

// vxWorks.h must be included before time.h, and key_t must be
// defined early also:  these are here
// because Time_Value.h #includes ace/config.h, but not ace/OS.h
typedef int key_t;
#include /**/ <vxWorks.h>


#endif /* ACE_CONFIG_H */


