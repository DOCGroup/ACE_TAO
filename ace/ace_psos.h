/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_psos.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_PSOS_H
# define ACE_ACE_PSOS_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */


# if defined (ACE_PSOS_SNARFS_HEADER_INFO)
  // Header information snarfed from compiler provided header files
  // that are not included because there is already an identically
  // named file provided with pSOS, which does not have this info
  // from compiler supplied stdio.h
  extern FILE *fdopen(int, const char *);
  extern int getopt(int, char *const *, const char *);
  extern char *tempnam(const char *, const char *);
  extern "C" int fileno(FILE *);

//  #define fileno(stream) ((stream)->_file)

  // from compiler supplied string.h
  extern char *strdup (const char *);

  // from compiler supplied stat.h
  extern mode_t umask (mode_t);
  extern int mkfifo (const char *, mode_t);
  extern int mkdir (const char *, mode_t);

  // from compiler supplied stdlib.h
  extern int putenv (char *);

  int isatty (int h);

# endif /* ACE_PSOS_SNARFS_HEADER_INFO */


# if defined (ACE_PSOS)
// Some versions of pSOS do not define error numbers, but newer
// versions do. So, include errno.h and then see which ones are not
// yet defined.
#   if !defined (EPERM)
#     define EPERM        1        /* Not super-user                        */
#   endif /* EPERM */
#   if !defined (ENOENT)
#     define ENOENT       2        /* No such file or directory             */
#   endif /* ENOENT */
#   if !defined (ESRCH)
#     define ESRCH        3        /* No such process                       */
#   endif /* ESRCH */
#   if ! defined (EINTR)
#     define EINTR        4        /* interrupted system call               */
#   endif /* EINTR */
#   if !defined (EBADF)
#     define EBADF        9        /* Bad file number                       */
#   endif /* EBADF */
#   if !defined (EAGAIN)
#     define EAGAIN       11       /* Resource temporarily unavailable      */
#   endif /* EAGAIN */
#   if !defined (EWOULDBLOCK)
#     define EWOULDBLOCK  EAGAIN   /* Blocking resource request would block */
#   endif /* EWOULDBLOCK */
#   if !defined (ENOMEM)
#     define ENOMEM       12       /* Not enough core                       */
#   endif /* ENOMEM */
#   if !defined (EACCESS)
#     define EACCESS      13       /* Permission denied                     */
#   endif /* EACCESS */
#   if !defined (EFAULT)
#     define EFAULT       14       /* Bad access                            */
#   endif /* EFAULT */
#   if !defined (EEXIST)
#     define EEXIST       17       /* File exists                           */
#   endif /* EEXIST */
#   if !defined (ENOSPC)
#     define ENOSPC       28       /* No space left on device               */
#   endif /* ENOSPC */
#   if !defined (EPIPE)
#     define EPIPE        32       /* Broken pipe                           */
#   endif /* EPIPE */
#   if !defined (ETIME)
#     define ETIME        62       /* timer expired                         */
#   endif /* ETIME */
#   if !defined (ENAMETOOLONG)
#     define ENAMETOOLONG 78       /* path name is too long                 */
#   endif /* ENAMETOOLONG */
#   if !defined (ENOSYS)
#     define ENOSYS       89       /* Unsupported file system operation     */
#   endif /* ENOSYS */
#   if !defined (EADDRINUSE)
#     define EADDRINUSE   125      /* Address already in use                */
#   endif /* EADDRINUSE */
#   if !defined (ENETUNREACH)
#     define ENETUNREACH  128      /* Network is unreachable                */
#   endif /* ENETUNREACH */
#   if !defined (EISCONN)
#     define EISCONN      133      /* Socket is already connected           */
#   endif /* EISCONN */
#   if !defined (ESHUTDOWN)
#     define ESHUTDOWN    143      /* Can't send after socket shutdown      */
#   endif /* ESHUTDOWN */
#   if !defined (ECONNREFUSED)
#     define ECONNREFUSED 146      /* Connection refused                    */
#   endif /* ECONNREFUSED */
#   if !defined (EINPROGRESS)
#     define EINPROGRESS  150      /* operation now in progress             */
#   endif /* EINPROGRESS */
#   if !defined (ERRMAX)
#     define ERRMAX       151      /* Last error number                     */
#   endif /* ERRMAX */
# endif /* ACE_PSOS */



