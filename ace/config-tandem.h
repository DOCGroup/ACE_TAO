/* -*- C++ -*- */
// Testing TANDEM
// $Id: 

// The following configuration file is designed to work for Tandems NonStop-UX
// 4.2MP  platforms using the NCC 3.20 compiler.

// Note this is a test version it might include several errors I
// have done a test and set/unset until I errors disappered.
// Some of the options that should be set aren't because of the simple fact
// that i haven't the time to check what is wrong.
// e.g. widecharacter are supported but a wcstok which only take 2 parameters
// are included by the compiler, to get the correct wcstok that takes 3 params
// we must set _XOPEN_SOURCE and we get ALOT of errors and warnings.
// So this config is done to get things to start to work it isn't finished.
// Janne (Jan.Perman@osd.Ericsson.se)

#if !defined (ACE_CONFIG_H)
#define ACE_CONFIG_H

#if ! defined (__ACE_INLINE__)
#define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

// Optimize ACE_Handle_Set for select().
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

// Tandem doesn't include this although they are defined 
// in sys/time.h and sys/resource.h
#define ACE_LACKS_RLIMIT_PROTOTYPE // jjpp
// Tandem has a function to set t_errno (set_t_errno)
#define ACE_HAS_SET_T_ERRNO         // jjpp

// Compiler/platform supports the "long long" datatype.
#define ACE_HAS_LONGLONG_T

// It does but problems with wcstok
// ACE_HAS_UNICODE			Platform/compiler supports UNICODE

//Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC

// OS/compiler omits the const from the sendmsg() prototype.
#define ACE_HAS_BROKEN_SENDMSG

//OS/compiler's header files are inconsistent with libC definition of rand_r().
//#define ACE_HAS_BROKEN_RANDR		// Defines it the same way as sunos5.4

//Platform supports system configuration information
#define ACE_HAS_SYSINFO

//Platform supports the POSIX regular expression library
#define ACE_HAS_REGEX

// Platform supports recvmsg and sendmsg
#define ACE_HAS_MSG

//Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYSCALL_H

//Platform provides <sysent.h> header
#define ACE_HAS_SYSENT_H

// Platform has terminal ioctl flags like TCGETS and TCSETS.
#define ACE_HAS_TERM_IOCTLS

// ? ACE_HAS_AUTOMATIC_INIT_FINI	Compiler/platform correctly calls 
//					init()/fini() for shared libraries

//Platform supports POSIX O_NONBLOCK semantics
#define ACE_HAS_POSIX_NONBLOCK

// Compiler/platform has correctly prototyped header files
#define ACE_HAS_CPLUSPLUS_HEADERS

// Platform supports IP multicast
//#define ACE_HAS_IP_MULTICAST            

//Compiler/platform supports alloca()
#define ACE_HAS_ALLOCA
//Compiler/platform has <alloca.h>
#define ACE_HAS_ALLOCA_H

//Platform contains <poll.h>
#define ACE_HAS_POLL

// Platform supports the POSIX struct timespec type
#define ACE_HAS_POSIX_TIME   // As i understand it, but i'm in deep water
//Platform supports the SVR4 timestruc_t type
#define ACE_HAS_SVR4_TIME

//ACE_HAS_PROC_FS			Platform supports the /proc file 
// system and defines tid_t in <sys/procfs.h>
// Has /proc but no tid_t

//ACE_HAS_PRUSAGE_T			Platform supports the prusage_t struct


// To get this to work a patch in sys/signal must be made
// typedef void          SIG_FUNC_TYPE(int);
//#if defined (__cplusplus)
//       void (*sa_handler)(int);
//#else
// ...
//#endif
//#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES
#define ACE_HAS_TANDEM_SIGNALS
//Compiler/platform defines the sig_atomic_t typedef
#define ACE_HAS_SIG_ATOMIC_T			
//Platform supports SVR4 extended signals
#define ACE_HAS_SIGINFO_T
//Platform supports ucontext_t (which is used in the extended signal API).
#define ACE_HAS_UCONTEXT_T 
//Compiler/platform supports SVR4 signal typedef
//#define ACE_HAS_SVR4_SIGNAL_T
//ACE_HAS_SVR4_SIGNAL_T


