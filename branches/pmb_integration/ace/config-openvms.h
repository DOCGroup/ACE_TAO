/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for OpenVMS 7.3-2

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H

#pragma message disable CODCAUUNR
#pragma message disable CODEUNREACHABLE
//#pragma message disable DOLLARID
//#pragma message disable NOSIMPINT
//#pragma message disable NOSTDLONGLONG
#pragma message disable NARROWPTR
//#pragma message disable LONGEXTERN
#pragma message disable UNSCOMZER

//#define MAXPATHLEN 255
//#define ERRMAX sys_nerr
//#define howmany __howmany
//#define MAXHOSTNAMELEN 255
//#define MAXNAMLEN FILENAME_MAX
//#define __unix__

// OpenVMS does not support symlinks and does not define this.
// Defining this as 0 effectively cancels out the one bit of code using this in
// OS_NS_stdlib.cpp.
#define MAXSYMLINKS 0

#define ACE_OPENVMS
#define ACE_DLL_SUFFIX ACE_LIB_TEXT("")

#define ACE_HAS_DUMP    1

//#define ACE_NEEDS_PTHREAD_UPPERCASE 1

/*
#define XtAppAddInput XTAPPADDINPUT
#define XtAppAddTimeOut XTAPPADDTIMEOUT
#define XtAppProcessEvent XTAPPPROCESSEVENT
#define XtRemoveInput XTREMOVEINPUT
#define XtRemoveTimeOut XTREMOVETIMEOUT
*/

// need this includes to ensure proper sequence of definitions so that
// f.i. HP C/C++ does not '#define ' memcpy, memmove etc.
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#undef clearerr

#define ACE_DEFAULT_BASE_ADDR ((char*)(0x30000000))

#define ACE_HAS_STDCPP_STL_INCLUDES 1

#define ACE_LACKS_STDINT_H  1
#define ACE_LACKS_SYS_IPC_H 1
#define ACE_LACKS_SYS_SEM_H 1
#define ACE_LACKS_SEMAPHORE_H 1
#define ACE_LACKS_SYS_SELECT_H 1
#define ACE_LACKS_TERMIOS_H 1
#define ACE_LACKS_SYS_SHM_H 1
#define ACE_LACKS_SYS_MSG_H 1
#define ACE_LACKS_REGEX_H 1
#define ACE_LACKS_SEARCH_H 1

#define ACE_LACKS_SETPGID 1
#define ACE_LACKS_SETREUID 1
#define ACE_LACKS_SETREGID 1
#define ACE_LACKS_FORK 1
#define ACE_LACKS_GETPGID 1
#define ACE_LACKS_SETSID 1
#define ACE_LACKS_FCNTL 1

#define ACE_LACKS_REALPATH 1

#define ACE_LACKS_TIMESPEC_T    1
#define ACE_LACKS_CONST_TIMESPEC_PTR 1

#define ACE_LACKS_RWLOCK_T 1
//#define ACE_HAS_PTHREADS_UNIX98_EXT 1
//#define ACE_LACKS_MUTEXATTR_PSHARED 1
//#define ACE_LACKS_CONDATTR_PSHARED 1
#define ACE_LACKS_PTHREAD_KILL 1
#define ACE_LACKS_THREAD_PROCESS_SCOPING 1

#define ACE_LACKS_LINEBUFFERED_STREAMBUF 1

#define ACE_LACKS_PWD_REENTRANT_FUNCTIONS 1
#define ACE_LACKS_RAND_REENTRANT_FUNCTIONS 1

#define ACE_LACKS_UNIX_SIGNALS 1

#define ACE_MT_SAFE 1
                                        /*Compile using multi-thread libraries*/
#define ACE_NEW_THROWS_EXCEPTIONS 1
                                        /*Compiler's 'new' throws exception on
                                        failure (ANSI C++ behavior).*/
#define ACE_TEMPLATES_REQUIRE_SOURCE 1
                                        /*Compiler's template mechanim
                                        must see source code (i.e.,
                                        .cpp files).  This is used for
                                        GNU G++.*/
#define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 1
                                        /*Platform has its standard c++
                                        library in the namespace std.*/
#define ACE_HAS_VOIDPTR_GETTIMEOFDAY 1
                                        /* 2nd arg 'void*' instead of 'struct timezone*' */
#define ACE_HAS_SNPRINTF 1
                                        /*Platform offers snprintf().*/
#define ACE_HAS_P_READ_WRITE
                                        /*Platform has pread() and
                                        pwrite() support*/