# if defined (ACE_PSOS)

// Wrapper for NT events on pSOS.
class ACE_OS_Export ACE_event_t
{
  friend class ACE_OS;

protected:

  /// Protect critical section.
  ACE_mutex_t lock_;

  /// Keeps track of waiters.
  ACE_cond_t condition_;

  /// Specifies if this is an auto- or manual-reset event.
  int manual_reset_;

  /// "True" if signaled.
  int is_signaled_;

  /// Number of waiting threads.
  u_long waiting_threads_;
};

# endif /* ACE_PSOS */



# if defined (ACE_PSOS)

#   if defined (ACE_PSOSIM)

#     include /**/ "ace/sys_conf.h" /* system configuration file */
#     include /**/ <psos.h>         /* pSOS+ system calls                */
#     include /**/ <pna.h>          /* pNA+ TCP/IP Network Manager calls */

    /* In the *simulator* environment, use unsigned int for size_t */
#     define size_t  unsigned int


    /*   include <rpc.h>       pRPC+ Remote Procedure Call Library calls   */
    /*                         are not supported by pSOSim                 */
    /*                                                                     */
    /*   include <phile.h>     pHILE+ file system calls are not supported  */
    /*                         by pSOSim *so*, for the time being, we make */
    /*                         use of UNIX file system headers and then    */
    /*                         when we have time, we wrap UNIX file system */
    /*                         calls w/ pHILE+ wrappers, and modify ACE to */
    /*                         use the wrappers under pSOSim               */

    /* put includes for necessary UNIX file system calls here */
#     include /**/ <sys/stat.h>
#     include /**/ <sys/ioctl.h>
#     include /**/ <sys/sockio.h>
#     include /**/ <netinet/tcp.h>

#     define TCP_
#     if ! defined (BUFSIZ)
#       define BUFSIZ 1024
#     endif  /* ! defined (BUFSIZ) */


#   else

#     if defined (ACE_PSOS_CANT_USE_SYS_TYPES)
      // these are missing from the pSOS types.h file, and the compiler
      // supplied types.h file collides with the pSOS version.
#       if !defined (ACE_SHOULD_NOT_DEFINE_SYS_TYPES)
      typedef unsigned char     u_char;
      typedef unsigned short    u_short;
#       endif /* ACE_SHOULD_NOT_DEFINE_SYS_TYPES */
      typedef unsigned int      u_int;
#       if !defined (ACE_SHOULD_NOT_DEFINE_SYS_TYPES)
      typedef unsigned long     u_long;
#       endif /* ACE_SHOULD_NOT_DEFINE_SYS_TYPES */

      // These are defined in types.h included by (among others) pna.h
#       if 0
      typedef unsigned char     uchar_t;
      typedef unsigned short    ushort_t;
      typedef unsigned int      uint_t;
      typedef unsigned long     ulong_t;
#       endif /* 0 */
      typedef char *            caddr_t;

#       if defined (ACE_PSOS_DIAB_PPC)
      // pid_t is defined in sys/types.h
#         if 0
      typedef unsigned long pid_t;
#         endif /* 0 */
#         define ACE_INVALID_PID ((pid_t) ~0)
#       else /* !defined (ACE_PSOS_DIAB_PPC) */
      typedef long pid_t;
#         define ACE_INVALID_PID ((pid_t) -1)
#       endif /* defined (ACE_PSOS_DIAB_PPC) */

//      typedef unsigned char wchar_t;
#     endif /* ACE_PSOS_CANT_USE_SYS_TYPES */

#     include /**/ "ace/sys_conf.h" /* system configuration file */
#     include /**/ <configs.h>   /* includes all pSOS headers */
//    #include /**/ <psos.h>    /* pSOS system calls */
#     include /**/ <pna.h>      /* pNA+ TCP/IP Network Manager calls */
#     include /**/ <phile.h>     /* pHILE+ file system calls */
//    #include /**/ <prepccfg.h>     /* pREPC+ file system calls */
#     if defined (ACE_PSOS_DIAB_MIPS)
#       if defined (ACE_PSOS_USES_DIAB_SYS_CALLS)
#         include /**/ <unistd.h>    /* Diab Data supplied file system calls */
#       else
#         include /**/ <prepc.h>
#       endif /* ACE_PSOS_USES_DIAB_SYS_CALLS */
#       include /**/ <sys/wait.h>    /* Diab Data supplied header file */
#     endif /* ACE_PSOS_DIAB_MIPS */

