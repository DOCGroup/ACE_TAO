/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for AIX 3.2.5
// with xlC.  Thanks to Bob Olson <olson@mcs.anl.gov> for this.

/*#define ACE_HAS_PTHREADS*/

#define MAXNAMELEN 1024

#define ACE_HAS_ONLY_TWO_PARAMS_FOR_ASCTIME_R_AND_CTIME_R
#define ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

#define ACE_HAS_TEMPLATE_TYPEDEFS

#define ACE_HAS_STRERROR

#define ACE_HAS_SIG_ATOMIC_T

#define ACE_HAS_SSIZE_T

#define ACE_HAS_CPLUSPLUS_HEADERS

#define ACE_HAS_POLL

#define ACE_HAS_POSIX_NONBLOCK

#define ACE_HAS_AIX_GETTIMEOFDAY

#define ACE_HAS_NO_SYSCALL_H

// Compiler/platform has the getrusage() system call.
#define ACE_HAS_GETRUSAGE

// Sockets may be called in multi-threaded programs.
#define ACE_HAS_MT_SAFE_SOCKETS

// Compiler/platform uses old malloc()/free() prototypes (ugh).
#define ACE_HAS_OLD_MALLOC

// Defines the page size of the system.
#define ACE_PAGE_SIZE 4096

// Compiler/platform supports struct strbuf
#define ACE_HAS_STRBUF_T

// Platform supports STREAMS.
/*#define ACE_HAS_STREAMS*/