#define ACE_HAS_AUTOMATIC_INIT_FINI 1
                                        /*Compiler/platform correctly
                                        calls init()/fini() for shared
                                        libraries*/
#define ACE_HAS_CHARPTR_DL 1
                                        /*OS/platform uses char * for
                                        dlopen/dlsym args, rather than
                                        const char *.*/
#define ACE_HAS_CLOCK_GETTIME 1
                                        /*Platform supports POSIX 1.b
                                        clock_gettime ()*/
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES 1
                                        /*Prototypes for both signal()
                                        and struct sigaction are
                                        consistent.*/
#define ACE_HAS_CPLUSPLUS_HEADERS 1
                                        /*Compiler/platform has
                                        correctly prototyped header
                                        files*/
#define ACE_HAS_DIRENT 1
                                        /*Compiler/platform has Dirent
                                        iterator functions*/
#define ACE_HAS_EXCEPTIONS 1
                                        /*Compiler supports C++
                                        exception handling*/
#define ACE_HAS_GETPAGESIZE 1
                                        /*Platform supports
                                        getpagesize() call (otherwise,
                                        ACE_PAGE_SIZE must be defined,
                                        except on Win32)*/
#define ACE_HAS_GPERF 1
                                        /*The GPERF utility is compiled
                                        for this platform*/
#define ACE_HAS_IDTYPE_T 1
                                        /*Compiler/platform supports
                                        idtype_t.*/
#define ACE_HAS_IP_MULTICAST 1
                                        /*Platform supports IP multicast*/
#define ACE_HAS_MEMCHR 1
                                        /*Use native implementation of memchr.*/
#define ACE_HAS_MSG
                                        /*Platform supports recvmsg and
                                        sendmsg*/
#define ACE_HAS_NONCONST_SELECT_TIMEVAL 1
                                        /*Platform's select() uses
                                        non-const timeval* (only found
                                        on Linux right now)*/
#define ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R 1
                                        /*Uses ctime_r & asctime_r with
                                        only two parameters
                                        vs. three.*/
#define ACE_HAS_3_PARAM_WCSTOK 1
                                        /*Platform has 3-parameter version
                                        of wcstok(), which was added in
                                        1994 in the ISO C standard Normative
                                        Addendum 1.  Other standards like XPG4
                                        define a 2 parameter wcstok().*/
#define ACE_HAS_POSIX_NONBLOCK 1
                                        /*Platform supports POSIX
                                        O_NONBLOCK semantics*/
#define ACE_HAS_POSIX_TIME 1
                                        /*Platform supports the POSIX
                                        struct timespec type*/
#define ACE_HAS_PTHREADS 1
                                        /*Platform supports POSIX
                                        Pthreads, of one form or
                                        another.  This macro says the
                                        platform has a pthreads
                                        variety - should also define
                                        one of the below to say which
                                        one.  Also may need some
                                        ACE_HAS_... thing for
                                        extensions.*/
#define ACE_HAS_PTHREADS_STD 1
                                        /*Platform supports POSIX.1c-1995 threads
                                        (This is the final standard
                                        Pthreads).*/
#define ACE_HAS_PTHREAD_PROCESS_ENUM 1
                                        /*pthread.h declares an enum with
                                        PTHREAD_PROCESS_PRIVATE and
                                        PTHREAD_PROCESS_SHARED values.*/
#define ACE_HAS_XPG4_MULTIBYTE_CHAR 1
                                        /*Platform has support for
                                        multi-byte character support
                                        compliant with the XPG4
                                        Worldwide Portability
                                        Interface wide-character
                                        classification.*/
#define ACE_HAS_SIGINFO_T 1
                                        /*Platform supports SVR4
                                        extended signals*/
#define ACE_HAS_SIGSUSPEND 1
                                        /*Platform supports sigsuspend()*/
#define ACE_HAS_SIG_MACROS 1
                                        /*Platform/compiler has macros
                                        for sig{empty,fill,add,del}set
                                        (e.g., SCO and FreeBSD)*/
#define ACE_HAS_SIGWAIT 1
                                        /*Platform/compiler has the
                                        sigwait(2) prototype*/
#define ACE_HAS_SIGTIMEDWAIT 1

#define ACE_HAS_SIG_C_FUNC 1
                                        /*Compiler requires extern "C"
                                        functions for signals.*/
#define ACE_HAS_SIZET_SOCKET_LEN 1
                                        /*OS/compiler uses size_t *
                                        rather than int * for socket
                                        lengths*/
