/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for SunOS 5.4
// platforms using the Centerline 2.x C++ compiler.

#if !defined (ACE_CONFIG_H)
#define ACE_CONFIG_H

#define ACE_HAS_UNICODE

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC			

// Sun has the wrong prototype for sendmsg.
#define ACE_HAS_BROKEN_SENDMSG

// The SunOS 5.x version of rand_r is inconsistent with the header files...
#define ACE_HAS_BROKEN_RANDR

// Platform supports system configuration information.
#define ACE_HAS_SYSINFO

// Platform supports the POSIX regular expression library
#define ACE_HAS_REGEX

// Platform supports recvmsg and sendmsg.
#define ACE_HAS_MSG

// Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYSCALL_H

// Platform supports reentrant functions (i.e., all the POSIX *_r functions).
#define ACE_HAS_REENTRANT_FUNCTIONS

// Platform has terminal ioctl flags like TCGETS and TCSETS.
#define ACE_HAS_TERM_IOCTLS

// Compiler/platform correctly calls init()/fini() for shared libraries.
#define ACE_HAS_AUTOMATIC_INIT_FINI

// Platform supports POSIX O_NONBLOCK semantics.
#define ACE_HAS_POSIX_NONBLOCK

// Compiler/platform has correctly prototyped header files.
#define ACE_HAS_CPLUSPLUS_HEADERS

// Compiler/platform supports SunOS high resolution timers.
#define ACE_HAS_HI_RES_TIMER

// Platform supports IP multicast
#define ACE_HAS_IP_MULTICAST

// Sockets may be called in multi-threaded programs.
#define ACE_HAS_MT_SAFE_SOCKETS

// Platform contains <poll.h>.
#define ACE_HAS_POLL

// Platform supports POSIX timers via timestruc_t.
#define ACE_HAS_POSIX_TIME
#define ACE_HAS_SVR4_TIME

// Platform supports the /proc file system.
#define ACE_HAS_PROC_FS

// Platform supports the prusage_t struct.
#define ACE_HAS_PRUSAGE_T

// Compiler/platform defines the sig_atomic_t typedef.
#define ACE_HAS_SIG_ATOMIC_T

// Platform supports SVR4 extended signals.
#define ACE_HAS_SIGINFO_T
#define ACE_HAS_UCONTEXT_T

// Compiler/platform provides the sockio.h file.
#define ACE_HAS_SOCKIO_H

// Compiler supports the ssize_t typedef.
#define ACE_HAS_SSIZE_T

// Platform supports STREAMS.
#define ACE_HAS_STREAMS

// Platform supports STREAM pipes.
#define ACE_HAS_STREAM_PIPES

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// Compiler/platform supports struct strbuf.
#define ACE_HAS_STRBUF_T

// Compiler/platform supports SVR4 dynamic linking semantics.
#define ACE_HAS_SVR4_DYNAMIC_LINKING

// Andreas Ueltschi tells me this is a good thing...
#define ACE_HAS_SVR5_GETTIMEOFDAY

// Compiler/platform supports poll().
#define ACE_HAS_SVR4_POLL

// Compiler/platform supports SVR4 signal typedef.
#define ACE_HAS_SVR4_SIGNAL_T

// Compiler/platform supports SVR4 ACE_TLI (in particular, T_GETNAME stuff)...
#define ACE_HAS_SVR4_TLI

// Platform provides <sys/filio.h> header.
#define ACE_HAS_SYS_FILIO_H

// Compiler/platform supports sys_siglist array.
#define ACE_HAS_SYS_SIGLIST

/* Turn off the following four defines if you want to disable threading. */
// Compile using multi-thread libraries.
#define ACE_MT_SAFE

// Platform supports threads.
#define ACE_HAS_THREADS

// Platform supports Solaris threads.
#define ACE_HAS_STHREADS

// Compiler/platform has thread-specific storage
#define ACE_HAS_THREAD_SPECIFIC_STORAGE

// Platform supports ACE_TLI timod STREAMS module.
#define ACE_HAS_TIMOD_H

// Platform supports ACE_TLI tiuser header.
#define ACE_HAS_TIUSER_H

// Platform provides ACE_TLI function prototypes.
#define ACE_HAS_TLI_PROTOTYPES

// Platform supports ACE_TLI.
#define ACE_HAS_TLI

// 10 millisecond fudge factor to account for Solaris timers...
#if !defined (ACE_TIMER_SKEW)
#define ACE_TIMER_SKEW 1000 * 10
#endif /* ACE_TIMER_SKEW */

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */

// Defines the page size of the system.
#define ACE_PAGE_SIZE 4096

#endif /* ACE_CONFIG_H */
