/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    temp.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_CONFIG_TEMP_H
#define PACE_CONFIG_TEMP_H


/* Temporary MACROS. Correct them later. Will probably be in OS.h or ACE.h. */


#include "pace/sys/types.h"
/*#include "pace/stdio.h"*/

/* Need for NT */
# define PACE_Export
# define PACE_BUILD_DLL

/* Tracing */
# define PACE_TRACE(X) fprintf (stderr, X)
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

/* Define some helpful constants. */
/* Not type-safe, and signed.  For backward compatibility. */
#define PACE_ONE_SECOND_IN_MSECS 1000L
#define PACE_ONE_SECOND_IN_USECS 1000000L
#define PACE_ONE_SECOND_IN_NSECS 1000000000L

#endif /* PACE_CONFIG_TEMP_H */
