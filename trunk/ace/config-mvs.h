/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for MVS with
// OpenEdition.

#if !defined (ACE_CONFIG_H)
#define ACE_CONFIG_H

// log is defined as a macro on MVS in math.h. :-(
#if defined (log)
#undef log
#endif /* log */

#define ACE_LACKS_TCP_H

// Platform supports getpagesize() call.
#define ACE_HAS_GETPAGESIZE

// highest possible errno.
#define ERRMAX __sys_nerr

// Platform does not define sys_nerr
#define ACE_LACKS_SYS_NERR

// The following #defines are hacks to get around things
// that seem to be missing or different in MVS land
#define MAXPATHLEN 1024          /* sys/param.h not on MVS */
#define NSIG 44                  /* missing from Signal.h */
#define MAXHOSTNAMELEN 256       /* missing form netdb.h */
#define howmany __howmany        /* MVS uses different names than most others */
#define fd_mask __fd_mask
#define MAXNAMLEN  __DIR_NAME_MAX

// Compiler must see template source.
#define ACE_TEMPLATES_REQUIRE_SOURCE

// Compiler/platform uses macro for ctime
#define ACE_HAS_BROKEN_CTIME

// Compiler requires extern "C" functions for signals.
#define ACE_HAS_SIG_C_FUNC

// Prototypes for both signal() and struct sigaction are consistent.
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

// Compiler/platform has correctly prototyped header files.
#define ACE_HAS_CPLUSPLUS_HEADERS

// Compiler supports the getrusage() system call
#define ACE_HAS_GETRUSAGE

// The pthread_create() routine *must* take extern C functions.
#define ACE_HAS_THR_C_FUNC

// The pthread_keycreate() routine *must* take extern C functions.
#define ACE_HAS_THR_C_DEST

// The rusage_t structure has only two fields
#define ACE_HAS_LIMITED_RUSAGE_T

// Platform supports IP MULTICAST
#define ACE_HAS_MULTICAST

// Platform supports recvmsg and sendmsg.
#define ACE_HAS_MSG

// sigwait() takes on argument
#define ACE_HAS_ONEARG_SIGWAIT

// Compiler/platform supports poll().
#define ACE_HAS_POLL

// Platform supports POSIX O_NONBLOCK semantics.
#define ACE_HAS_POSIX_NONBLOCK

// Platform supports POSIX timers via timestruc_t.
#define ACE_HAS_POSIX_TIME

// Platform supports POSIX threads
#define ACE_HAS_PTHREADS

#define ACE_LACKS_RWLOCK_T

// Platform has pthread_condattr_setkind_np()
#define ACE_HAS_PTHREAD_CONDATTR_SETKIND_NP

// Platform has pthread_mutexattr_setkind_np()
#define ACE_HAS_PTHREAD_MUTEXATTR_SETKIND_NP

// Platform has pthread_equal()
#define ACE_HAS_PTHREAD_EQUAL

// Platform has struct pthread_t
#define ACE_HAS_PTHREAD_T

// pthread_getspecific() takes a data pointer for 2nd argument
#define ACE_HAS_PTHREAD_GETSPECIFIC_DATAPTR

// pthread_attr_setdetachstate() takes pointer to 2nd arg
#define ACE_HAS_PTHREAD_DSTATE_PTR

// Platform has sigwait prototype
#define ACE_HAS_SIGWAIT

// Compiler/platform defines the sig_atomic_t typedef
#define ACE_HAS_SIG_ATOMIC_T

// Compiler has siginfo_t typedef
#define ACE_HAS_SIGINFO_T

// Platform supports inet_addr len field.
#define ACE_HAS_SIN_LEN

// Compiler uses size_t rather than in for socket lenghts
#define ACE_HAS_SIZET_SOCKET_LEN

// Compiler supports the ssize_t typedef.
#define ACE_HAS_SSIZE_T

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// Platform has strings.h
#define ACE_HAS_STRINGS

// Platform supports System V IPC
#define ACE_HAS_SYSV_IPC

// Platform/compiler supports timezone * as second parameter to gettimeofday()
#define ACE_HAS_TIMEZONE_GETTIMEOFDAY

// Platform supports threads
#define ACE_HAS_THREADS

// Platform/compiler has thread-specific storage
#define ACE_HAS_THREAD_SPECIFIC_STORAGE

// Platform has the sid_t type
#define ACE_HAS_TID_T

// Platform has utime.h
#define ACE_HAS_UTIME

// Platform uses void * arg 1 for mmap()
#define ACE_HAS_VOIDPTR_MMAP

// Platform uses void * arg 4 for setsockopt()
#define ACE_HAS_VOIDPTR_SOCKOPT

// Platform requrires pthread_yield() to take a NULL
#define ACE_HAS_YIELD_VOID_PTR

// Platform doesn't have pthread_condattr_setpshared()
#define ACE_LACKS_CONDATTR_PSHARED

// iostream header lacks ipfx (), isfx (), etc., declarations
#define ACE_LACKS_IOSTREAM_FX

// Platform doesn't have pthread_key_delete()
#define ACE_LACKS_KEYDELETE

// Platform doesn't have madvise()
#define ACE_LACKS_MADVISE

// Platform doesn't have malloc.h
#define ACE_LACKS_MALLOC_H

// Platform doesn't have struct msgbuf_t
#define ACE_LACKS_MSGBUF_T

// Platform doesn't have <sys/param.h>
#define ACE_LACKS_PARAM_H

// Platform lacks pthread_sigaction
#define ACE_LACKS_PTHREAD_THR_SIGSETMASK

// Platform doesn't have pthread_setsched() and friends.
#define ACE_LACKS_SETSCHED

// Compiler doesn't have <siginfo.h>
#define ACE_LACKS_SIGINFO_H

// Platform doesn't define struct strrecvfd
#define ACE_LACKS_STRRECVFD

// Platform doesn't have pthread_attr_setscope()
#define ACE_LACKS_THREAD_PROCESS_SCOPING

// Platform doesn't have pthread_attr_setstackaddr()
#define ACE_LACKS_THREAD_STACK_ADDR

// Compiler doesn't have <ucontext.h>
#define ACE_LACKS_UCONTEXT_H

// Compile using multi-threaded libraries
#define ACE_MT_SAFE

// <time.h> doesn't automatically #include <sys/time.h>
#define ACE_NEEDS_SYSTIME_H

#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#endif /* ACE_CONFIG_H */