// Platform/compiler has the sigwait(2) prototype
#define ACE_HAS_SIGWAIT
//sigwait() takes only one argument.
#define ACE_HAS_ONEARG_SIGWAIT


//Compiler/platform provides the sockio.h file
#define ACE_HAS_SOCKIO_H

// Compiler supports the ssize_t typedef
#define ACE_HAS_SSIZE_T    // Limits.h must be included

//Platform supports STREAMS
#define ACE_HAS_STREAMS

#define ACE_HAS_STREAM_PIPES
//Platform supports STREAM pipes

//Compiler/platform supports strerror ()
#define ACE_HAS_STRERROR

//Compiler/platform supports struct strbuf
#define ACE_HAS_STRBUF_T

//Compiler/platform supports SVR4 dynamic linking semantics
#define ACE_HAS_SVR4_DYNAMIC_LINKING
//Compiler/platform supports SVR4  gettimeofday() prototype
//#define ACE_HAS_SVR4_GETTIMEOFDAY       // Defines it the same way as sunos5.4

//Compiler/platform supports SVR4 TLI (in particular, T_GETNAME stuff)...
#define ACE_HAS_SVR4_TLI

//Platform provides <sys/filio.h> header
#define ACE_HAS_SYS_FILIO_H

//Platform supports TLI timod STREAMS module
#define ACE_HAS_TIMOD_H
//Platform supports TLI tiuser header
#define ACE_HAS_TIUSER_H

//Platform supports TLI
#define ACE_HAS_TLI
//Platform provides TLI function prototypes
#define ACE_HAS_TLI_PROTOTYPES

//Platform lacks streambuf  "linebuffered ()".
#define ACE_LACKS_LINEBUFFERED_STREAMBUF

// Platform lacks "signed char" type (broken!)
#define ACE_LACKS_SIGNED_CHAR


//? ACE_NEEDS_DEV_IO_CONVERSION		Necessary with some compilers
//to pass ACE_TTY_IO as parameter to DEV_Connector.

#define ACE_PAGE_SIZE 4096
// Defines the page size of the system (not used on Win32 or
// with ACE_HAS_GETPAGESIZE).

/****** THREAD SPECIFIC **********/
/* If you want to remove threading then comment out the following four #defines .*/
#if !defined (ACE_MT_SAFE)
	#define ACE_MT_SAFE 1		//Compile using multi-thread libraries
#endif
#define ACE_HAS_THREADS			//Platform supports threads
#define ACE_HAS_STHREADS		//Platform supports Solaris threads

// Compiler/platform has threadspecific storage
#define ACE_HAS_THREAD_SPECIFIC_STORAGE   
//Platform supports thr_keydelete  (e.g,. UNIXWARE)

#define ACE_HAS_THR_MINSTACK            // Tandem uses thr_minstack instead of thr_min_stack
#define ACE_LACKS_PRI_T                 // Tandem lacks pri_t
#define ACE_HAS_THR_KEYDELETE

#define ACE_HAS_MT_SAFE_SOCKETS		// Sockets may be called in multi-threaded programs

//ACE_HAS_REENTRANT_FUNCTIONS 		Platform supports reentrant 
// functions (i.e., all the POSIX *_r functions).
//ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS	Platform will recurse infinitely on 
// thread exits from TSS cleanup routines (e.g., AIX).  
//ACE_NEEDS_HUGE_THREAD_STACKSIZE	Required by platforms with small 
// default stacks.
//ACE_HAS_ONLY_SCHED_OTHER                Platform, e.g., Solaris 2.5, only 
// supports SCHED_OTHER POSIX scheduling policy.