#define ACE_HAS_SSIZE_T 1
                                        /*Compiler supports the ssize_t
                                        typedef*/
#define ACE_HAS_STRPTIME 1
                                        /*Enables ACE_OS::strptime ().*/
#define ACE_HAS_STANDARD_CPP_LIBRARY 1
                                        /*Platform/compiler supports
                                        Standard C++ Library*/
#define ACE_HAS_STRNLEN 1
                                        /*Platform supports strnlen(3).*/
#define ACE_HAS_STREAMS 1
                                        /*Platform supports STREAMS*/
#define ACE_HAS_STRERROR 1
                                        /*Compiler/platform supports strerror ()*/
#define ACE_HAS_STRING_CLASS 1
                                        /*Platform/Compiler supports a
                                        String class (e.g., GNU or
                                        Win32).*/
#define ACE_HAS_SVR4_DYNAMIC_LINKING 1
                                        /*Compiler/platform supports
                                        SVR4 dynamic linking semantics*/
#define ACE_HAS_TEMPLATE_SPECIALIZATION 1
                                        /*Compiler implements template
                                        specialization*/

#define ACE_HAS_STD_TEMPLATE_CLASS_MEMBER_SPECIALIZATION 1

#define ACE_HAS_TEMPLATE_TYPEDEFS 1
                                        /*Compiler implements templates
                                        that support typedefs inside
                                        of classes used as formal
                                        arguments to a template
                                        class.*/
#define ACE_HAS_THREADS 1
                                        /*Platform supports threads*/
#define ACE_HAS_THREAD_SPECIFIC_STORAGE 1
                                        /*Compiler/platform has
                                        thread-specific storage*/
#define ACE_HAS_THR_C_DEST 1
                                        /*The pthread_keycreate()
                                        routine *must* take extern C
                                        functions.*/
#define ACE_HAS_THR_C_FUNC 1
                                        /*The pthread_create() routine
                                        *must* take extern C
                                        functions.*/
#define ACE_HAS_UALARM 1
                                        /*Platform supports ualarm()*/
#define ACE_HAS_USING_KEYWORD 1
                                        /*Compiler supports the new
                                        using keyword for C++
                                        namespaces.*/
#define ACE_HAS_VOIDPTR_MMAP 1
                                        /*Platform requires void * for
                                        mmap().*/
#define ACE_HAS_VOIDPTR_SOCKOPT 1
                                        /*OS/compiler uses void * arg 4
                                        setsockopt() rather than const
                                        char **/
#define ACE_LACKS_LSTAT 1
                                        /*Platform lacks the lstat() function.*/
#define ACE_LACKS_MADVISE 1
                                        /*Platform lacks madvise()
                                        (e.g., Linux)*/
#define ACE_LACKS_MALLOC_H 1
                                        /*Platform lacks malloc.h*/
#define ACE_LACKS_MKFIFO 1
                                        /*Platform lacks mkfifo() e.g.,
                                        VxWorks, Chorus, pSoS, and WinNT.*/
#define ACE_LACKS_NAMED_POSIX_SEM 1
                                        /*Platform lacks named POSIX
                                        semaphores (e.g., Chorus)*/
#define ACE_LACKS_NETDB_REENTRANT_FUNCTIONS 1
                                        /*Platform does not support
                                        reentrant netdb functions
                                        (getprotobyname_r,
                                        getprotobynumber_r,
                                        gethostbyaddr_r,
                                        gethostbyname_r,
                                        getservbyname_r).*/
#define ACE_LACKS_SYS_PARAM_H 1
                                        /*Platform lacks <sys/param.h>
                                        (e.g., MVS)*/
#define ACE_LACKS_PRI_T 1
                                        /*Platform lacks pri_t (e.g.,
                                        Tandem NonStop UNIX).*/
#define ACE_LACKS_PTHREAD_SIGMASK 1
                                        /*Platform lacks pthread_sigmask ().*/
#define ACE_LACKS_PTHREAD_THR_SIGSETMASK 1
                                        /*Platform lacks
                                        pthread_thr_sigsetmask (e.g.,
                                        MVS, HP/UX, and OSF/1 3.2)*/
#define ACE_LACKS_READLINK 1
                                        /*Platform lacks the readlink() function.*/
#define ACE_LACKS_RLIMIT 1
                                        /*Platform/compiler lacks
                                        {get,set}rlimit() function
                                        (e.g., VxWorks, Chorus, and
                                        SCO UNIX)*/
