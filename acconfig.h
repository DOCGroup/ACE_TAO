/* -*- C++ -*- */
#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H

/* ACE configuration header file */

/*   @TOP@  */
/* All lines above "@TOP@" will be placed at the top of config.h.in. */

/*
  $Id$

  This file, "acconfig.h," contains additional preprocessor symbols that
  autoheader may encounter.
*/

/* Name of package */
#ifndef PACKAGE
# undef PACKAGE
#endif

/* Version of package */
#ifndef VERSION
# undef VERSION
#endif

/* Define _REENTRANT if reentrant functions should be used. */
#ifndef _REENTRANT
# undef _REENTRANT
#endif

#ifndef _POSIX_C_SOURCE
# undef _POSIX_C_SOURCE
#endif

#ifndef _POSIX_THREADS
# undef _POSIX_THREADS
#endif

#ifndef _POSIX_THREAD_SAFE_FUNCTIONS
# undef _POSIX_THREAD_SAFE_FUNCTIONS
#endif

/* Some platforms need _POSIX_PTHREAD_SEMANTICS to make some functions work */
#ifndef _POSIX_PTHREAD_SEMANTICS
# undef _POSIX_PTHREAD_SEMANTICS
#endif

/* Some platforms require _GNU_SOURCE to be defined to make some function
   prototypes "visible." */
#ifndef _GNU_SOURCE
# undef _GNU_SOURCE
#endif

/* ACE currently doesn't use these; however the configure script does */
#undef ACE_LACKS_SYSTIMES_H
#undef ACE_LACKS_UNBUFFERED_STREAMBUF
#undef ACE_HAS_STDCPP_STL_INCLUDES

#undef ACE_HAS_LIBC_H
#undef ACE_HAS_OSFCN_H
#undef ACE_HAS_NEW_NO_H
#undef ACE_HAS_NEW_H
#undef ACE_HAS_STDEXCEPT_NO_H
#undef ACE_HAS_EXCEPTION_H
#undef ACE_HAS_IOMANIP_NO_H

/* Platform provides <sys/ioctl.h> header */
#undef ACE_HAS_SYS_IOCTL_H

/*
 * Deprecated! (or soon to be?)
 */
#undef ACE_HAS_OSF1_GETTIMEOFDAY /* timezone* 2nd parameter & no prototype */
#undef ACE_HAS_LYNXOS_SIGNALS
#undef ACE_HAS_TANDEM_SIGNALS
#undef ACE_HAS_IRIX_53_SIGNALS

/*
 * TODO: These two next #defines have an #undef before them, in
 * case the variable being defined already had a value.
 * The #undefs are being picked up by configure, and are commented out!
 */
#undef ACE_THREAD_MIN_PRIORITY
#if defined (ACE_THREAD_MIN_PRIORITY)
# undef PTHREAD_MIN_PRIORITY
# define PTHREAD_MIN_PRIORITY ACE_THREAD_MIN_PRIORITY
#endif  /* #if defined (ACE_THREAD_MIN_PRIORITY) */

#undef ACE_THREAD_MAX_PRIORITY
#if defined (ACE_THREAD_MAX_PRIORITY)
# undef PTHREAD_MAX_PRIORITY
# define PTHREAD_MAX_PRIORITY ACE_THREAD_MAX_PRIORITY
#endif  /* #if defined (ACE_THREAD_MAX_PRIORITY) */

#undef PTHREAD_STACK_MIN
#undef PTHREAD_STACK_MAX

/* ///////////////////// OSSAMA'S NEW STUFF ////////////////// */
/* 
 THIS STUFF WILL REPLACE THE ABOVE OLDER STUFF AND/OR WILL BE MERGED INTO IT
*/

/* results from checks for programs */
/* results from checks for libraries */
/* results from checks for header files */
/* results from checks for typedefs */
/* results from checks for structures */
/* results from checks for variables */
/* results from checks for compiler characteristics */
/* results from checks for library functions */
/* results from checks for system services */

/* AIX specific configuration parameters */
#undef AIX
#undef _BSD
#undef _BSD_INCLUDES

/* Cray specific configuration parameters */
/*
    The following predefined macros are used within ACE ifdefs.  
    These are defined when using the Cray compilers.  _CRAYMPP
    is defined, for example, if you are running on a Cray T3E
    massively parallel machine.  Moreover, in the case of the T3E,
    _CRAYT3E will be defined.  This is used to determine the
    ACE_SIZEOF defines for primitive types.  

    _UNICOS is defined as either the major version of UNICOS being run,
    e.g. 9 or 10 on the vector machines (e.g. C90, T90, J90, YMP, ...)
    or the major+minor+level UNICOS/mk version, e.g. 2.0.3 => 203,
    being run on an MPP machine.

    Summary:

    _CRAYMPP  (defined only if running on MPP machine, e.g. T3E, UNICOS/mk)
    _CRAYT3E  (defined specifically if compiling on a Cray T3E)
    _UNICOS   (defined if running UNICOS or UNICOS/mk)

    Tested on UNICOS 10.0.0.2, UNICOS/mk 2.0.3.10

    Contributed by Doug Anderson <doug@clark.net>
*/
#undef _CRAYMPP
#undef _CRAYT3E
#undef _UNICOS

/* DG/UX specific configuration parameters */
#undef ACE_DGUX
#undef _DGUX_SOURCE
#undef _POSIX4A_DRAFT10_SOURCE
#undef _POSIX4_DRAFT_SOURCE

/* FreeBSD specific configuration parameters */
/* Nothing yet */

/* HP/UX specific configuration parameters */
#undef HPUX
#undef HPUX_10
#undef HPUX_11
#undef _HPUX_SOURCE

/* Irix specific configuration parameters */
#undef IRIX5
#undef IRIX6
#undef _BSD_TYPES
#undef _SGI_MP_SOURCE
#undef _MODERN_C_

/* Linux specific configuration parameters */
/* Nothing yet */

/* LynxOS specific configuration parameters */
#undef __NO_INCLUDE_WARN__
#undef _POSIX_THREADS_CALLS

/* M88K specific configuration parameters */
#undef m88k
#undef __m88k__

/* MVS specific configuration parameters */
/* Nothing yet */

/* NetBSD specific configuration parameters */
#undef ACE_NETBSD

/* OSF/1 and Digital Unix specific configuration parameters */
#undef DEC_CXX
#undef DIGITAL_UNIX

/* pSOS specific configuration parameters */
#undef ACE_PSOS
#undef ACE_PSOSIM
#undef ACE_PSOSTBD

/* SCO specific configuration parameters */
#undef SCO
#undef _SVID3

/* SunOS / Solaris specific configuration parameters */
/* Nothing yet */