// This collides with phile.h
//    #include /**/ <sys/stat.h>    /* Diab Data supplied header file */

// Some versions have missing preprocessor definitions
#     if !defined (AF_UNIX)
#       define AF_UNIX 0x1
#     endif /* AF_UNIX */
#     define PF_UNIX AF_UNIX
#     define PF_INET AF_INET
#     if !defined (AF_MAX)
#       define AF_MAX AF_INET
#     endif /* AF_MAX */
#     if !defined (IFF_LOOPBACK)
#       define IFF_LOOPBACK IFF_EXTLOOPBACK
#     endif /* IFF_LOOPBACK */

  typedef long fd_mask;
#     define IPPORT_RESERVED       1024
#     define IPPORT_USERRESERVED   5000

  extern "C"
  {
    typedef void (* ACE_SignalHandler) (void);
    typedef void (* ACE_SignalHandlerV) (void);
  }

#     if !defined(SIG_DFL)
#       define SIG_DFL (ACE_SignalHandler) 0
#     endif  /* philabs */

#   endif /* defined (ACE_PSOSIM) */


#   if ! defined (NSIG)
#     define NSIG 32
#   endif /* NSIG */

#   if ! defined (TCP_NODELAY)
#     define TCP_NODELAY  1
#   endif /* TCP_NODELAY */

// For general purpose portability

#   define ACE_BITS_PER_ULONG (8 * sizeof (u_long))

typedef u_long ACE_idtype_t;
typedef u_long ACE_id_t;
#   define ACE_SELF (0)
typedef u_long ACE_pri_t;

// pHILE+ calls the DIR struct XDIR instead
#   if !defined (ACE_PSOS_DIAB_PPC)
typedef XDIR ACE_DIR;
#   endif /* !defined (ACE_PSOS_DIAB_PPC) */

// Use pSOS semaphores, wrapped . . .
typedef struct
{
  u_long sema_;
  // Semaphore handle.  This is allocated by pSOS.

  char name_[4];
  // Name of the semaphore: really a 32 bit number to pSOS
} ACE_sema_t;

// Used for dynamic linking.
#   if !defined (ACE_DEFAULT_SVC_CONF)
#     define ACE_DEFAULT_SVC_CONF "./svc.conf"
#   endif /* ACE_DEFAULT_SVC_CONF */

#   if !defined (ACE_DEFAULT_SEM_KEY)
#     define ACE_DEFAULT_SEM_KEY 1234
#   endif /* ACE_DEFAULT_SEM_KEY */

#   define ACE_STDIN 0
#   define ACE_STDOUT 1
#   define ACE_STDERR 2

#   define ACE_DIRECTORY_SEPARATOR_STR_A "/"
#   define ACE_DIRECTORY_SEPARATOR_CHAR_A '/'
#   define ACE_PLATFORM_A "pSOS"
#   define ACE_PLATFORM_EXE_SUFFIX_A ""

#   define ACE_DLL_SUFFIX ACE_LIB_TEXT (".so")
#   define ACE_DLL_PREFIX ACE_LIB_TEXT ("lib")
#   define ACE_LD_SEARCH_PATH ACE_LIB_TEXT ("LD_LIBRARY_PATH")
#   define ACE_LD_SEARCH_PATH_SEPARATOR_STR ACE_LIB_TEXT (":")
#   define ACE_LOGGER_KEY ACE_LIB_TEXT ("/tmp/server_daemon")

#   define ACE_MAX_DEFAULT_PORT 65535

#   if ! defined(MAXPATHLEN)
#     define MAXPATHLEN  1024
#   endif /* MAXPATHLEN */

#   if ! defined(MAXNAMLEN)
#     define MAXNAMLEN   255
#   endif /* MAXNAMLEN */

#   if defined (ACE_LACKS_MMAP)
#     define PROT_READ 0
#     define PROT_WRITE 0
#     define PROT_EXEC 0
#     define PROT_NONE 0
#     define PROT_RDWR 0
#     define MAP_PRIVATE 0
#     define MAP_SHARED 0
#     define MAP_FIXED 0
#   endif /* ACE_LACKS_MMAP */