//*************************************
//ACE_HAS_PTHREADS			Platform supports POSIX Pthreads
//ACE_HAS_PTHREAD_CONDATTR_SETKIND_NP   Platform has pthread_condattr_
// setkind_np().
//ACE_HAS_PTHREAD_DSTATE_PTR		pthread_attr_setdetachstate() takes 
// pointer to 2nd arg.
//ACE_HAS_PTHREAD_EQUAL			Platform has pthread_equal().
//ACE_HAS_PTHREAD_GETSPECIFIC_DATAPTR	pthread_getspecific() takes a data 
// pointer for 2nd arg.
//ACE_HAS_PTHREAD_MUTEXATTR_SETKIND_NP	Platform has 
// pthread_mutexattr_setkind_np().
//ACE_HAS_PTHREAD_T			Platform has pthread_t defined.
//ACE_HAS_PTHREAD_YIELD_VOIDPTR		pthread_yield() takes a void pointer 
// arg.
//ACE_HAS_THREAD_SELF			Platform has thread_self() rather 
// than pthread_self() (e.g., DCETHREADS and AIX)
//ACE_HAS_THR_C_DEST			The pthread_keycreate() routine *
// must* take extern C functions.
//ACE_HAS_THR_C_FUNC			The pthread_create() routine *must* 
// take extern C functions.
//ACE_HAS_YIELD_VOID_PTR		Platform requires pthread_yield() to 
// take a NULL.
//ACE_LACKS_CONDATTR_PSHARED		Platform has no implementation of 
// pthread_condattr_setpshared(), even though it supports pthreads!
//ACE_LACKS_PTHREAD_THR_SIGSETMASK	Platform lacks pthread_thr_
// sigsetmask (e.g., MVS, HP/UX, and OSF/1 3.2)
//ACE_LACKS_SETDETACH			Platform lacks pthread_attr_
// setdetachstate() (e.g., HP/UX 10.x)
//ACE_LACKS_SETSCHED			Platform lacks pthread_attr_
// setsched() (e.g. MVS)
//ACE_LACKS_THREAD_STACK_SIZE		Platform lacks pthread_attr_
// setstacksize() (e.g., Linux pthreads)

//ACE_HAS_IRIX62_THREADS		Platform supports the very odd IRIX 
// 6.2 threads...

/*********************************/

/******* SIGNAL STUFF *******/
//ACE_HAS_SIGNAL_OBJECT_AND_WAIT	Platform supports the Win32 
// SignalObjectAndWait() function (WinNT 4.0 and beyond).
//#define ACE_HAS_SIGNAL_SAFE_OS_CALLS
//Automatically restart OS system 
// calls when EINTR occurs

// 10 millisecond fudge factor to account for Solaris timers...
//#if !defined (ACE_TIMER_SKEW)
//#define ACE_TIMER_SKEW 1000 * 10
//#endif /* ACE_TIMER_SKEW */

// Platform supports the getrusage() system call.
//#define ACE_HAS_GETRUSAGE
//Platform uses non-const char * in calls to gethostbyaddr, gethostbyname,
// getservbyname
#define ACE_HAS_NONCONST_GETBY
// Platform's select() uses non-const timeval* (only found on Linux right now)
#define ACE_HAS_NONCONST_SELECT_TIMEVAL
// And on Tandem :-)
//Uses ctime_r & asctime_r with only two parameters vs. three.
#define ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R
//Platform has special header for select().
#define ACE_HAS_SELECT_H
// Platform/compiler supports Standard C++ Library
#define ACE_HAS_STANDARD_CPP_LIBRARY
//Platform/compiler support the System V sprintf().
//?#define ACE_HAS_SYSV_SPRINTF
//Platform/compiler supports _sys_errlist symbol
//#define ACE_HAS_SYS_ERRLIST
//Platform lacks madvise() (e.g., Linux)
#define ACE_LACKS_MADVISE
//Platform lacks the si_addr field of siginfo_t (e.g.,VxWorks and HP/UX 10.x)
//?#define ACE_LACKS_SI_ADDR
//Compiler/platform lacks strcasecmp() (e.g., DG/UX, UNIXWARE, VXWORKS)
#define ACE_LACKS_STRCASECMP
//<time.h> doesn't automatically #include /**/ <sys/time.h>
#define ACE_NEEDS_SYSTIME_H