/* Tandem specific configuration parameters */
/* Nothing yet */

/* UnixWare specific configuration parameters */
#undef UNIXWARE
#undef UNIXWARE_2_0
#undef UNIXWARE_2_1

/* VXWorks specific configuration parameters */
#undef VXWORKS

/* Win32 specific configuration parameters */
#undef ACE_WIN32

/* ACE internals */
#undef ACE_DEFAULT_BASE_ADDR
#undef ACE_DEFAULT_BASE_ADDRL
#undef ACE_DEFAULT_CLOSE_ALL_HANDLES
#undef ACE_DEFAULT_MAX_SOCKET_BUFSIZ
#undef ACE_DEFAULT_SELECT_REACTOR_SIZE
#undef ACE_MALLOC_ALIGN
#undef ACE_MAP_PRIVATE
#undef ACE_THR_PRI_FIFO_DEF
#undef ACE_TIMER_SKEW
#undef ACE_UINT64_FORMAT_SPECIFIER
#undef ACE_USE_RCSID
#undef IP_ADD_MEMBERSHIP
#undef IP_DROP_MEMBERSHIP

/* Specify sizes of given built-in types.  If a size isn't defined here,
   then ace/Basic_Types.h will attempt to deduce the size. */
/* #undef ACE_SIZEOF_CHAR */
#undef ACE_SIZEOF_SHORT
#undef ACE_SIZEOF_INT
#undef ACE_SIZEOF_LONG
#undef ACE_SIZEOF_LONG_LONG
#undef ACE_SIZEOF_VOID_P
#undef ACE_SIZEOF_FLOAT
#undef ACE_SIZEOF_DOUBLE
#undef ACE_SIZEOF_LONG_DOUBLE

/* typedef for ACE_UINT64 */
/*
   We only make the typedef if ACE_UINT64_TYPEDEF is defined and
   ACE_LACKS_LONGLONG_T isn't defined  .  Otherwise,
   let ace/Basic_Types.h do the work for us.  ACE_UINT64_TYPEDEF is
   defined during the initial configuration process if a 64 bit unsigned int
   was found.
*/
#undef ACE_UINT64_TYPEDEF
#if defined(ACE_UINT64_TYPEDEF)
  typedef ACE_UINT64_TYPEDEF ACE_UINT64;
#endif /* ACE_UINT64_TYPEDEF */

/* Enable ACE inlining */
#undef __ACE_INLINE__

/* Explicitly disable ACE inlining */
#undef ACE_NO_INLINE

/* Enable ACE_Timeprobes */
#undef ACE_COMPILE_TIMEPROBES

/* Enable use of GNU template repositories.  GNU C++ w/repo patch
   and EGCS only */
#undef ACE_HAS_GNU_REPO

/* Define this if you don't want debug version ACE search for debug version
   DLLs first before looking for the DLL names specified. */
#undef ACE_DISABLE_DEBUG_DLL_CHECK

/* Platform supports new C++ style casts (dynamic_cast, static_cast,
   reinterpret_cast and const_cast) */
#undef ACE_HAS_ANSI_CASTS

/* Platform supports Asynchronous IO calls */
#undef ACE_HAS_AIO_CALLS

/* Number of TSS keys, with ACE_HAS_TSS_EMULATION _only_.  Defaults to 64.  */
#undef ACE_DEFAULT_THREAD_KEYS

/* Specify this if you don't want threads to inherit parent thread's
   ACE_Log_Msg properties. */
#undef ACE_THREADS_DONT_INHERIT_LOG_MSG

/* Compiler enforces C++ One Definition Rule */
#undef ACE_HAS_ONE_DEFINITION_RULE

/* OS has priocntl (2) */
#undef ACE_HAS_PRIOCNTL

/* Platform has the MIT pthreads APIs for timed send/recv operations */
#undef ACE_HAS_RECV_TIMEDWAIT
#undef ACE_HAS_RECVFROM_TIMEDWAIT
#undef ACE_HAS_RECVMSG_TIMEDWAIT
#undef ACE_HAS_SEND_TIMEDWAIT
#undef ACE_HAS_SENDTO_TIMEDWAIT
#undef ACE_HAS_SENDMSG_TIMEDWAIT
#undef ACE_HAS_READ_TIMEDWAIT
#undef ACE_HAS_READV_TIMEDWAIT
#undef ACE_HAS_WRITE_TIMEDWAIT
#undef ACE_HAS_WRITEV_TIMEDWAIT

/* Platform has enum instead of int for first argument to ::{get,set}rlimit ().
   The value of this macro is the enum definition, e.g.,
   enum __rlimit_resource, for Linux glibc 2.0. */
#undef ACE_HAS_RLIMIT_RESOURCE_ENUM

/* Platform has enum instead of int for first argument to ::getrusage ().  The
   value of this macro is the enum definition, e.g., enum __rusage_who, for
   Linux glibc 2.0. */
#undef ACE_HAS_RUSAGE_WHO_ENUM

/* Platform also has __TEXT defined */
#undef ACE_HAS_TEXT_MACRO_CONFLICT

/* Platform has void (*)(...) prototype for pthread_key_create() destructor
   (e.g., LynxOS). */
#undef ACE_HAS_STDARG_THR_DEST

/* Used when users want to compile ACE with STL and STL map class conflicts
   with <net/if.h> map struct. */
#undef ACE_HAS_STL_MAP_CONFLICT

/* Used when users want to compile ACE with STL and STL queue class conflicts
   with <netinet/in.h> queue struct. */
#undef ACE_HAS_STL_QUEUE_CONFLICT

/* Platform/compiler supports Win32 structural exceptions */
#undef ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS

/* Platform has BSD 4.4 sendmsg()/recvmsg() APIs. */
#undef ACE_HAS_4_4BSD_SENDMSG_RECVMSG

/* Platform has pread() and pwrite() support */
#undef ACE_HAS_P_READ_WRITE

/* Platform, such as AIX4, needs to wrap #include of sys/socket.h with
   #undef/#define of __cplusplus. */
#undef ACE_HAS_AIX_BROKEN_SOCKET_HEADER

/* Platform has AIX4 ::read_real_time () */
#undef ACE_HAS_AIX_HI_RES_TIMER

/* Compiler/platform supports alloca() */
#undef ACE_HAS_ALLOCA

/* Compiler/platform has <alloca.h> */
#undef ACE_HAS_ALLOCA_H

/* CPU is an Alpha, with the rpcc instruction to read the tick timer. */
#undef ACE_HAS_ALPHA_TIMER

/* Compiler/platform correctly calls init()/fini() for shared libraries */
#undef ACE_HAS_AUTOMATIC_INIT_FINI