typedef int ACE_exitcode;

typedef ACE_HANDLE ACE_SHLIB_HANDLE;
#   define ACE_SHLIB_INVALID_HANDLE ACE_INVALID_HANDLE
#   define ACE_DEFAULT_SHLIB_MODE 0

#   define ACE_INVALID_SEM_KEY -1

struct  hostent {
  char    *h_name;        /* official name of host */
  char    **h_aliases;    /* alias list */
  int     h_addrtype;     /* host address type */
  int     h_length;       /* address length */
  char    **h_addr_list;  /* (first, only) address from name server */
#   define h_addr h_addr_list[0]   /* the first address */
};

struct  servent {
  char     *s_name;    /* official service name */
  char    **s_aliases; /* alias list */
  int       s_port;    /* port # */
  char     *s_proto;   /* protocol to use */
};

#   define ACE_SEH_TRY if (1)
#   define ACE_SEH_EXCEPT(X) while (0)
#   define ACE_SEH_FINALLY if (1)

#   if !defined (LPSECURITY_ATTRIBUTES)
#     define LPSECURITY_ATTRIBUTES int
#   endif /* !defined LPSECURITY_ATTRIBUTES */
#   if !defined (GENERIC_READ)
#     define GENERIC_READ 0
#   endif /* !defined GENERIC_READ */
#   if !defined (FILE_SHARE_READ)
#     define FILE_SHARE_READ 0
#   endif /* !defined FILE_SHARE_READ */
#   if !defined (OPEN_EXISTING)
#     define OPEN_EXISTING 0
#   endif /* !defined OPEN_EXISTING */
#   if !defined (FILE_ATTRIBUTE_NORMAL)
#     define FILE_ATTRIBUTE_NORMAL 0
#   endif /* !defined FILE_ATTRIBUTE_NORMAL */
#   if !defined (MAXIMUM_WAIT_OBJECTS)
#     define MAXIMUM_WAIT_OBJECTS 0
#   endif /* !defined MAXIMUM_WAIT_OBJECTS */
#   if !defined (FILE_FLAG_OVERLAPPED)
#     define FILE_FLAG_OVERLAPPED 0
#   endif /* !defined FILE_FLAG_OVERLAPPED */
#   if !defined (FILE_FLAG_SEQUENTIAL_SCAN)
#     define FILE_FLAG_SEQUENTIAL_SCAN 0
#   endif /* !defined FILE_FLAG_SEQUENTIAL_SCAN */

struct ACE_OVERLAPPED
{
  u_long Internal;
  u_long InternalHigh;
  u_long Offset;
  u_long OffsetHigh;
  ACE_HANDLE hEvent;
};

#   if !defined (USER_INCLUDE_SYS_TIME_TM)
#     if defined (ACE_PSOS_DIAB_PPC)
typedef struct timespec timespec_t;
#     else /* ! defined (ACE_PSOS_DIAB_PPC) */
typedef struct timespec
{
  time_t tv_sec; // Seconds
  long tv_nsec; // Nanoseconds
} timespec_t;
#     endif /* defined (ACE_PSOS_DIAB_PPC) */
#   endif /*  !defined (USER_INCLUDE_SYS_TIME_TM) */

#   if defined (ACE_PSOS_HAS_TIME)

// Use pSOS time, wrapped . . .
class ACE_OS_Export ACE_PSOS_Time_t
{
public:
    /// default ctor: date, time, and ticks all zeroed.
  ACE_PSOS_Time_t (void);

    /// ctor from a timespec_t
  ACE_PSOS_Time_t (const timespec_t& t);

    /// type cast operator (to a timespec_t)
  operator timespec_t ();

    /// static member function to get current system time
  static u_long get_system_time (ACE_PSOS_Time_t& t);

    /// static member function to set current system time
  static u_long set_system_time (const ACE_PSOS_Time_t& t);

#     if defined (ACE_PSOSIM)
    /// static member function to initialize system time, using UNIX calls
  static u_long init_simulator_time (void);
#     endif /* ACE_PSOSIM */