#define ACE_LACKS_RLIMIT_PROTOTYPE 1
                                        /*Platform/compiler lacks
                                        {get,set}rlimit() prototypes
                                        (e.g., Tandem)*/
#define ACE_LACKS_SEMBUF_T 1
                                        /*Platform lacks struct sembuf
                                        (e.g., Win32 and VxWorks)*/
#define ACE_LACKS_SETSCHED
                                        /*Platform lacks
                                        pthread_attr_setsched()
                                        (e.g. MVS)*/
#define ACE_LACKS_SYSV_SHMEM 1
                                        /*Platform lacks System V shared
                                        memory (e.g., Win32 and
                                        VxWorks)*/
#define ACE_LACKS_SIGINFO_H 1
                                        /*Platform lacks the siginfo.h
                                        include file (e.g., MVS)*/
#define ACE_LACKS_SOCKETPAIR 1
                                        /*Platform lacks the
                                        socketpair() call (e.g., SCO
                                        UNIX)*/
#define ACE_LACKS_STRRECVFD 1
                                        /*Platform doesn't define struct
                                        strrecvfd.*/
#define ACE_LACKS_SYSCALL 1
                                        /*Platform doesn't have
                                        syscall() prototype*/
#define ACE_LACKS_T_ERRNO 1
                                        /*Header files lack t_errno for
                                        TLI*/
#define ACE_LACKS_UCONTEXT_H 1
                                        /*Platform lacks the ucontext.h
                                        file*/
#define ACE_LACKS_UNIX_DOMAIN_SOCKETS 1
                                        /*ACE platform has no UNIX
                                        domain sockets*/
#define ACE_LACKS_UNIX_SYSLOG 1

#define ACE_NEEDS_HUGE_THREAD_STACKSIZE (64U*1024)
                                        /*Required by platforms with small default stacks.*/

#ifdef ACE_NEEDS_PTHREAD_UPPERCASE

/*
** These defines take care of the upper/lower casing problem that occurs
** if you are using pthreads and are compiling /NAMES=AS_IS (which is necessary for ACE/TAO)
**
** Note that it does NOT take care of any argument differences between
** the various implementations of pthreads.
*/

