#ifndef ACE_INT_CONFIG_H
#define ACE_INT_CONFIG_H

/*
 * This config.h file is for version 4.0.x of the
 * Integrity RTOS with SCA from Green Hills Software
 * http://www.ghs.com/products/rtos/integrity.html
 *
 * $Id$
 */

#define ghs
/* compilation defines */
#define ACE_LACKS_GETPGID
#define ACE_LACKS_SETPGID
#define ACE_LACKS_SETREUID
#define ACE_LACKS_SETREGID
#ifndef ACE_HAS_EXCEPTIONS
  #define ACE_HAS_EXCEPTIONS
#endif
#define ACE_NEW_THROWS_EXCEPTIONS
#define ACE_HAS_STANDARD_CPP_LIBRARY 1
#define	ACE_HAS_ANSI_CASTS
#define	ACE_HAS_TEMPLATE_SPECIALIZATION
#define ACE_HAS_ANSI_CASTS
#if 0
#define ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA
#endif
#define ACE_TEMPLATES_REQUIRE_SOURCE 1
#define ACE_HAS_TEMPLATE_TYPEDEFS
#define TAO_USE_SEQUENCE_TEMPLATES
#define ACE_HAS_USING_KEYWORD
#define ACE_NEEDS_FUNC_DEFINITIONS
#define _REENTRANT
#define ACE_MT_SAFE 1
#define __ACE_INLINE__
#define ACE_HAS_ONE_DEFINITION_RULE

// Compiler/platform has correctly prototyped header files.
#define ACE_HAS_CPLUSPLUS_HEADERS

#define ACE_HAS_SHM_OPEN

/***** Operating System Defines *****/

/***** ANSI defines *****/
#define ACE_LACKS_TEMPNAM /* believe it or not, this is ANSI C */
#define ACE_HAS_STRERROR


#define ACE_LACKS_SENDMSG

/***** End Stack Defines *****/


/* SCA STUFF */
#define ACE_HAS_SIGWAIT
#define ACE_HAS_SIGACTION
#define ACE_HAS_SIGINFO_T
#define ACE_LACKS_SIGINFO_H
#define ACE_LACKS_UCONTEXT_H
#define ACE_HAS_SIG_C_FUNC
#define ACE_LACKS_SI_ADDR
#define ACE_HAS_AIO_CALLS

#define ACE_HAS_POSIX_NONBLOCK
#define ACE_HAS_DIRENT

#define ACE_HAS_THREADS

#define ACE_HAS_PTHREADS
#define ACE_HAS_PTHREADS_STD
/***** End Threading Defines *****/

/***** ACE Specific Features *****/
#define ACE_NTRACE 1
/***** end ACE Specific Features *****/

/***** Hardware Defines *****/
#define ACE_PAGE_SIZE 4096
/***** End Hardware Defines *****/

/****** SYSV_IPC STUFF *****/
#define ACE_LACKS_KEY_T

/****** Posix Defines *****/
#define ACE_HAS_POSIX_TIME
#define ACE_HAS_POSIX_SEM
#define ACE_HAS_STRDUP_EMULATION
#define ACE_HAS_MSG
#define ACE_LACKS_CONDATTR_PSHARED
#define ACE_LACKS_EXEC
#define ACE_LACKS_FORK
#define ACE_LACKS_MKFIFO
#define ACE_LACKS_MKTEMP
#define ACE_LACKS_MKSTEMP
#if 0
#define ACE_LACKS_MMAP
#endif
#define ACE_LACKS_MPROTECT
#define ACE_LACKS_MUTEXATTR_PSHARED
#define ACE_LACKS_PIPE
#define ACE_LACKS_RLIMIT
#define ACE_LACKS_RPC_H
#define ACE_LACKS_RECVMSG
#define ACE_LACKS_RWLOCK_T
#define ACE_LACKS_SEMBUF_T
#if 0
#define ACE_LACKS_SOCKETPAIR
#endif
#define ACE_LACKS_SOME_POSIX_PROTOTYPES
#define ACE_LACKS_UNIX_DOMAIN_SOCKETS
#define ACE_LACKS_USER
#if 0
#define ACE_LACKS_STAT
#endif
#define ACE_LACKS_FILE_FCNTL
#define ACE_LACKS_FCNTL
#define ACE_LACKS_SYS_FILE_H
#if 0
#define ACE_LACKS_DIRECTORY_FUNCTIONS
#define ACE_LACKS_UNLINK
#endif
#define ACE_LACKS_UMASK
#if 0
#define ACE_LACKS_CREAT
#define ACE_LACKS_TRUNCATE
#define ACE_LACKS_DUP
#endif
#define ACE_LACKS_SEEK
#define ACE_LACKS_SHARED_MEMORY
#define ACE_LACKS_MSYNC
#if 0
#define ACE_LACKS_SIGNALS
#endif
#define ACE_LACKS_PID_STUFF
#define ACE_LACKS_NETDB_REENTRANT_FUNCTIONS
#define ACE_LACKS_IPC_H
#define ACE_LACKS_PARAM_H
#define ACE_LACKS_SYSV_MSG_H
#define ACE_LACKS_UTSNAME_T
#define ACE_LACKS_ISATTY
#define ACE_LACKS_GETOPT
#define ACE_LACKS_STRCASECMP
#define ACE_HAS_SYS_BSDTIME_H
#define ACE_LACKS_TRUNCATE
#define ACE_LACKS_PWD_FUNCTIONS
#define ACE_LACKS_UNIX_SIGNALS
#if 0
#define ACE_LACKS_ACCESS
#define ACE_LACKS_UNAME
#endif
#define ACE_HAS_THREAD_SPECIFIC_STORAGE
#define ACE_LACKS_SYSV_SHMEM
#define ACE_LACKS_PUTENV
#define ACE_HAS_4_4BSD_SENDMSG_RECVMSG
#define ACE_HAS_CLOCK_GETTIME
#define ACE_LACKS_THREAD_PROCESS_SCOPING
#define ACE_LACKS_SETSCHED
/* #define ACE_LACKS_SYS_NERR */
#if 0
#define ACE_LACKS_PTHREAD_CANCEL
#define ACE_LACKS_PTHREAD_KILL
#define ACE_LACKS_PTHREAD_THR_SIGSETMASK
#define ACE_LACKS_PTHREAD_CLEANUP
#endif
#define ACE_LACKS_STRRECVFD
#define ACE_LACKS_WRITEV
#define ACE_LACKS_READV
/* below refers to fcntl style locking */
#define ACE_LACKS_FILELOCKS

