Return-Path: ace@svappl36.mdc.com 
Delivery-Date: Thu, 24 Oct 1996 10:50:14 -0500
Return-Path: ace@svappl36.mdc.com
Return-Path: <ace@svappl36.mdc.com>
Received: from svmail01.mdc.com by cs.wustl.edu (SMI-8.6/ECL-J1.00)
	id KAA03041; Thu, 24 Oct 1996 10:50:01 -0500
Received: from svappl36 (SVAPPL36.MDC.COM) by svmail01.mdc.com with SMTP
	(1.37.109.11/16.2) id AA215472200; Thu, 24 Oct 1996 10:50:00 -0500
Received: from huskers by svappl36 (5.x/SMI-SVR4)
	id AA25932; Thu, 24 Oct 1996 10:49:09 -0500
Received: by huskers (SMI-8.6/SMI-SVR4)
	id KAA26789; Thu, 24 Oct 1996 10:49:27 -0500
Date: Thu, 24 Oct 1996 10:49:27 -0500
From: ace@svappl36.mdc.com (Adaptive Comm Env)
Message-Id: <199610241549.KAA26789@huskers>
Content-Type: text
Apparently-To: levine@cs.wustl.edu
content-length: 1599

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
#define ACE_HAS_THREADS
#define ACE_LACKS_MADVISE
#define ACE_LACKS_MALLOC_H
#define ACE_LACKS_MKTEMP
#define ACE_LACKS_MMAP
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
#define SIGNAL_SAFE_OS_CALLS

// Defines the page size of the system.
#define ACE_PAGE_SIZE 4096


// vxWorks.h must be included before time.h, and key_t must be
// defined early also:  these are here
// because Time_Value.h #includes ace/config.h, but not ace/OS.h
typedef int key_t;
#include <vxWorks.h>


#endif /* ACE_CONFIG_H */