#define pthread_attr_create PTHREAD_ATTR_CREATE
#define pthread_attr_delete PTHREAD_ATTR_DELETE
#define pthread_attr_destroy PTHREAD_ATTR_DESTROY
#define pthread_attr_getdetach_np PTHREAD_ATTR_GETDETACH_NP
#define pthread_attr_getguardsize_np PTHREAD_ATTR_GETGUARDSIZE_NP
#define pthread_attr_getinheritsched PTHREAD_ATTR_GETINHERITSCHED
#define pthread_attr_getprio PTHREAD_ATTR_GETPRIO
#define pthread_attr_getsched PTHREAD_ATTR_GETSCHED
#define pthread_attr_getschedparam PTHREAD_ATTR_GETSCHEDPARAM
#define pthread_attr_getstacksize PTHREAD_ATTR_GETSTACKSIZE
#define pthread_attr_init PTHREAD_ATTR_INIT
#define pthread_attr_setdetach_np PTHREAD_ATTR_SETDETACH_NP
#define pthread_attr_setdetachstate PTHREAD_ATTR_SETDETACHSTATE
#define pthread_attr_setguardsize_np PTHREAD_ATTR_SETGUARDSIZE_NP
#define pthread_attr_setinheritsched PTHREAD_ATTR_SETINHERITSCHED
#define pthread_attr_setprio PTHREAD_ATTR_SETPRIO
#define pthread_attr_setsched PTHREAD_ATTR_SETSCHED
#define pthread_attr_setschedparam PTHREAD_ATTR_SETSCHEDPARAM
#define pthread_attr_setschedpolicy PTHREAD_ATTR_SETSCHEDPOLICY
#define pthread_attr_setstacksize PTHREAD_ATTR_SETSTACKSIZE
#define pthread_cancel PTHREAD_CANCEL
#define pthread_cancel_e PTHREAD_CANCEL_E
#define pthread_cond_broadcast PTHREAD_COND_BROADCAST
#define pthread_cond_destroy PTHREAD_COND_DESTROY
#define pthread_cond_init PTHREAD_COND_INIT
#define pthread_cond_sig_preempt_int_np PTHREAD_COND_SIG_PREEMPT_INT_NP
#define pthread_cond_signal PTHREAD_COND_SIGNAL
#define pthread_cond_signal_int_np PTHREAD_COND_SIGNAL_INT_NP
#define pthread_cond_timedwait PTHREAD_COND_TIMEDWAIT
#define pthread_cond_wait PTHREAD_COND_WAIT
#define pthread_condattr_create PTHREAD_CONDATTR_CREATE
#define pthread_condattr_delete PTHREAD_CONDATTR_DELETE
#define pthread_condattr_init PTHREAD_CONDATTR_INIT
#define pthread_create PTHREAD_CREATE
#define pthread_delay_np PTHREAD_DELAY_NP
#define pthread_detach PTHREAD_DETACH
#define pthread_equal PTHREAD_EQUAL
#define pthread_exc_fetch_fp_np PTHREAD_EXC_FETCH_FP_NP
#define pthread_exc_handler_np PTHREAD_EXC_HANDLER_NP
#define pthread_exc_pop_ctx_np PTHREAD_EXC_POP_CTX_NP
#define pthread_exc_push_ctx_np PTHREAD_EXC_PUSH_CTX_NP
#define pthread_exc_savecontext_np PTHREAD_EXC_SAVECONTEXT_NP
#define pthread_exit PTHREAD_EXIT
#define pthread_get_expiration_np PTHREAD_GET_EXPIRATION_NP
#define pthread_getprio PTHREAD_GETPRIO
#define pthread_getschedparam PTHREAD_GETSCHEDPARAM
#define pthread_getscheduler PTHREAD_GETSCHEDULER
#define pthread_getspecific PTHREAD_GETSPECIFIC
#define pthread_getunique_np PTHREAD_GETUNIQUE_NP
#define pthread_join PTHREAD_JOIN
#define pthread_join32 PTHREAD_JOIN32
#define pthread_keycreate PTHREAD_KEYCREATE
#define pthread_key_create PTHREAD_KEY_CREATE
#define pthread_kill PTHREAD_KILL
#define pthread_lock_global_np PTHREAD_LOCK_GLOBAL_NP
#define pthread_mutex_destroy PTHREAD_MUTEX_DESTROY
#define pthread_mutex_init PTHREAD_MUTEX_INIT
#define pthread_mutex_lock PTHREAD_MUTEX_LOCK
#define pthread_mutex_trylock PTHREAD_MUTEX_TRYLOCK
#define pthread_mutex_unlock PTHREAD_MUTEX_UNLOCK
#define pthread_mutexattr_create PTHREAD_MUTEXATTR_CREATE
#define pthread_mutexattr_delete PTHREAD_MUTEXATTR_DELETE
#define pthread_mutexattr_destroy PTHREAD_MUTEXATTR_DESTROY
#define pthread_mutexattr_getkind_np PTHREAD_MUTEXATTR_GETKIND_NP
#define pthread_mutexattr_init PTHREAD_MUTEXATTR_INIT
#define pthread_mutexattr_setkind_np PTHREAD_MUTEXATTR_SETKIND_NP
#define pthread_mutexattr_settype_np PTHREAD_MUTEXATTR_SETTYPE_NP
#define pthread_once PTHREAD_ONCE
#define pthread_resume_np PTHREAD_RESUME_NP
#define pthread_self PTHREAD_SELF
#define pthread_setasynccancel PTHREAD_SETASYNCCANCEL
#define pthread_setcancel PTHREAD_SETCANCEL
#define pthread_setcancelstate PTHREAD_SETCANCELSTATE
#define pthread_setprio PTHREAD_SETPRIO
#define pthread_setschedparam PTHREAD_SETSCHEDPARAM
#define pthread_setscheduler PTHREAD_SETSCHEDULER
#define pthread_setspecific PTHREAD_SETSPECIFIC
#define pthread_suspend_np PTHREAD_SUSPEND_NP
#define pthread_testcancel PTHREAD_TESTCANCEL
#define pthread_unlock_global_np PTHREAD_UNLOCK_GLOBAL_NP
#define pthread_yield PTHREAD_YIELD
#define pthread_yield_np PTHREAD_YIELD_NP

/* new for OpenVMS 7.3.2 ECO 1 */
#define pthread_attr_setscope       PTHREAD_ATTR_SETSCOPE
#define pthread_attr_setstackaddr   PTHREAD_ATTR_SETSTACKADDR
#define pthread_condattr_destroy    PTHREAD_CONDATTR_DESTROY
#define pthread_key_delete          PTHREAD_KEY_DELETE
#define pthread_setcanceltype       PTHREAD_SETCANCELTYPE

#endif // if ACE_NEEDS_PTHREAD_UPPERCASE

#endif
