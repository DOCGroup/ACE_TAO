/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for the SGI
// Indigo2EX running Irix 5.3 platform using the GNU C++ Compiler

#if !defined (ACE_CONFIG_H)
#define ACE_CONFIG_H

// Platform supports getpagesize() call.
#define ACE_HAS_GETPAGESIZE

#define IRIX5
#define ACE_HAS_SIGWAIT

#define ACE_TEMPLATES_REQUIRE_SOURCE

#define ACE_TEMPLATES_REQUIRE_SPECIALIZATION

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC			

// Platform requires void * for mmap().
#define ACE_HAS_VOIDPTR_MMAP

// Platform supports recvmsg and sendmsg.
#define ACE_HAS_MSG

// Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYSCALL_H

// Compiler/platform supports alloca()
#define ACE_HAS_ALLOCA 

// Compiler/platform has <alloca.h>
#define ACE_HAS_ALLOCA_H

// IRIX5 needs to define bzero() in this odd file <bstring.h>
#define ACE_HAS_BSTRING

// Compiler/platform has the getrusage() system call.
#define ACE_HAS_GETRUSAGE

// Denotes that IRIX 5.3 has second argument to gettimeofday()
// which is variable ...
#define ACE_HAS_IRIX_GETTIMEOFDAY

// Denotes that GNU has cstring.h as standard
// which redefines memchr()
#define ACE_HAS_GNU_CSTRING_H

// Compiler doesn't support static data member templates.
#define ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES

// Platform supports POSIX O_NONBLOCK semantics.
#define ACE_HAS_POSIX_NONBLOCK 

// Platform supports POSIX timers via timestruc_t.
#define ACE_HAS_POSIX_TIME 
#define ACE_HAS_SVR4_TIME

// Compiler/platform has correctly prototyped header files.
#define ACE_HAS_CPLUSPLUS_HEADERS 

// Platform contains <poll.h>.
#define ACE_HAS_POLL 

// Platform supports the /proc file system.
#define ACE_HAS_PROC_FS 

// Explicit dynamic linking permits "lazy" symbol resolution.
#define ACE_HAS_RTLD_LAZY_V 

// Compiler/platform defines the sig_atomic_t typedef.
#define ACE_HAS_SIG_ATOMIC_T 

// Platform supports SVR4 extended signals.
#define ACE_HAS_SIGINFO_T 
#define ACE_HAS_UCONTEXT_T 

// Compiler supports the ssize_t typedef.
#define ACE_HAS_SSIZE_T 

// Platform supports STREAMS.
#define ACE_HAS_STREAMS 

// Platform supports STREAM pipes (note that this is disabled by
// default, see the manual page on pipe(2) to find out how to enable
// it). 
// #define ACE_HAS_STREAM_PIPES 

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR 

// Compiler/platform supports struct strbuf.
#define ACE_HAS_STRBUF_T 

// Compiler/platform supports SVR4 dynamic linking semantics.
#define ACE_HAS_SVR4_DYNAMIC_LINKING 

// Compiler/platform supports SVR4 signal typedef.
#define ACE_HAS_IRIX_53_SIGNALS 

// Compiler/platform supports sys_siglist array.
#define ACE_HAS_SYS_SIGLIST 

// Platform provides <sys/filio.h> header.
#define ACE_HAS_SYS_FILIO_H 

// Compiler/platform defines a union semun for SysV shared memory.
#define ACE_HAS_SEMUN 

// Platform supports IP multicast
#define ACE_HAS_IP_MULTICAST

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#endif /* ACE_CONFIG_H */