/* Compiler handles explicit calling of template destructor correctly.
   See "ace/OS.h" for details. */
#undef ACE_HAS_WORKING_EXPLICIT_TEMPLATE_DESTRUCTOR

/* Compiler/platform doesn't support namespaces (or the support is not fully
   implemented.) */
#undef ACE_HAS_BROKEN_NAMESPACES

/* Compiler has integer overflow problem with bit-shift operations. */
#undef ACE_HAS_BROKEN_BITSHIFT

/* Compiler can't handle const char * as rvalue in conditional operator. */
#undef ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS

/* Compiler can't handle calls like foo->operator T *() */
#undef ACE_HAS_BROKEN_CONVERSIONS

/* Compiler/platform uses macro for ctime (e.g., MVS) */
#undef ACE_HAS_BROKEN_CTIME

/* Earlier versions of HP/UX C++ are damned... */
#undef ACE_HAS_BROKEN_HPUX_TEMPLATES

/* Platform headers don't support <msg.h> prototypes */
#undef ACE_HAS_BROKEN_MSG_H

/* HP/UX does not wrap the mmap(2) header files with extern "C". */
#undef ACE_HAS_BROKEN_MMAP_H

/* MSVC has trouble with defining STL containers for nested structs and
   classes */
#undef ACE_HAS_BROKEN_NESTED_TEMPLATES

/* Platform has a bug with non-blocking connects (e.g., WinNT 4.0) */
#undef ACE_HAS_BROKEN_NON_BLOCKING_CONNECTS

/* Platform defines struct timespec in <sys/timers.h> */
#undef ACE_HAS_BROKEN_POSIX_TIME

/* Platform defines ctime_r, asctime_r, rand_r and getpwnam_r as macros */
#undef ACE_HAS_BROKEN_R_ROUTINES

/* OS/compiler's header files are inconsistent with libC definition of
   rand_r(). */
#undef ACE_HAS_BROKEN_RANDR

/* OS/Compiler's header files are not consistent with readv() definition. */
#undef ACE_HAS_BROKEN_READV

/* Compiler can't handle the static ACE_Addr::sap_any construct. */
#undef ACE_HAS_BROKEN_SAP_ANY

/* OS/compiler omits the const from the sendmsg() prototype. */
#undef ACE_HAS_BROKEN_SENDMSG

/* OS/compiler omits the const from the rlimit parameter in the setrlimit()
   prototype. */
#undef ACE_HAS_BROKEN_SETRLIMIT

/* platform define struct timespec members as ts_sec and ts_nsec instead of
   tv_sec and tv_nsec.  This is highly non-portable.  Currently only
   FreeBSD 2.1.x uses it. */
#undef ACE_HAS_BROKEN_TIMESPEC_MEMBERS

/* OS/compiler omits the const from the iovec parameter in the writev()
   prototype. */
#undef ACE_HAS_BROKEN_WRITEV

/* OS header files have some problems with XTI (HP/UX 11). */
#undef ACE_HAS_BROKEN_XTI_MACROS

/* Platform has <bstring.h> (which contains bzero() prototype) */
#undef ACE_HAS_BSTRING

/* Platform has <bytesex.h>. */
#undef ACE_HAS_BYTESEX_H

/* Platform has <features.h> */
#undef ACE_HAS_FEATURES_H

/* Platform supports the Win32 CancelIO() function (WinNT 4.0 and beyond). */
#undef ACE_HAS_CANCEL_IO

/* OS/platform uses char * for dlopen/dlsym args, rather than const char *. */
#undef ACE_HAS_CHARPTR_DL

/* OS/platform uses char * for sockopt, rather than const char * */
#undef ACE_HAS_CHARPTR_SOCKOPT

/* sprintf() returns char * rather than int (e.g., SunOS 4.x)  */
#undef ACE_HAS_CHARPTR_SPRINTF

/* Platform supports POSIX 1.b clock_gettime () */
#undef ACE_HAS_CLOCK_GETTIME

/* Prototypes for both signal() and struct sigaction are consistent. */
#undef ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

/* Compiler/platform has correctly prototyped header files */
#undef ACE_HAS_CPLUSPLUS_HEADERS

/* Platform has cygwin32 socket.h */
#undef ACE_HAS_CYGWIN32_SOCKET_H

/* Platform supports operations on directories via struct dirent,
   readdir_r, etc. */
#undef ACE_HAS_DIRENT

/* For platforms, e.g., RedHat 4.2/Linux 2.0.30/Alpha, that don't declare dl*
   functions as extern "C" in dlfcn.h.*/
#undef ACE_HAS_DLFCN_H_BROKEN_EXTERN_C

/* Build ACE using the frigging PC DLL nonsense... */
#undef ACE_HAS_DLL

/* Compiler supports C++ exception handling */
#undef ACE_HAS_EXCEPTIONS

/* Platform supports getpagesize() call (otherwise, ACE_PAGE_SIZE must be
   defined, except on Win32) */
#undef ACE_HAS_GETPAGESIZE

/* Platform supports the getrusage() system call. */
#undef ACE_HAS_GETRUSAGE

/* Platform has a getrusage () prototype in sys/resource.h that differs from
   the one in ace/OS.i. */
#undef ACE_HAS_GETRUSAGE_PROTO

/* GNUC 2.7.3 mistakenly takes the template definition as the place where an
   inline function of an argument class is first used. */
#undef ACE_HAS_GNUC_BROKEN_TEMPLATE_INLINE_FUNCTIONS

/* Denotes that GNU has cstring.h as standard which redefines memchr() */
#undef ACE_HAS_GNU_CSTRING_H

/* The GPERF utility is compiled for this platform */
#undef ACE_HAS_GPERF

/* Optimize ACE_Handle_Set::count_bits for select() operations (common case) */
#undef ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

/* Compiler/platform supports SunOS high resolution timers */
#undef ACE_HAS_HI_RES_TIMER

/* Compiler/platform supports idtype_t. */
#undef ACE_HAS_IDTYPE_T

/* Inline all the static class OS methods to remove call overhead */
/* Note: This gets defined by OS.h if __ACE_INLINE__ is defined */
#undef ACE_HAS_INLINED_OSCALLS

/* Platform supports IP multicast */
#undef ACE_HAS_IP_MULTICAST

/* Platform supports the very odd IRIX 6.2 threads... */
#undef ACE_HAS_IRIX62_THREADS

/* Causes the ACE_Object_Manager instance to be created in
   main (int, char *[]), instead of as a static (global) instance. */
#undef ACE_HAS_NONSTATIC_OBJECT_MANAGER