//ACE_HAS_64BIT_LONGS	Platform has 64bit longs and 32bit ints...
//ACE_HAS_AIX_BROKEN_SOCKET_HEADER	Platform, such as AIX4, needs to wrap
//                #include of sys/socket.h with #undef/#define of __cplusplus.
//ACE_HAS_AIX_HI_RES_TIMER		Platform has AIX4 ::read_real_time ()
// ? ACE_HAS_BROKEN_BITSHIFT                 Compiler has integer overflow 
//					problem with bit-shift operations.
// ? ACE_HAS_BROKEN_CONVERSIONS		Compiler can't handle calls like 
//					foo->operator T *()
//ACE_HAS_BROKEN_CTIME			Compiler/platform uses macro for ctime
// (e.g., MVS)
//ACE_HAS_BROKEN_HPUX_TEMPLATES		Earlier versions of HP/UX C++ are 
// damned...
//ACE_HAS_BROKEN_MSG_H                 Platform headers don't support <msg.h>
// prototypes
//ACE_HAS_BROKEN_MMAP_H		HP/UX does not wrap the mmap(2) header
// files with extern "C".
//ACE_HAS_BROKEN_POSIX_TIME             Platform defines struct timespec in
// <sys/timers.h>
//ACE_HAS_BROKEN_SETRLIMIT             OS/compiler omits the const from the
// rlimit parameter in the setrlimit() prototype.
//ACE_HAS_BROKEN_WRITEV                OS/compiler omits the const from the
// iovec parameter in the writev() prototype.
// There is a bstring in the Tandem but where man bstring OK, find bstring NOK
// ? ACE_HAS_BSTRING			 Platform has <bstring.h> (which contains bzero() prototype)
//ACE_HAS_CANCEL_IO			Platform supports the Win32 CancelIO()
// function (WinNT 4.0 and beyond).
//ACE_HAS_CHARPTR_DL			OS/platform uses char * for 
// dlopen/dlsym args, rather than const char *.
// Yes in man, no in header file
//ACE_HAS_CHARPTR_SOCKOPT              OS/platform uses char * for sockopt, 
// rather than const char *
// sprintf() returns char * rather than int (e.g., SunOS 4.x)
//#define ACE_HAS_CHARPTR_SPRINTF
//ACE_HAS_CLOCK_GETTIME			Platform supports POSIX 1.b
// clock_gettime ()
//ACE_HAS_COMPLEX_LOCK			Platform supports non-standard
// readers/writer locks...
//? ACE_HAS_EXCEPTIONS			Compiler supports C++ exception 
// handling
// Platform supports getpagesize() call (otherwise, ACE_PAGE_SIZE must be
// defined, except on Win32)
//#define ACE_HAS_GETPAGESIZE		// Man getpagesize ok grep /usr/.. not
// found
//ACE_HAS_GNU_CSTRING_H		Denotes that GNU has cstring.h as
// standard which redefines memchr() 
//ACE_HAS_HI_RES_TIMER			Compiler/platform supports SunOS 
// high resolution timers
//ACE_HAS_INLINED_OSCALLS		Inline all the static class OS 
// methods to remove call overhead
//ACE_HAS_IRIX_GETTIMEOFDAY		Denotes that IRIX 5.3 has second 
// argument to gettimeofday() which is variable ... 
//ACE_HAS_LIMITED_RUSAGE_T		The rusage_t structure has only two 
// fields.
//ACE_HAS_LONG_MAP_FAILED                 Platform defines MAP_FAILED as a 
// long constant.
//? ACE_HAS_NONCONST_MSGSND		Platform has a non-const parameter to
// msgsend() (e.g., SCO).
// Has it msgsend at all

//ACE_HAS_OLD_MALLOC			Compiler/platform uses old malloc()/
// free() prototypes (ugh)
#if !defined (ACE_HAS_ORBIX)
	#define ACE_HAS_ORBIX 0
