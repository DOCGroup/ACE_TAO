/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    constants.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_CONFIG_CONSTANTS_H
#define PACE_CONFIG_CONSTANTS_H

#define PACE_ONE_SECOND_IN_MSECS 1000L
#define PACE_ONE_SECOND_IN_USECS 1000000L
#define PACE_ONE_SECOND_IN_NSECS 1000000000L

/* ============================================================================
 * Posix minimum values
 * ============================================================================ */

/* The number of I/O operations that can be specified in a list I/O call. */
# if defined (_POSIX_AIO_LISTIO_MAX)
# define PACE_POSIX_AIO_LISTIO_MAX _POSIX_AIO_LISTIO_MAX
# endif

/* The number of outstanding asynchronous I/O operations. */
# if defined (_POSIX_AIO_MAX)
# define PACE_POSIX_AIO_MAX _POSIX_AIO_MAX
# endif

/* The length of the arguments for one of the exec functions, in bytes,
 * excluding environment data. */
# if defined (_POSIX_ARG_MAX)
# define PACE_POSIX_ARG_MAX _POSIX_ARG_MAX
# endif

/* The number of simultaneous processes per real user ID. */
# if defined (_POSIX_CHILD_MAX)
# define PACE_POSIX_CHILD_MAX _POSIX_CHILD_MAX
# endif

/* The number of timer expiration overruns. */
# if defined (_POSIX_DELAYTIMER_MAX)
# define PACE_POSIX_DELAYTIMER_MAX _POSIX_DELAYTIMER_MAX
# endif

/* The value of a file's link count. */
# if defined (_POSIX_LINK_MAX)
# define PACE_POSIX_LINK_MAX _POSIX_LINK_MAX
# endif

/* The size of the storage required for a login name, in bytes,
 * including the terminating NULL. */
# if defined (_POSIX_LOGIN_NAME_MAX)
# define PACE_POSIX_LOGIN_NAME_MAX _POSIX_LOGIN_NAME_MAX
# endif

/* The number of bytes in a terminal canonical input queue. */
# if defined (_POSIX_MAX_CANON)
# define PACE_POSIX_MAX_CANON _POSIX_MAX_CANON
# endif

/* The number of bytes for which space will be available in a
 * terminal input queue. */
# if defined (_POSIX_MAX_INPUT)
# define PACE_POSIX_MAX_INPUT _POSIX_MAX_INPUT
# endif

/* The number of message queues that can be open for a single
 * process. */
# if defined (_POSIX_MQ_OPEN_MAX)
# define PACE_POSIX_MQ_OPEN_MAX _POSIX_MQ_OPEN_MAX
# endif

/* The maximum number of message priorities supported by the
 * implementation. */
# if defined (_POSIX_MQ_PRIO_MAX)
# define PACE_POSIX_MQ_PRIO_MAX _POSIX_MQ_PRIO_MAX
# endif

/* The number of bytes in a filename. */
# if defined (_POSIX_NAME_MAX)
# define PACE_POSIX_NAME_MAX _POSIX_NAME_MAX
# endif

/* The number of simultaneous supplementary group IDs per process. */
# if defined (_POSIX_NGROUPS_MAX)
# define PACE_POSIX_NGROUPS_MAX _POSIX_NGROUPS_MAX
# endif

/* The number of files that one process can have open at one time. */
# if defined (_POSIX_OPEN_MAX)
# define PACE_POSIX_OPEN_MAX _POSIX_OPEN_MAX
# endif

/* The number of bytes in a pathname. */
# if defined (_POSIX_PATH_MAX)
# define PACE_POSIX_PATH_MAX _POSIX_PATH_MAX
# endif

/* The number of bytes that can be written atomically when writing
 * to a pipe. */
# if defined (_POSIX_PIPE_BUF)
# define PACE_POSIX_PIPE_BUF _POSIX_PIPE_BUF
# endif

/* The number of realtime signal numbers reserved for application use. */
# if defined (_POSIX_RTSIG_MAX)
# define PACE_POSIX_RTSIG_MAX _POSIX_RTSIG_MAX
# endif

/* The number of semaphores that a process may have. */
# if defined (_POSIX_SEM_NSEMS_MAX)
# define PACE_POSIX_SEM_NSEMS_MAX _POSIX_SEM_NSEMS_MAX
# endif

/* The maximum value a semaphore may have. */
# if defined (_POSIX_SEM_VALUE_MAX)
# define PACE_POSIX_SEM_VALUE_MAX _POSIX_SEM_VALUE_MAX
# endif

/* The number of queued signals that a process may send and have pending
 * at the receiver(s) at any time. */