/* Platform supports thr_keydelete (e.g,. UNIXWARE) */
#undef ACE_HAS_THR_KEYDELETE

/* Platform calls thr_minstack() rather than thr_min_stack() (e.g., Tandem). */
#undef ACE_HAS_THR_MINSTACK

/* The rusage_t structure has only two fields. */
#undef ACE_HAS_LIMITED_RUSAGE_T

/* Compiler/platform has "big" fd_set, i.e. large number of bits set
   in fd_set passed back from select(). */
#undef ACE_HAS_BIG_FD_SET

/* Platform defines MAP_FAILED as a long constant. */
#undef ACE_HAS_LONG_MAP_FAILED

/* Enabled malloc statistics collection. */
#undef ACE_HAS_MALLOC_STATS

/* Avoid #including ace/streams.h in OS.h.  Users must include ace/streams.h,
   <iostream>, or <iostream.h> explicitly in their code.  Some platforms, such
   as g++/VxWorks, have trouble compiling templates and iostreams header
   because of static variables in the stream headers.  This flag will also
   avoid extra compilation and runtime overheads on some platforms. */
#undef ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION

/* Some files, such as ace/streams.h, want to include new style C++ stream
   headers.  These headers are iomanip, ios, iostream, istream, ostream,
   fstream and streambuf.  If _all_ of these headers aren't available, then
   assume that only iostream.h and fstream.h are available. */
#undef ACE_USES_OLD_IOSTREAMS

/* Platform supports Microsoft Foundation Classes */
#undef ACE_HAS_MFC

/* Platform supports recvmsg and sendmsg */
#undef ACE_HAS_MSG

/* Platform supports MT safe mktime() call (do any of them?) */
#undef ACE_HAS_MT_SAFE_MKTIME

/* Sockets may be called in multi-threaded programs */
#undef ACE_HAS_MT_SAFE_SOCKETS

/* Platform uses non-const char * in calls to gethostbyaddr, gethostbyname,
   getservbyname */
#undef ACE_HAS_NONCONST_GETBY

/* Platform has a non-const parameter to msgsnd() (e.g., SCO). */
#undef ACE_HAS_NONCONST_MSGSND

/* Platform's select() uses non-const timeval* (only found on Linux right
   now) */
#undef ACE_HAS_NONCONST_SELECT_TIMEVAL

/* Platform has "old" GNU compiler,  i.e. does not completely support
   standard C++. (compiling with g++ prior to version 2.8.0) */
#undef ACE_HAS_GNUG_PRE_2_8

/* Compiler/platform uses old malloc()/free() prototypes (ugh) */
#undef ACE_HAS_OLD_MALLOC

/* Uses ctime_r & asctime_r with only two parameters vs. three. */
#undef ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R

/* Platform, e.g., Solaris 2.5, only supports SCHED_OTHER POSIX scheduling
   policy. */
#undef ACE_HAS_ONLY_SCHED_OTHER

/* Use the semaphore implementation of ACE_Message_Queue rather than the
   emulated condition variable (NT and VxWorks). */
#undef ACE_HAS_OPTIMIZED_MESSAGE_QUEUE

/* Platform has Orbix CORBA implementation */
#undef ACE_HAS_ORBIX

/* Platform supports the OSF TLI timod STREAMS module */
#undef ACE_HAS_OSF_TIMOD_H

/* Platform is an Intel Pentium microprocessor. */
#undef ACE_HAS_PENTIUM

/* Platform contains <poll.h> */
#undef ACE_HAS_POLL

/* Platform supports POSIX O_NONBLOCK semantics */
#undef ACE_HAS_POSIX_NONBLOCK

/* Platform supports POSIX real-time semaphores (e.g., VxWorks and Solaris) */
#undef ACE_HAS_POSIX_SEM

/* Platform supports the POSIX struct timespec type */
#undef ACE_HAS_POSIX_TIME

/* Platform supports the /proc file system and defines tid_t
   in <sys/procfs.h> */
#undef ACE_HAS_PROC_FS

/* Platform supports PowerPC time-base register. */
#undef ACE_HAS_POWERPC_TIMER

/* Platform supports the prusage_t struct */
#undef ACE_HAS_PRUSAGE_T

/* Platform supports POSIX Threads */
#undef ACE_HAS_PTHREADS

/* Platform supports POSIX Threads .4a Draft 4 */
#undef ACE_HAS_PTHREADS_DRAFT4

/* Platform supports POSIX Threads .4a Draft 6 */
#undef ACE_HAS_PTHREADS_DRAFT6

/* Platform supports POSIX Threads .1c Draft 7 */
#undef ACE_HAS_PTHREADS_DRAFT7

/* Platform supports POSIX.1c-1995 threads */
#undef ACE_HAS_PTHREADS_STD

/* Platform has pthread_condattr_setkind_np(). */
#undef ACE_HAS_PTHREAD_CONDATTR_SETKIND_NP

/* Platform has pthread_mutexattr_setkind_np(). */
#undef ACE_HAS_PTHREAD_MUTEXATTR_SETKIND_NP

/* pthread.h declares an enum with PTHREAD_PROCESS_PRIVATE and
   PTHREAD_PROCESS_SHARED values */
#undef ACE_HAS_PTHREAD_PROCESS_ENUM

/* Platform has pthread_sigmask() defined. */
#undef ACE_HAS_PTHREAD_SIGMASK

/* Purify'ing.  Defined on command line. */
#undef ACE_HAS_PURIFY

/* Quantify'ing.  Defined on command line. */
#undef ACE_HAS_QUANTIFY

/* Platform will recurse infinitely on thread exits from TSS cleanup routines
   (e.g., AIX) */
#undef ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS

/* Platform supports reentrant functions (i.e., all the POSIX *_r
   functions). */
#undef ACE_HAS_REENTRANT_FUNCTIONS

/* Compiler typedefs wchar with char. */
#undef ACE_HAS_WCHAR_TYPEDEFS_CHAR

/* Solaris for intel uses macros for fstat() and stat(), these are wrappers for
   _fxstat() and _xstat() uses of the macros.  Causes compile and runtime
   problems. */
#undef ACE_HAS_X86_STAT_MACROS

/* Platform has support for multi-byte character support compliant with the
   XPG4 Worldwide Portability Interface wide-character classification. */
#undef ACE_HAS_XPG4_MULTIBYTE_CHAR

/* No system support for replacing any previous mappings. */
#undef ACE_LACKS_AUTO_MMAP_REPLACEMENT

/* Compiler/platform lacks the standard C library bsearch() function  */
#undef ACE_LACKS_BSEARCH

/* Platform has ACE_HAS_4_4BSD_SENDMSG_RECVMSG but does not define
   CMSG_DATA (cmsg) macro. */