#endif
// ACE_HAS_ORBIX			Platform has Orbix CORBA implementation
//? ACE_HAS_POSIX_SEM			Platform supports POSIX real-time 
//semaphores (e.g., VxWorks and Solaris)
//Compiler/platform defines a union semun for SysV shared memory
//#define ACE_HAS_SEMUN
//ACE_HAS_SIN_LEN			Platform supports new BSD inet_addr 
// len field.
//? ACE_HAS_SIZET_SOCKET_LEN		OS/compiler uses size_t * rather 
//than int * for socket lengths
//ACE_HAS_SOCKADDR_MSG_NAME		Platform requires (struct sockaddr *)
//for msg_name field of struct msghdr.
//ACE_HAS_STRICT		Use the STRICT compilation mode on Win32.
//? ACE_HAS_STRUCT_NETDB_DATA		Compiler/platform has strange 
// hostent API for socket *_r() calls
//? ACE_HAS_STRUCT_PROTOENT_DATA	Compiler/platform has strange 
//protoent API for socket *_r() calls
//ACE_HAS_SUNOS4_GETTIMEOFDAY		SunOS 4 style prototype.
//? ACE_HAS_SYSCALL_GETRUSAGE		HP/UX has an undefined syscall for 
//GETRUSAGE...
//ACE_HAS_TEMPLATE_TYPEDEFS		Compiler implements templates that 
// support typedefs inside of classes used as formal arguments to a template
// class. 
//ACE_HAS_TID_T				Platform supports the tid_t type 
//(e.g., AIX)
//Platform/compiler supports timezone * as second parameter to gettimeofday()
//#define ACE_HAS_TIMEZONE_GETTIMEOFDAY  According to man we have but not 
//when compiling
//ACE_HAS_UNION_WAIT			The wait() system call takes a (
// union wait *) rather than int *
//? ACE_HAS_USING_KEYWORD		Compiler supports the new using 
// keyword for C++ namespaces.
//ACE_HAS_VOIDPTR_MMAP			Platform requires void * for mmap().
//ACE_HAS_VOIDPTR_SOCKOPT		OS/compiler uses void * arg 4 
// setsockopt() rather than const char *
//ACE_HAS_WIN32_TRYLOCK			The Win32 platform support 
//TryEnterCriticalSection() (WinNT 4.0 and beyond)
//ACE_HAS_WINSOCK2			The Win32 platform supports WinSock 2.0
//ACE_HAS_XLI 				Platform has the XLI version of TLI
//ACE_HAS_XT 				Platform has Xt and Motif
//ACE_LACKS_CONST_TIMESPEC_PTR		Platform forgot const in cond_timewait
// (e.g., HP/UX).
//ACE_LACKS_COND_T			Platform lacks condition variables 
// (e.g., Win32 and VxWorks)
//ACE_LACKS_EXEC			Platform lacks the exec() family of 
// system calls (e.g., Win32, VxWorks, Chorus)
//ACE_LACKS_FILELOCKS			Platform lacks file locking mechanism
//ACE_LACKS_IOSTREAM_FX			iostream header does not declare 
// ipfx (), opfx (), etc.
//ACE_LACKS_MALLOC_H			Platform lacks malloc.h
//ACE_LACKS_MEMORY_H			Platform lacks memory.h 
//(e.g., VxWorks and Chorus)
//ACE_LACKS_MKTEMP			ACE has no mktemp()
//ACE_LACKS_MMAP			The platform doesn't have mmap(2) 
// (e.g., SCO UNIX).
//ACE_LACKS_MODE_MASKS			Platform/compiler doesn't have 
//open() mode masks.
//ACE_LACKS_MPROTECT			The platform doesn't have mprotect(2) 
//(e.g., EPLX real time OS from CDC (based on LYNX))
//ACE_LACKS_MSGBUF_T			Platform lacks struct msgbuf (e.g., 
// NT and MSV).
//ACE_LACKS_MSYNC				Platform lacks msync() (e.g., Linux)
//ACE_LACKS_NETDB_REENTRANT_FUNCTIONS	Platform does not support reentrant 
// netdb functions (getprotobyname_r, getprotobynumber_r, gethostbyaddr_r, 
// gethostbyname_r, getservbyname_r).
//ACE_LACKS_RPC_H			Platform lacks the ONC RPC header 
// files.  
//ACE_LACKS_PARAM_H			Platform lacks <sys/param.h> (e.g., 
//MVS)
//ACE_LACKS_POSIX_PROTOTYPES			Platform lacks POSIX prototypes for 
//certain System V functions like shared memory and message queues.
//ACE_LACKS_RECVMSG			Platform lacks recvmsg() (e.g., Linux)
//ACE_LACKS_RWLOCK_T			Platform lacks readers/writer locks.
//ACE_LACKS_SBRK			Platform lacks a working sbrk() 
// (e.g., Win32 and VxWorks)
//ACE_LACKS_SEMBUF_T			Platform lacks struct sembuf 
//(e.g., Win32 and VxWorks)
//ACE_LACKS_SIGACTION			Platform lacks struct sigaction 
// (e.g., Win32 and Chorus)
//? ACE_LACKS_SYS_NERR			Platforms/compiler lacks the sys_nerr
// variable (e.g., VxWorks and MVS).
//ACE_LACKS_SYSV_MSG_H			Platform lacks sys/msg.h 
//(e.g., Chorus and VxWorks)
//ACE_LACKS_KEYDELETE			Platform lacks TSS keydelete 
//(e.g., HP/UX)
//ACE_LACKS_KEY_T			Platform lacks key_t 
//(e.g., Chorus, VxWorks, Win32)
//ACE_LACKS_SENDMSG			Platform lacks sendmsg() 
// (e.g., Linux)
//ACE_LACKS_SYSV_SHMEM			Platform lacks System V shared 
// memory (e.g., Win32 and VxWorks)
//ACE_LACKS_SIGINFO_H			Platform lacks the siginfo.h include 
// file (e.g., MVS)
//ACE_LACKS_SOCKETPAIR			Platform lacks the socketpair() 
// call (e.g., SCO UNIX)
//? ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES	Compiler doesn't support 
// static data member templates
//ACE_LACKS_STRRECVFD			Platform doesn't define struct 
//strrecvfd.
//ACE_LACKS_SYSCALL			Platform doesn't have syscall() 
// prototype
//ACE_LACKS_SYSV_MSQ_PROTOS		Platform doesn't have prototypes for 
//Sys V msg()* queues.
//ACE_LACKS_T_ERRNO			Header files lack t_errno for TLI
//ACE_LACKS_TCP_H			Platform doesn't have netinet/tcp.h
//ACE_LACKS_UCONTEXT_H			Platform lacks the ucontext.h file
//ACE_LACKS_UNIX_DOMAIN_SOCKETS		ACE platform has no UNIX domain sockets
//ACE_LACKS_UTSNAME_T			Platform lacks struct utsname
// (e.g., Win32 and VxWorks)
//ACE_NDEBUG				Turns off debugging features
//ACE_NEEDS_READV			Platform doesn't define readv, so 
// use our own
//ACE_NEEDS_WRITEV			Platform doesn't define writev, so 
//use our own
//ACE_NLOGGING				Turns off the LM_DEBUG and LM_ERROR 
//logging macros...
//ACE_NTRACE				Turns off the tracing feature.
//ACE_REDEFINES_XTI_FUNCTIONS		Platform redefines the t_... names 
//(UnixWare)
//ACE_SELECT_USES_INT			Platform uses int for select() 
//rather than fd_set
//? ACE_TEMPLATES_REQUIRE_PRAGMA	Compiler's template mechanism must 
// use a pragma This is used for AIX's C++ compiler.
//? ACE_TEMPLATES_REQUIRE_SOURCE	Compiler's template mechanim must 
// see source code (i.e., .cpp files).  This is used for GNU G++.
//? ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION    Compiler's template mechanism 
//requires the use of explicit C++ specializations for all used 
//templates. This is also used for GNU G++ if you don't use the "repo" 
//patches.
//ACE_USE_POLL				Use the poll() event demultiplexor 
//rather than select().



// Turns off the tracing feature.
// #define ACE_NTRACE 0
#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */

// Defines the page size of the system.
#define ACE_PAGE_SIZE 4096

#endif /* ACE_CONFIG_H */
