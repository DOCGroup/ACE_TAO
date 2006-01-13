/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for pSOSim on SunOS5
// using the GNU/Cygnus g++ 2.7.2 compiler, without repo patch.

///////////////////////////////////////////////////////////////////////////////
// * IMPORTANT * IMPORTANT * IMPORTANT * IMPORTANT * IMPORTANT * IMPORTANT * //
//                                                                           //
//   Because pSOSim includes UNIX system header files in order to do         //
//   its emulation of pSOSystSem on Solaris, there are a number of           //
//   things that are "available" to ACE on pSOSim that are *not*             //
//   really available on pSOSystem.  Every effort has been made to           //
//   avoid dependencies on these "features" in the ACE pSOSim port,          //
//   which has in turn necessarily required pSOSim specific definitions.     //
//                                                                           //
//   To ease portability between pSOSim and pSOSystem, the definitions       //
//   in this file have been separated into three groups: those that          //
//   are known to be appropriate for both pSOSim and  pSOSystem, those       //
//   known to be appropriate for pSOSim but (probably) not for pSOSystem,    //
//   and those that are (probably) appropriate for pSOSystem, but are        //
//   not appropriate for pSOSim.                                             //
//                                                                           //
//   When porting from pSOSim to pSOSystem, it is (probably) a good          //
//   idea to leave the definitions in the first category alone,              //
//   comment out the definitions in the second category, and uncomment       //
//   the definitions in the third category.  Additional definitions          //
//   may need to be added to the third category, as only those that          //
//   were encountered during the pSOSim port were added (that is, one        //
//   of the system files included by pSOSim could be compensating for        //
//   a definition pSOSystem really needs.                                    //
//                                                                           //
// * IMPORTANT * IMPORTANT * IMPORTANT * IMPORTANT * IMPORTANT * IMPORTANT * //
///////////////////////////////////////////////////////////////////////////////


#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H
#include /**/ "ace/pre.h"

////////////////////////////////////////////////////////////////
//                                                            //
//   Definitions appropriate for both pSOSim and pSOSystem    //
//                                                            //
////////////////////////////////////////////////////////////////

#if ! defined (__ACE_INLINE__)
# define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

#if defined (__GNUG__)
# include "ace/config-g++-common.h"
#endif /* __GNUG__ */

#define ACE_HAS_IP_MULTICAST
#define ACE_LACKS_WAIT
#define ACE_LACKS_WAITPID
#define ACE_HAS_CPLUSPLUS_HEADERS
#define ACE_LACKS_SYSTEM

// #define ACE_LACKS_U_LONG_LONG

#define ACE_LACKS_HRTIME_T

// #define ACE_LACKS_EVENT_T

#define ACE_HAS_VERBOSE_NOTSUP

#define ACE_LACKS_MEMORY_H

#define ACE_LACKS_MALLOC_H

#define ACE_LACKS_MMAP
#define ACE_LACKS_UMASK

#define ACE_LACKS_UNIX_DOMAIN_SOCKETS

#define ACE_HAS_NONSTATIC_OBJECT_MANAGER

#define ACE_LACKS_SEMBUF_T

#define ACE_LACKS_EXEC

#define ACE_LACKS_FORK

#define ACE_LACKS_SETEGID
#define ACE_LACKS_SETPGID
#define ACE_LACKS_SETREGID
#define ACE_LACKS_SETREUID
#define ACE_LACKS_SETSID
#define ACE_LACKS_SETUID
#define ACE_LACKS_SETEUID
#define ACE_LACKS_SETGID
#define ACE_LACKS_PIPE
#define ACE_LACKS_GETEUID
#define ACE_LACKS_GETUID
#define ACE_LACKS_GETPGID
#define ACE_LACKS_GETEGID
#define ACE_LACKS_GETGID

// rename the main entry point
#define ACE_MAIN extern "C" void root

// All this is commented out for the single threaded port
/*

#define ACE_HAS_THREADS

#if !defined (ACE_MT_SAFE)
# define ACE_MT_SAFE 1
#endif

#define ACE_DEFAULT_THREAD_KEYS 256

#define ACE_LACKS_COND_T


*/

#define ACE_HAS_TSS_EMULATION


////////////////////////////////////////////////////////////////
//                                                            //
//   Definitions appropriate for pSOSim but not pSOSystem     //
//                                                            //
////////////////////////////////////////////////////////////////

#define ACE_HAS_POSIX_TIME

////////////////////////////////////////////////////////////////
//                                                            //
//   Definitions appropriate for pSOSystem but not pSOSim     //
//                                                            //
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
//                                                            //
//   Definitions that have not been categorized               //
//                                                            //
////////////////////////////////////////////////////////////////

/* In the *simulator* environment, use unsigned int for size_t */
#define size_t  unsigned int

#define TCP_

#define ACE_LACKS_PTHREAD_THR_SIGSETMASK

/* #define ACE_HAS_NONCONST_SENDMSG */

/* #define ACE_HAS_NONCONST_WRITEV  */

#define ACE_HAS_CHARPTR_SOCKOPT

#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

#define ACE_HAS_MSG

#define ACE_HAS_POSIX_NONBLOCK

#define ACE_HAS_SIGINFO_T

#define ACE_HAS_SIGWAIT

#define ACE_HAS_SIG_ATOMIC_T

#define ACE_HAS_STRERROR

#define ACE_LACKS_ACCESS

#define ACE_LACKS_GETHOSTENT

#define ACE_LACKS_GETSERVBYNAME

#define ACE_LACKS_IOSTREAM_FX

#define ACE_LACKS_KEY_T

#define ACE_LACKS_LINEBUFFERED_STREAMBUF

#define ACE_LACKS_LONGLONG_T

#define ACE_LACKS_LSTAT

#define ACE_LACKS_MADVISE

#define ACE_LACKS_MKTEMP

#define ACE_LACKS_MPROTECT

#define ACE_LACKS_MSYNC

#define ACE_LACKS_SYS_PARAM_H

#define ACE_LACKS_PWD_FUNCTIONS

#define ACE_LACKS_READLINK

#define ACE_LACKS_RLIMIT

#define ACE_LACKS_RWLOCK_T

#define ACE_LACKS_SBRK

#define ACE_LACKS_SIGINFO_H

#define ACE_LACKS_SIGNED_CHAR

#define ACE_LACKS_SI_ADDR

#define ACE_LACKS_SOCKETPAIR

#define ACE_LACKS_STRCASECMP

#define ACE_LACKS_STRRECVFD

#define ACE_LACKS_SYSCALL

#define ACE_LACKS_SYS_MSG_H

#define ACE_LACKS_SYSV_SHMEM
#define ACE_LACKS_DUP2

#define ACE_LACKS_TIMESPEC_T

#define ACE_LACKS_UCONTEXT_H

#define ACE_LACKS_UNIX_SIGNALS

#define ACE_LACKS_UTSNAME_T
#define ACE_LACKS_UNAME

#define ACE_PAGE_SIZE 4096

#if !defined (ACE_PSOS)
#define ACE_PSOS
#endif /* ACE_PSOS */

#if !defined (ACE_PSOSIM)
#define ACE_PSOSIM
#endif /* ACE_PSOSIM */

#if !defined (ACE_PSOS_TBD)
#define ACE_PSOS_TBD
#endif /* ACE_PSOS_TBD */

// By default, don't include RCS Id strings in object code.
#if !defined (ACE_USE_RCSID)
#define ACE_USE_RCSID 0
#endif /* #if !defined (ACE_USE_RCSID) */
#define ACE_LACKS_MKFIFO

#define ACE_MALLOC_ALIGN 8
#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_H */