#undef ACE_LACKS_CMSG_DATA_MACRO

/* Platform has ACE_HAS_4_4BSD_SENDMSG_RECVMSG but its cmsghdr structure does
   not contain an 'unsigned char cmsg_data[0]' member.  (This may be
   'unsigned char __cmsg_data[0]' on some platforms, in which case we need
   another macro.) */
#undef ACE_LACKS_CMSG_DATA_MEMBER

/* Compiler/platform lacks the standard C library qsort() function */
#undef ACE_LACKS_QSORT

/* Platform lacks POSIX-style fcntl () */
#undef ACE_LACKS_FCNTL

/* Platform lacks fsync() */
#undef ACE_LACKS_FSYNC

/* Compiler does not support dynamic_cast.  Usually used
   with ACE_HAS_ANSI_CASTS */
#undef ACE_LACKS_RTTI

/* Platform lacks readdir_r() */
#undef ACE_LACKS_READDIR_R

/* Platform lacks seekdir() */
#undef ACE_LACKS_SEEKDIR

/* Platform lacks telldir() */
#undef ACE_LACKS_TELLDIR

/* Platform can't handle "inline" keyword correctly. */
#undef ACE_LACKS_INLINE_FUNCTIONS

/* Iostreams are not supported adequately on the given platform. */
#undef ACE_LACKS_IOSTREAM_TOTALLY

/* Platform does not support reentrant netdb functions (getprotobyname_r,
   getprotobynumber_r, gethostbyaddr_r, gethostbyname_r, getservbyname_r). */
#undef ACE_LACKS_NETDB_REENTRANT_FUNCTIONS

/* Platform supports the POSIX regular expression library */
#undef ACE_HAS_REGEX

/* Platform has special header for select(). */
#undef ACE_HAS_SELECT_H

/* For Win32: Use Select_Reactor as default implementation of Reactor instead
   of WFMO_Reactor. */
#undef ACE_USE_SELECT_REACTOR_FOR_REACTOR_IMPL

/* Compiler/platform defines a union semun for SysV shared memory */
#undef ACE_HAS_SEMUN

/* Platform has a function to set t_errno (e.g., Tandem). */
#undef ACE_HAS_SET_T_ERRNO

/* Platform has shm_open() */
#undef ACE_HAS_SHM_OPEN

/* Platform supports SVR4 extended signals */
#undef ACE_HAS_SIGINFO_T

/* Platform has bug with sigismember() (HP/UX 11). */
#undef ACE_HAS_SIGISMEMBER_BUG

/* Platform/compiler has macros for sig{empty,fill,add,del}set (e.g., SCO and
   FreeBSD) */
#undef ACE_HAS_SIG_MACROS

/* Platform supports the Win32 SignalObjectAndWait() function (WinNT 4.0 and
   beyond). */
#undef ACE_HAS_SIGNAL_OBJECT_AND_WAIT

/* Automatically restart OS system calls when EINTR occurs */
#undef ACE_HAS_SIGNAL_SAFE_OS_CALLS

/* Platform/compiler has the sigwait(2) prototype */
#undef ACE_HAS_SIGWAIT

/* Compiler/platform defines the sig_atomic_t typedef */
#undef ACE_HAS_SIG_ATOMIC_T

/* Compiler requires extern "C" functions for signals. */
#undef ACE_HAS_SIG_C_FUNC

/* Platform supports new BSD inet_addr len field. */
#undef ACE_HAS_SIN_LEN

/* OS/compiler uses size_t * rather than int * for socket lengths */
#undef ACE_HAS_SIZET_SOCKET_LEN

/* Platform requires (struct sockaddr *) for msg_name field of
   struct msghdr. */
#undef ACE_HAS_SOCKADDR_MSG_NAME

/* Compiler/platform provides the sys/sockio.h file */
#undef ACE_HAS_SOCKIO_H

/* Platform provides socklen_t type, such as Linux with glibc2. */
#undef ACE_HAS_SOCKLEN_T

/* Compiler has brain-damaged SPARCwork SunOS 4.x signal prototype... */
#undef ACE_HAS_SPARCWORKS_401_SIGNALS

/* Compiler supports the ssize_t typedef */
#undef ACE_HAS_SSIZE_T

/* Platform supports UNIX International Threads */
#undef ACE_HAS_STHREADS

/* Platform has thr_yield() */
#undef ACE_HAS_THR_YIELD

/* Platform/compiler supports Standard C++ Library */
#undef ACE_HAS_STANDARD_CPP_LIBRARY

/* Compiler/platform supports struct strbuf */
#undef ACE_HAS_STRBUF_T

/* Platform/compiler lacks strdup() (e.g., VxWorks, Chorus, WinCE) */
#undef ACE_HAS_STRDUP_EMULATION

/* Platform supports STREAMS */
#undef ACE_HAS_STREAMS

/* Platform supports STREAM pipes */
#undef ACE_HAS_STREAM_PIPES

/* Compiler/platform supports strerror () */
#undef ACE_HAS_STRERROR

/* Use the STRICT compilation mode on Win32. */
#undef ACE_HAS_STRICT

/* Platform/Compiler supports a String class (e.g., GNU or Win32). */
#undef ACE_HAS_STRING_CLASS

/* Platform has <strings.h> (which contains bzero() prototype) */
#undef ACE_HAS_STRINGS

/* Compiler/platform has strange hostent API for socket *_r() calls */
#undef ACE_HAS_STRUCT_NETDB_DATA

/* Platform has void * as second parameter to gettimeofday and a has a
   prototype */
#undef ACE_HAS_SUNOS4_GETTIMEOFDAY

/* Compiler has horrible SunOS 4.x signal handlers... */
#undef ACE_HAS_SUNOS4_SIGNAL_T

/* Compiler/platform supports SVR4 dynamic linking semantics */
#undef ACE_HAS_SVR4_DYNAMIC_LINKING

/* Compiler/platform supports SVR4 gettimeofday() prototype but doesn't have
   a prototype */
#undef ACE_HAS_SVR4_GETTIMEOFDAY

/* Compiler/platform supports SVR4 signal typedef */
#undef ACE_HAS_SVR4_SIGNAL_T

/* Compiler/platform supports SVR4 TLI (in particular, T_GETNAME stuff)... */
#undef ACE_HAS_SVR4_TLI

/* HP/UX has an undefined syscall for GETRUSAGE... */
#undef ACE_HAS_SYSCALL_GETRUSAGE

/* Compiler/platform contains the <sys/syscall.h> file. */
#undef ACE_HAS_SYSCALL_H

/* Platform provides <sysent.h> header */
#undef ACE_HAS_SYSENT_H