/***** Not tied to standards AFAIK ****/
#define ACE_LACKS_MADVISE /* paging optimization not needed with INTEGRITY */
#define ACE_LACKS_MALLOC_H /* netbsd's just includes stdlib.h */
#define ACE_LACKS_MEMORY_H /* netbsd's just includes string.h */

/***** STUFF INTEGRITY 4.0.8 APPEARS TO SUPPORT ****/
/* note, possibly untested with ace */
#if 0
#define ACE_LACKS_GETSERVBYNAME
#endif

/***** TAO STUFF ****/
#define TAO_USE_DOTTED_DECIMAL_ADDRESSES 1

#include <INTEGRITY.h>

#include <time.h>

typedef void (*__sighandler_t)(int);

extern "C"
{
  inline int isatty(int) { return 0; }
}

#if 0
struct stat
{
  unsigned int st_atime; // time of last access
  unsigned int st_mtime; // time of last data modification
  unsigned int st_size;           // file size, in bytes
  unsigned int st_mode;
  unsigned int st_nlink;
};

#define MAXNAMLEN 1024
#define S_IRUSR 1
#define S_IRGRP 2
#define S_IROTH 4
#define S_IWUSR 8
#define S_IWGRP 16
#define S_IWOTH 32
#endif

#ifdef ppc
#define ACE_HAS_POWERPC_TIMER
#endif

typedef union sigval sigval_t;

/* MIKEC Addtions */
#define ACE_HAS_NONCONST_SELECT_TIMEVAL
#include <sys/uio.h> // needed to define iovec
#define ACE_LACKS_READLINK
#define ACE_LACKS_GETPPID
#define NSIG (SIGRTMAX+1)
#define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 1
#define ACE_USE_RCSID 0
#if 0
typedef int mode_t;
typedef int gid_t;
typedef int uid_t;
#endif
#define ACE_LACKS_CUSERID
#if 0
#define ACE_LACKS_FSYNC
#define ACE_LACKS_CHDIR
#define ACE_LACKS_LSTAT
#endif
#define ACE_NEEDS_HUGE_THREAD_STACKSIZE 0x20000
// #include <stdio.h>
#define fileno(file)	((file)->io_channel) //Hack to get Svc_Conf_l.cpp compiled
#define ACE_DEFAULT_THREAD_PRIORITY 127
#define ACE_HAS_ATTR_SETSCHED
#if 0
#define ACE_LACKS_INHERIT_SCHED
#endif
#define PRI_FIFO_MIN 1
#define PRI_FIFO_MAX 127
#define ACE_THR_PRI_FIFO_DEF 127
#define PRI_RR_MIN 1
#define PRI_RR_MAX 127
#define ACE_THR_PRI_RR_DEF 127
#define PRI_OTHER_MIN 1
#define PRI_OTHER_MAX 127
#define ACE_THR_PRI_OTHER_DEF 127
#define ACE_PTHREAD_RETURN_ON_EXIT
#undef ACE_LACKS_UNLINK

#define ACE_HAS_TIMED_MESSAGE_BLOCKS

extern "C" {
int unlink(const char *);
}

#define ACE_LACKS_SETSID
#define ACE_HAS_TIMEZONE_GETTIMEOFDAY /* TZ is ignored */
#define ACE_LACKS_UNIX_SYSLOG
#define ACE_LACKS_TELLDIR
#define ACE_LACKS_SEEKDIR
#if 0
#define ACE_LACKS_REWINDDIR
#endif
#define ACE_LACKS_GETHOSTENT


/* end MIKEC Addtions */

#endif /* ACE_CONFIG_H */
