/* -*- C++ -*- */
// $Id$

// ***** This configuration file is still under debugging. *****
// ***** It still doesn't work quite the way it expected to. *****

// The following configuration file is designed to work for FreeBSD
// platforms using GNU C++ and the POSIX (pthread) threads package
// by John Birrell.  You must at least have 3.0-current as of 2/9/1997 to 
// take the advantage of latest pthread library.

#if !defined (ACE_CONFIG_H)
#define ACE_CONFIG_H

#if ! defined (__ACE_INLINE__)
#define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

// Platform specific directives
// gcc defines __FreeBSD__ automatically for us.
#define _THREAD_SAFE
#define ACE_LACKS_GETPGID
#define ACE_LACKS_RWLOCK_T
#define ACE_HAS_SIG_MACROS
#define ACE_HAS_CHARPTR_DL
#define ACE_USES_ASM_SYMBOL_IN_DLSYM	  
#define ACE_LACKS_SIGSET  

// sched.h still not fully support on FreeBSD ?
// this is taken from /usr/src/lib/libc_r/uthread/pthread-private.h
enum schedparam_policy {
        SCHED_RR,
        SCHED_IO,
        SCHED_FIFO,
        SCHED_OTHER
};

// Platform supports POSIX timers via timestruct_t.
#define ACE_HAS_POSIX_TIME
#define ACE_NEEDS_SYSTIME_H

#define ACE_LACKS_STRRECVFD

#define ACE_HAS_SIN_LEN

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC			

// Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYSCALL_H

#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

// Compiler/platform supports SVR4 signal typedef
#define ACE_HAS_SVR4_SIGNAL_T

// Compiler/platform supports alloca().
#define ACE_HAS_ALLOCA

// Compiler/platform supports SVR4 dynamic linking semantics..
#define ACE_HAS_SVR4_DYNAMIC_LINKING

// Compiler/platform correctly calls init()/fini() for shared libraries.
#define ACE_HAS_AUTOMATIC_INIT_FINI

// Explicit dynamic linking permits "lazy" symbol resolution
#define ACE_HAS_RTLD_LAZY_V

// platform supports POSIX O_NONBLOCK semantics
#define ACE_HAS_POSIX_NONBLOCK

// platform supports IP multicast
#define ACE_HAS_IP_MULTICAST

// ***** NOT SURE YET ***** However, this doesn't seem to be
// used any where, so I just put it in.  ;0
// Sockets may be called in multi-threaded programs.
#define ACE_HAS_MT_SAFE_SOCKETS

// Compiler/platform has <alloca.h>
//#define ACE_HAS_ALLOCA_H

// Compiler/platform has the getrusage() system call.
#define ACE_HAS_GETRUSAGE

// Compiler/platform defines the sig_atomic_t typedef.
#define ACE_HAS_SIG_ATOMIC_T

// Compiler/platform supports sys_siglist array.
// *** This refers to (_sys_siglist) instead of (sys_siglist)
// #define ACE_HAS_SYS_SIGLIST

// Compiler/platform defines a union semun for SysV shared memory.
#define ACE_HAS_SEMUN

// Compiler supports the ssize_t typedef.
#define ACE_HAS_SSIZE_T

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// Compiler/platform provides the sockio.h file.
#define ACE_HAS_SOCKIO_H

// Defines the page size of the system.
#define ACE_PAGE_SIZE 4096

// Platform provides <sys/filio.h> header.
#define ACE_HAS_SYS_FILIO_H

// Compiler/platform supports SVR4 gettimeofday() prototype
#define ACE_HAS_SUNOS4_GETTIMEOFDAY
// #define ACE_HAS_TIMEZONE_GETTIMEOFDAY

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#define ACE_HAS_MSG
#define ACE_HAS_4_4BSD_SENDMSG_RECVMSG

// TDN - adapted from file for SunOS4 platforms using the GNU g++ compiler
// Compiler's template mechanism must see source code (i.e., .C files).
#define ACE_TEMPLATES_REQUIRE_SOURCE

#define ACE_HAS_TEMPLATE_SPECIALIZATION
#define ACE_TEMPLATES_REQUIRE_SPECIALIZATION

// Compiler doesn't support static data member templates.
#define ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES

// Thread specific settings
// Yes, we do have threads.
#define ACE_HAS_THREADS
// And they're even POSIX pthreads
#if !defined (ACE_MT_SAFE)
	#define ACE_MT_SAFE 1
#endif
#define ACE_HAS_PTHREADS
#define ACE_LACKS_SETSCHED
#define ACE_LACKS_THREAD_PROCESS_SCOPING
#define ACE_LACKS_PTHREAD_THR_SIGSETMASK
#define ACE_LACKS_CONDATTR_PSHARED
// this one is in doubt
#define ACE_HAS_THREAD_SPECIFIC_STORAGE

// #define ACE_HAS_SIGWAIT

#endif /* ACE_CONFIG_H */