/* Platform supports system configuration information */
#undef ACE_HAS_SYSINFO

/* Platform supports System V IPC (most versions of UNIX, but not Win32) */
#undef ACE_HAS_SYSV_IPC

/* Platform/compiler supports _sys_errlist symbol */
#undef ACE_HAS_SYS_ERRLIST

/* Platform provides <sys/filio.h> header */
#undef ACE_HAS_SYS_FILIO_H

/* Compiler/platform supports _sys_siglist array */
#undef ACE_HAS_SYS_SIGLIST

/* Platform provides <sys/xti.h> header */
#undef ACE_HAS_SYS_XTI_H

/* Compiler implements template specialization */
#undef ACE_HAS_TEMPLATE_SPECIALIZATION

/* Compiler implements templates that support typedefs inside of classes used
   as formal arguments to a template class. */
#undef ACE_HAS_TEMPLATE_TYPEDEFS

/* Platform has terminal ioctl flags like TCGETS and TCSETS. */
#undef ACE_HAS_TERM_IOCTLS

/* Platform supports threads */
#undef ACE_HAS_THREADS

/* Platform allows multiple threads to call accept() on the same port
   (e.g., WinNT). */
#undef ACE_HAS_THREAD_SAFE_ACCEPT

/* Platform has thread_self() rather than pthread_self() (e.g., DCETHREADS
   and AIX) */
#undef ACE_HAS_THREAD_SELF

/* Compiler/platform has thread-specific storage */
#undef ACE_HAS_THREAD_SPECIFIC_STORAGE

/* The pthread_keycreate() routine *must* take extern C functions. */
#undef ACE_HAS_THR_C_DEST

/* The pthread_create() routine *must* take extern C functions. */
#undef ACE_HAS_THR_C_FUNC

/* Platform/compiler supports timezone * as second parameter to
   gettimeofday() and has a prototype*/
#undef ACE_HAS_TIMEZONE_GETTIMEOFDAY

/* Platform supports TLI timod STREAMS module */
#undef ACE_HAS_TIMOD_H

/* Platform supports TLI tiuser header */
#undef ACE_HAS_TIUSER_H

/* Platform does not protect <tiuser.h> with extern "C" */
#undef ACE_HAS_TIUSER_H_BROKEN_EXTERN_C

/* Platform supports TLI.  Also see ACE_TLI_TCP_DEVICE. */
#undef ACE_HAS_TLI

/* Platform provides TLI function prototypes */
#undef ACE_HAS_TLI_PROTOTYPES

/* ACE provides TSS emulation.  See also ACE_DEFAULT_THREAD_KEYS. */
#undef ACE_HAS_TSS_EMULATION

/* Platform supports ualarm() */
#undef ACE_HAS_UALARM

/* Platform supports ucontext_t (which is used in the extended signal API). */
#undef ACE_HAS_UCONTEXT_T

/* The wait() system call takes a (union wait *) rather than int * */
#undef ACE_HAS_UNION_WAIT

/* Has inconsistent SVR4 signal stuff, but not the same as the other
   platforms */
#undef ACE_HAS_UNIXWARE_SVR4_SIGNAL_T

/* Platform/compiler supports UNICODE */
#undef ACE_HAS_UNICODE

/* Platform has <utime.h> header file */
#undef ACE_HAS_UTIME

/* Most APIs in platform/compiler supports UNICODE. (No char version
   available.) */
#undef ACE_HAS_MOSTLY_UNICODE_APIS

/* Compiler supports the C++ typename keyword */
#undef ACE_HAS_TYPENAME_KEYWORD

/* Compiler supports the new using keyword for C++ namespaces. */
#undef ACE_HAS_USING_KEYWORD

/* Prints out console message in ACE_NOTSUP.  Useful for tracking down origin
   of ACE_NOTSUP. */
#undef ACE_HAS_VERBOSE_NOTSUP

/* Platform requires void * for mmap(). */
#undef ACE_HAS_VOIDPTR_MMAP

/* OS/compiler uses void * arg 4 setsockopt() rather than const char * */
#undef ACE_HAS_VOIDPTR_SOCKOPT

/* The Win32 platform support TryEnterCriticalSection() (WinNT 4.0 and
   beyond) */
#undef ACE_HAS_WIN32_TRYLOCK

/* The Win32 platform supports WinSock 2.0 */
#undef ACE_HAS_WINSOCK2

/* Platform has the XLI version of TLI */
#undef ACE_HAS_XLI

/* Platform has Xt Intrinsics Toolkit */
#undef ACE_HAS_XT

/* Platform does not have Motif X toolkit available */
#undef ACE_LACKS_MOTIF

/* Platform has XTI (X/Open-standardized superset of TLI).  Implies
   ACE_HAS_TLI but uses a different header file. */
#undef ACE_HAS_XTI

/* Platform lacks access() (e.g., VxWorks and Chorus) */
#undef ACE_LACKS_ACCESS

/* Platform can not build ace/IOStream{,_T}.cpp.  This does not necessarily
   mean that the platform does not support iostreams. */
#undef ACE_LACKS_ACE_IOSTREAM

/* Platform lacks condition variables (e.g., Win32 and VxWorks) */
#undef ACE_LACKS_COND_T

/* pthread_cond_timedwait does *not* reset the time argument when
   the lock is acquired. */
#undef ACE_LACKS_COND_TIMEDWAIT_RESET

/* Platform has no implementation of pthread_condattr_setpshared(), even
   though it supports pthreads! */
#undef ACE_LACKS_CONDATTR_PSHARED

/* Platform uses struct strbuf * rather than const struct strbuf * (e.g.,
   HP/UX 10.x) */
#undef ACE_LACKS_CONST_STRBUF_PTR

/* Platform forgot const in cond_timewait (e.g., HP/UX). */
#undef ACE_LACKS_CONST_TIMESPEC_PTR

/* Platform lacks difftime() implementation */
#undef ACE_LACKS_DIFFTIME

/* Platform lacks the exec() family of system calls (e.g., Win32, VxWorks,
   Chorus) */
#undef ACE_LACKS_EXEC

/* Platform lacks file locking mechanism */
#undef ACE_LACKS_FILELOCKS

/* Platform does not support floating point operations */
#undef ACE_LACKS_FLOATING_POINT

/* Platform lacks the fork() system call (e.g., Win32, VxWorks, Chorus) */
#undef ACE_LACKS_FORK

/* Platform lacks the getopt() prototype (e.g., LynxOS) */
#undef ACE_LACKS_GETOPT_PROTO

/* Platform lacks getpgid() call (e.g., Win32, Chorus, and FreeBSD). */
#undef ACE_LACKS_GETPGID