    /// max number of ticks supported in a single system call
  static const u_long max_ticks;
private:
  // = Constants for prying info out of the pSOS time encoding.
  static const u_long year_mask;
  static const u_long month_mask;
  static const u_long day_mask;
  static const u_long hour_mask;
  static const u_long minute_mask;
  static const u_long second_mask;
  static const int year_shift;
  static const int month_shift;
  static const int hour_shift;
  static const int minute_shift;
  static const int year_origin;
  static const int month_origin;

  // error codes
  static const u_long err_notime;   // system time not set
  static const u_long err_illdate;  // date out of range
  static const u_long err_illtime;  // time out of range
  static const u_long err_illticks; // ticks out of range

  /// date : year in bits 31-16, month in bits 15-8, day in bits 7-0
   u_long date_;

  /// time : hour in bits 31-16, minutes in bits 15-8, seconds in bits 7-0
  u_long time_;

  /// ticks: number of system clock ticks (KC_TICKS2SEC-1 max)
  u_long ticks_;
} ;
#   endif /* ACE_PSOS_HAS_TIME */

# endif /* defined (ACE_PSOS) */

# if defined (ACE_HAS_THREADS)

// Some versions of pSOS provide native mutex support.  For others,
// implement ACE_thread_mutex_t and ACE_mutex_t using pSOS semaphores.
// Either way, the types are all u_longs.
typedef u_long ACE_mutex_t;
typedef u_long ACE_thread_mutex_t;
typedef u_long ACE_thread_t;
typedef u_long ACE_hthread_t;

#   if defined (ACE_PSOS_HAS_COND_T)
typedef u_long ACE_cond_t;
typedef u_long ACE_condattr_t;
struct ACE_OS_Export ACE_mutexattr_t
{
  int type;
};
#   endif /* ACE_PSOS_HAS_COND_T */


// TCB registers 0-7 are for application use
#   define PSOS_TASK_REG_TSS 0
#   define PSOS_TASK_REG_MAX 7

#   define PSOS_TASK_MIN_PRIORITY   1
#   define PSOS_TASK_MAX_PRIORITY 239

// Key type: the ACE TSS emulation requires the key type be unsigned,
// for efficiency.  Current POSIX and Solaris TSS implementations also
// use unsigned int, so the ACE TSS emulation is compatible with them.
// Native pSOS TSD, where available, uses unsigned long as the key type.
#   if defined (ACE_PSOS_HAS_TSS)
typedef u_long ACE_thread_key_t;
#   else
typedef u_int ACE_thread_key_t;
#   endif /* ACE_PSOS_HAS_TSS */

#   define THR_CANCEL_DISABLE      0  /* thread can never be cancelled */
#   define THR_CANCEL_ENABLE       0      /* thread can be cancelled */
#   define THR_CANCEL_DEFERRED     0      /* cancellation deferred to cancellation point */
#   define THR_CANCEL_ASYNCHRONOUS 0      /* cancellation occurs immediately */

#   define THR_BOUND               0
#   define THR_NEW_LWP             0
#   define THR_DETACHED            0
#   define THR_SUSPENDED           0
#   define THR_DAEMON              0
#   define THR_JOINABLE            0

#   define THR_SCHED_FIFO          0
#   define THR_SCHED_RR            0
#   define THR_SCHED_DEFAULT       0
#   define USYNC_THREAD            T_LOCAL
#   define USYNC_PROCESS           T_GLOBAL

/* from psos.h */
/* #define T_NOPREEMPT     0x00000001   Not preemptible bit */
/* #define T_PREEMPT       0x00000000   Preemptible */
/* #define T_TSLICE        0x00000002   Time-slicing enabled bit */
/* #define T_NOTSLICE      0x00000000   No Time-slicing */
/* #define T_NOASR         0x00000004   ASRs disabled bit */
/* #define T_ASR           0x00000000   ASRs enabled */

/* #define SM_GLOBAL       0x00000001  1 = Global */
/* #define SM_LOCAL        0x00000000  0 = Local */
/* #define SM_PRIOR        0x00000002  Queue by priority */
/* #define SM_FIFO         0x00000000  Queue by FIFO order */

/* #define T_NOFPU         0x00000000   Not using FPU */
/* #define T_FPU           0x00000002   Using FPU bit */
# endif /* ACE_HAS_THREADS */


#endif /* ACE_ACE_PSOS_H */