# if defined (_POSIX_SIGQUEUE_MAX)
# define PACE_POSIX_SIGQUEUE_MAX _POSIX_SIGQUEUE_MAX
# endif

/* The value that can be stored in an object of type ssize_t. */
# if defined (_POSIX_SSIZE_MAX)
# define PACE_POSIX_SSIZE_MAX _POSIX_SSIZE_MAX
# endif

/* The number of streams that one process can have open at one time. */
# if defined (_POSIX_STREAM_MAX)
# define PACE_POSIX_STREAM_MAX _POSIX_STREAM_MAX
# endif

/* The number of attempts made to destroy the thread-specific data
 * values of a thread on thread exit. */
# if defined (_POSIX_THREAD_DESTRUCTOR_ITERATIONS)
# define PACE_POSIX_THREAD_DESTRUCTOR_ITERATIONS _POSIX_THREAD_DESTRUCTORITERATIONS
# endif

/* The number of data keys per process. */
# if defined (_POSIX_THREAD_KEYS_MAX)
# define PACE_POSIX_THREAD_KEYS_MAX _POSIX_THREAD_KEYS_MAX
# endif

/* The number of threads per process. */
# if defined (_POSIX_THREAD_THREADS_MAX)
# define PACE_POSIX_THREAD_THREADS_MAX _POSIX_THREAD_THREADS_MAX
# endif

/* The size of the storage required for a terminal device name, in bytes,
 * including the terminating NULL. */
# if defined (_POSIX_TTY_NAME_MAX)
# define PACE_POSIX_TTY_NAME_MAX _POSIX_TTY_NAME_MAX
# endif

/* The per-process number of times. */
# if defined (_POSIX_TIMER_MAX)
# define PACE_POSIX_TIMER_MAX _POSIX_TIMER_MAX
# endif

/* The maximum number of bytes supported for the name of a time zone
 * (not of the TZ variable). */
# if defined (_POSIX_TZNAME_MAX)
# define PACE_POSIX_TZNAME_MAX _POSIX_TZNAME_MAX
# endif


/* ============================================================================
 * Temporary MACROS. Correct them later.
 * ============================================================================ */

#include "pace/sys/types.h"

/* Tracing */
# define PACE_DEBUG(X)
# define PACE_LM_DEBUG PACE_stderr
# define PACE_BEGIN_DUMP
# define PACE_END_DUMP

/* Identification for PACE.h */
# define PACE_RCSID(X,Y,Z)

/* Hooks */
/* Dummy values */
# define PACE_ALLOC_HOOK_DECLARE int declare_hook
# define PACE_ALLOC_HOOK_DEFINE(X)

/* Signals / ioctl / fcntl - dummy values */
# define PACE_SIGURG 1
# define PACE_NONBLOCK 2
# define PACE_CLOEXEC 3

/* Flags */
/* Will not be a macro, dummy values */
# define PACE_set_flags(X,Y) 1
# define PACE_clr_flags(X,Y) 1

/* Log Error */
# define PACE_ERROR(X)

/* Host Entries */
# if defined (PACE_HAS_STRUCT_NETDB_DATA)
typedef char PACE_HOSTENT_DATA[sizeof(struct hostent_data)];
typedef char PACE_SERVENT_DATA[sizeof(struct servent_data)];
typedef char PACE_PROTOENT_DATA[sizeof(struct protoent_data)];
# else
# if !defined PACE_HOSTENT_DATA_SIZE
# define PACE_HOSTENT_DATA_SIZE (4*1024)
# endif /* PACE_HOSTENT_DATA_SIZE */
# if !defined PACE_SERVENT_DATA_SIZE
# define PACE_SERVENT_DATA_SIZE (4*1024)
# endif /*PACE_SERVENT_DATA_SIZE */
# if !defined PACE_PROTOENT_DATA_SIZE
# define PACE_PROTOENT_DATA_SIZE (2*1024)
# endif /*PACE_PROTOENT_DATA_SIZE */
typedef char PACE_HOSTENT_DATA[PACE_HOSTENT_DATA_SIZE];
typedef char PACE_SERVENT_DATA[PACE_SERVENT_DATA_SIZE];
typedef char PACE_PROTOENT_DATA[PACE_PROTOENT_DATA_SIZE];
# endif /* PACE_HAS_STRUCT_NETDB_DATA */

/* PACE_HAS_SIN_LEN */

/* PACE_FREE */
/* adjust later to use macros to decide how to free */
# define PACE_FREE(X,Y) \
X Y

# define AF_ANY -1

/* PACE CATCHALL */
/* FIONREAD is used in PACE_SOCK_IO::recvv */
/* PACE_HAS_ALLOCA is used in PACE_SOCK_IO::recvv */

#endif /* PACE_CONFIG_CONSTANTS_H */