/* Platforms lacks getservbyname() (e.g., VxWorks and Chorus). */
#undef ACE_LACKS_GETSERVBYNAME

/* iostream header does not declare ipfx (), opfx (), etc. */
#undef ACE_LACKS_IOSTREAM_FX

/* Platform lacks streambuf "linebuffered ()". */
#undef ACE_LACKS_LINEBUFFERED_STREAMBUF

/* Compiler/platform does not support the unsigned long long datatype. */
#undef ACE_LACKS_LONGLONG_T

/* Platform lacks the lstat() function. */
#undef ACE_LACKS_LSTAT

/* Platform does not have u_longlong_t typedef */
#undef ACE_LACKS_U_LONGLONG_T

/* Platform lacks madvise() (e.g., Linux) */
#undef ACE_LACKS_MADVISE

/* Platform lacks malloc.h */
#undef ACE_LACKS_MALLOC_H

/* Platform lacks memory.h (e.g., VxWorks and Chorus) */
#undef ACE_LACKS_MEMORY_H

/* Platform lacks mkfifo(), e.g. VxWorks, Chorus, pSoS, and WinNT. */
#undef ACE_LACKS_MKFIFO

/* ACE has no mktemp() */
#undef ACE_LACKS_MKTEMP

/* The platform doesn't have mmap(2) (e.g., SCO UNIX). */
#undef ACE_LACKS_MMAP

/* Platform/compiler doesn't have open() mode masks. */
#undef ACE_LACKS_MODE_MASKS

/* The platform doesn't have mprotect(2) (e.g., EPLX real time OS from CDC
   (based on LYNX)) */
#undef ACE_LACKS_MPROTECT

/* Platform defines ACE_HAS_MSG, but lacks msg_accrights{,len}. */
#undef ACE_LACKS_MSG_ACCRIGHTS

/* Platform lacks struct msgbuf (e.g., NT and MSV). */
#undef ACE_LACKS_MSGBUF_T

/* Platform lacks msync() */
#undef ACE_LACKS_MSYNC

/* Platform doesn't support "sync" third argument to msync (?) */
#undef ACE_HAS_BROKEN_NETBSD_MSYNC

/* Platform doesn't support tempnam */
#undef ACE_LACKS_TEMPNAM

/* Platform lacks pthread_mutexattr_setpshared(). */
#undef ACE_LACKS_MUTEXATTR_PSHARED

/* Platform lacks <sys/param.h> (e.g., MVS) */
#undef ACE_LACKS_PARAM_H

/* Platform lacks named POSIX semaphores (e.g., Chorus) */
#undef ACE_LACKS_NAMED_POSIX_SEM

/* Platform/compiler lacks {get,set}rlimit() function (e.g., VxWorks, Chorus,
   and SCO UNIX) */
#undef ACE_LACKS_RLIMIT

/* Platform/compiler lacks {get,set}rlimit() prototypes (e.g., Tandem) */
#undef ACE_LACKS_RLIMIT_PROTOTYPE

/* Platform lacks POSIX prototypes for certain System V functions like shared
   memory and message queues. */
#undef ACE_LACKS_POSIX_PROTOTYPES

/* Platform lacks POSIX prototypes for certain System V functions like shared
   memory and message queues. */
#undef ACE_LACKS_SOME_POSIX_PROTOTYPES

/* Compiler complains about "#pragma once" */
#undef ACE_LACKS_PRAGMA_ONCE

/* Platform lacks pri_t (e.g., Tandem NonStop UNIX). */
#undef ACE_LACKS_PRI_T

/* Platform lack pthread_attr_setstackaddr() */
#undef ACE_LACKS_THREAD_STACK_ADDR

/* Platform lacks pthread_cancel(). */
#undef ACE_LACKS_PTHREAD_CANCEL

/* Platform lacks pthread_thr_sigsetmask (e.g., MVS, HP/UX, and OSF/1 3.2) */
#undef ACE_LACKS_PTHREAD_THR_SIGSETMASK

/* Platfrom lack pthread_yield() support. */
#undef ACE_LACKS_PTHREAD_YIELD

/* Platform lacks, getpwnam(), etc. */
#undef ACE_LACKS_PWD_FUNCTIONS

/* Platform lacks getpwnam_r() methods (e.g., SGI 6.2). */
#undef ACE_LACKS_PWD_REENTRANT_FUNCTIONS

/* Platform lacks the readlink() function. */
#undef ACE_LACKS_READLINK

/* Platform lacks the rename() function. */
#undef ACE_LACKS_RENAME

/* Platform lacks recvmsg() */
#undef ACE_LACKS_RECVMSG

/* Platform lacks readers/writer locks. */
#undef ACE_LACKS_RWLOCK_T

/* Platform lacks a working sbrk() (e.g., Win32 and VxWorks) */
#undef ACE_LACKS_SBRK

/* Platform lacks struct sembuf (e.g., Win32 and VxWorks) */
#undef ACE_LACKS_SEMBUF_T

/* Platform lacks pthread_attr_setdetachstate() (e.g., HP/UX 10.x) */
#undef ACE_LACKS_SETDETACH

/* Platform lacks pthread_attr_setsched() (e.g. MVS) */
#undef ACE_LACKS_SETSCHED

/* Platform lacks struct sigaction (e.g., Win32 and Chorus) */
#undef ACE_LACKS_SIGACTION

/* Platform lacks "signed char" type (broken!) */
#undef ACE_LACKS_SIGNED_CHAR

/* Platform lacks signal sets (e.g., Chorus and Win32) */
#undef ACE_LACKS_SIGSET

/* Platform/compiler lacks strrchr () function. */
#undef ACE_LACKS_STRRCHR

/* Platforms/compiler lacks the sys_nerr variable (e.g., VxWorks and MVS). */
#undef ACE_LACKS_SYS_NERR

/* Platform lacks sys/msg.h (e.g., Chorus and VxWorks) */
#undef ACE_LACKS_SYSV_MSG_H

/* Platform lacks SYSV message queue prototypes */
#undef ACE_LACKS_SYSV_MSQ_PROTOS

/* Platform lacks key_t (e.g., Chorus, VxWorks, Win32) */
#undef ACE_LACKS_KEY_T

/* Platform lacks sendmsg() */
#undef ACE_LACKS_SENDMSG

/* Platform lacks the si_addr field of siginfo_t (e.g., VxWorks and
   HP/UX 10.x) */
#undef ACE_LACKS_SI_ADDR

/* Platform lacks System V shared memory (e.g., Win32 and VxWorks) */
#undef ACE_LACKS_SYSV_SHMEM

