/* -*- C++ -*- */
// $Id$

// ***** This configuration file is still under testing. *****

// The following configuration file is designed to work for NetBSD
// platforms using GNU g++.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H

#if ! defined (__ACE_INLINE__)
#define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

// ********************************************************
// uncomment next line if you are using FreeBSD 2.1.x[R]
// #define FreeBSD_2_1
// ********************************************************
#define ACE_NETBSD

#define ACE_SIZEOF_LONG_DOUBLE 12

#if defined (__GNUG__)
# include "ace/config-g++-common.h"
#endif /* __GNUG__ */

// #define ACE_HAS_BROKEN_NETBSD_MSYNC

// Platform specific directives
#define ACE_LACKS_GETPGID
#define ACE_LACKS_RWLOCK_T
#define ACE_HAS_SIG_MACROS
#define ACE_HAS_CHARPTR_DL
#define ACE_USES_ASM_SYMBOL_IN_DLSYM
#define ACE_LACKS_SIGSET

// This is for 2.1.x only.  By default, gcc defines __FreeBSD__ automatically
#if defined(FreeBSD_2_1)

#define ACE_HAS_CPLUSPLUS_HEADERS

// This is to fix the nested struct if_data definition on FreeBSD 2.1.x
#include <sys/types.h>
#include <sys/time.h>
struct  if_data {
/* generic interface information */
  u_char  ifi_type;       /* ethernet, tokenring, etc */
  u_char  ifi_physical;   /* e.g., AUI, Thinnet, 10base-T, etc */
  u_char  ifi_addrlen;    /* media address length */
  u_char  ifi_hdrlen;     /* media header length */
  u_long  ifi_mtu;        /* maximum transmission unit */
  u_long  ifi_metric;     /* routing metric (external only) */
  u_long  ifi_baudrate;   /* linespeed */
/* volatile statistics */
  u_long  ifi_ipackets;   /* packets received on interface */
  u_long  ifi_ierrors;    /* input errors on interface */
  u_long  ifi_opackets;   /* packets sent on interface */
  u_long  ifi_oerrors;    /* output errors on interface */
  u_long  ifi_collisions; /* collisions on csma interfaces */
  u_long  ifi_ibytes;     /* total number of octets received */
  u_long  ifi_obytes;     /* total number of octets sent */
  u_long  ifi_imcasts;    /* packets received via multicast */
  u_long  ifi_omcasts;    /* packets sent via multicast */
  u_long  ifi_iqdrops;    /* dropped on input, this interface */
  u_long  ifi_noproto;    /* destined for unsupported protocol */
  struct  timeval ifi_lastchange;/* time of last administrative ch
ange */
} ;

// this is a hack, but since this only occured in FreeBSD 2.1.x,
// I guess it is ok.
#define ACE_HAS_BROKEN_TIMESPEC_MEMBERS

#endif /* defined FreeBSD_2_1 */

// Platform supports POSIX timers via struct timespec.
#define ACE_HAS_POSIX_TIME
#define ACE_HAS_UALARM

// Platform defines struct timespec but not timespec_t
#define ACE_LACKS_TIMESPEC_T

#define ACE_LACKS_SYSTIME_H

#define ACE_LACKS_STRRECVFD

#define ACE_HAS_SIN_LEN

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC

// Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYSCALL_H

#if !defined(FreeBSD_2_1)
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES
#endif /* defined FreeBSD_2_1 */

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

// Optimize ACE_Handle_Set for select().
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

#define ACE_LACKS_GETHOSTENT

#endif /* ACE_CONFIG_H */