/* Platform lacks the siginfo.h include file (e.g., MVS) */
#undef ACE_LACKS_SIGINFO_H

/* Platform doesn't support SO_SNDBUF/SO_RCVBUF (used in TAO) */
#undef ACE_LACKS_SOCKET_BUFSIZ

/* Platform lacks the socketpair() call (e.g., SCO UNIX) */
#undef ACE_LACKS_SOCKETPAIR

/* Compiler doesn't support static data member templates */
#undef ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES

/* Compiler/platform lacks strcasecmp() (e.g., DG/UX, UNIXWARE, VXWORKS) */
#undef ACE_LACKS_STRCASECMP

/* Platform doesn't define struct strrecvfd. */
#undef ACE_LACKS_STRRECVFD

/* Platform doesn't have syscall() prototype */
#undef ACE_LACKS_SYSCALL

/* Platform lacks <sys/types.h> header file */
#undef ACE_LACKS_SYS_TYPES_H

/* Header files lack t_errno for TLI */
#undef ACE_LACKS_T_ERRNO

/* Platform doesn't have netinet/tcp.h */
#undef ACE_LACKS_TCP_H

/* Platform lacks pthread_attr_setscope() */
#undef ACE_LACKS_THREAD_PROCESS_SCOPING

/* Platform lacks pthread_attr_setstacksize() (e.g., Linux pthreads) */
#undef ACE_LACKS_THREAD_STACK_SIZE

/* MIT pthreads platform lacks the timedwait prototypes */
#undef ACE_LACKS_TIMEDWAIT_PROTOTYPES

/* Platform does not define timepec_t as a typedef for struct timespec. */
#undef ACE_LACKS_TIMESPEC_T

/* Platform doesn't have truncate() (e.g., vxworks) */
#undef ACE_LACKS_TRUNCATE

/* Platform/compiler lacks the ualarm() prototype (e.g., Solaris) */
#undef ACE_LACKS_UALARM_PROTOTYPE

/* Platform lacks the ucontext.h file */
#undef ACE_LACKS_UCONTEXT_H

/* Platform lacks the unistd.h file (e.g., VxWorks and Win32) */
#undef ACE_LACKS_UNISTD_H

/* ACE platform has no UNIX domain sockets */
#undef ACE_LACKS_UNIX_DOMAIN_SOCKETS

/* Platform lacks full signal support (e.g., Win32 and Chorus). */
#undef ACE_LACKS_UNIX_SIGNALS

/* Platform lacks struct utsname (e.g., Win32 and VxWorks) */
#undef ACE_LACKS_UTSNAME_T

/* Platform lacks wchar_t typedef */
#undef ACE_LACKS_WCHAR_T

/* The bind() call will not select the port if it's 0. */
#undef ACE_LACKS_WILDCARD_BIND

/* Renames "main (int, char *[])", for platforms such as g++/VxWorks that
   don't allow main.  Requires the use of ACE_HAS_NONSTATIC_OBJECT_MANAGER. */
#undef ACE_MAIN

/* Compile using multi-thread libraries */
#undef ACE_MT_SAFE

/* Turns off debugging features */
#undef ACE_NDEBUG

/* Necessary with some compilers to pass ACE_TTY_IO as parameter to
   DEV_Connector. */
#undef ACE_NEEDS_DEV_IO_CONVERSION

/* Required by platforms with small default stacks. */
#undef ACE_NEEDS_HUGE_THREAD_STACKSIZE

/* OS has LWPs, and when the priority of a bound thread is set, then the LWP
   priority must be set also. */
#undef ACE_NEEDS_LWP_PRIO_SET

/* Platform doesn't define readv, so use our own */
#undef ACE_LACKS_READV

/* Platform doesn't define writev, so use our own */
#undef ACE_LACKS_WRITEV

/* Platform needs regexpr.h for regular expression support */
#undef ACE_NEEDS_REGEXPR_H

/* Platform needs to #include <sched.h> to get thread scheduling defs. */
#undef ACE_LACKS_SCHED_H

/* <time.h> doesn't automatically #include <sys/time.h> */
#undef ACE_LACKS_SYSTIME_H

/* Compiler's 'new' throws exception on failure (ANSI C++ behavior). */
#undef ACE_NEW_THROWS_EXCEPTIONS

/* Turns off the LM_DEBUG and LM_ERROR logging macros... */
#undef ACE_NLOGGING

/* Turns off the tracing feature. */
#undef ACE_NTRACE

/* Defines the page size of the system (not used on Win32 or with
   ACE_HAS_GETPAGESIZE). */
#undef ACE_PAGE_SIZE

/* Platform redefines the t_... names (UnixWare) */
#undef ACE_REDEFINES_XTI_FUNCTIONS

/* Platform uses int for select() rather than fd_set */
#undef ACE_SELECT_USES_INT

/* Compiler's template mechanism must use a pragma.  This is used for AIX's
   C++ compiler. */
#undef ACE_TEMPLATES_REQUIRE_PRAGMA

/* Compiler's template mechanim must see source code (i.e., .cpp files).  This
   is used for GNU G++. */
#undef ACE_TEMPLATES_REQUIRE_SOURCE

/* Compiler's template instantiation mechanism supports the use of explicit
   C++ specializations for all used templates. This is also used for GNU G++
   if you don't use the "repo" patches. */
#undef ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION

/* Compiler's template instantiation mechanism supports the use of
   "#pragma instantiate".  Edison Design Group compilers, e.g., SGI C++ and
   Green Hills 1.8.8 and later, support this. */
#undef ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA

/* Compiler requires a definition for a "hidden" function, e.g., a private,
   unimplemented copy constructor or assignment operator.  The SGI C++
   compiler needs this, in template classes, with
   ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA. */
#undef ACE_NEEDS_FUNC_DEFINITIONS

/* Device the platform uses for TCP on TLI.  Only needed if not /dev/tcp. */
#undef ACE_TLI_TCP_DEVICE

/* The OS/platform supports the poll() event demultiplexor */
#undef ACE_USE_POLL

/* Platform has broken poll() */
#undef ACE_POLL_IS_BROKEN

/* Platform uses assembly symbols instead of C symbols in dlsym() */
#undef ACE_USES_ASM_SYMBOL_IN_DLSYM

/* When linking MFC as a static library is desired */
#undef ACE_USES_STATIC_MFC

/* Platform has its standard c++ library in the namespace std. */
#undef ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB

/* A parameter list indicating the version of WinSock (e.g., "1, 1" is
   version 1.1). */
#undef ACE_WSOCK_VERSION



/*   @BOTTOM@   */
/* All lines below "@BOTTOM@" will be placed at the bottom of config.h.in. */


#endif /* ACE_CONFIG_H */
