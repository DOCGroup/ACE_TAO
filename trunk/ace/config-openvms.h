/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for OpenVMS 7.3-2

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H

#if !defined (ACE_USE_RCSID)
# define ACE_USE_RCSID 0
#endif

#pragma message disable CODCAUUNR
#pragma message disable CODEUNREACHABLE
//#pragma message disable DOLLARID
//#pragma message disable NOSIMPINT
//#pragma message disable NOSTDLONGLONG
#pragma message disable NARROWPTR
//#pragma message disable LONGEXTERN
#pragma message disable UNSCOMZER

#define ACE_OPENVMS 0x0821

#define ACE_DLL_SUFFIX ACE_LIB_TEXT("")

#define ACE_HAS_DUMP    1

// need this includes to ensure proper sequence of definitions so that
// f.i. HP C/C++ does not '#define ' memcpy, memmove etc.
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#undef clearerr
#undef memset
#undef memcpy
#undef memmove

#define ACE_DEFAULT_BASE_ADDR ((char*)(0x30000000))

#define ACE_MAX_UDP_PACKET_SIZE 65535

#define ACE_HAS_STDCPP_STL_INCLUDES 1

/* missing system headers */
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
#define ACE_LACKS_SCHED_H 1
#define ACE_LACKS_SYS_SYSCTL_H 1
#define ACE_LACKS_MALLOC_H 1
                                        /*Platform lacks malloc.h*/
#define ACE_LACKS_SYS_PARAM_H 1
                                        /*Platform lacks <sys/param.h>
                                        (e.g., MVS)*/
#define ACE_LACKS_SIGINFO_H 1
                                        /*Platform lacks the siginfo.h
                                        include file (e.g., MVS)*/
#define ACE_LACKS_UCONTEXT_H 1
                                        /*Platform lacks the ucontext.h
                                        file*/

/* missing rtl functions */
#define ACE_LACKS_SETPGID 1
#define ACE_LACKS_SETREUID 1
#define ACE_LACKS_SETREGID 1
#define ACE_LACKS_FORK 1
#define ACE_LACKS_GETPGID 1
#define ACE_LACKS_SETSID 1
#define ACE_LACKS_FCNTL 1
#define ACE_LACKS_SETEGID 1
#define ACE_LACKS_SETEUID 1

#define ACE_LACKS_REALPATH 1

#define ACE_LACKS_SYMLINKS 1

#define ACE_LACKS_PWD_REENTRANT_FUNCTIONS 1
#define ACE_LACKS_RAND_REENTRANT_FUNCTIONS 1

#define ACE_HAS_SNPRINTF 1
                                        /*Platform offers snprintf().*/
#define ACE_HAS_P_READ_WRITE
                                        /*Platform has pread() and
                                        pwrite() support*/
#define ACE_HAS_CHARPTR_DL 1
                                        /*OS/platform uses char * for
                                        dlopen/dlsym args, rather than
                                        const char *.*/
#define ACE_HAS_CLOCK_GETTIME 1
                                        /*Platform supports POSIX 1.b
                                        clock_gettime ()*/
#define ACE_HAS_CLOCK_SETTIME 1
                                        /*Platform supports POSIX.1b
                                        clock_settime ()*/
#define ACE_HAS_VOIDPTR_GETTIMEOFDAY 1
                                        /* 2nd arg 'void*' instead of 'struct timezone*' */
#define ACE_HAS_DIRENT 1
                                        /*Compiler/platform has Dirent
                                        iterator functions*/
#define ACE_HAS_GETPAGESIZE 1
                                        /*Platform supports
                                        getpagesize() call (otherwise,
                                        ACE_PAGE_SIZE must be defined,
                                        except on Win32)*/
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
#define ACE_HAS_STRNLEN 1
                                        /*Platform supports strnlen(3).*/
#define ACE_HAS_STREAMS 1
                                        /*Platform supports STREAMS*/
#define ACE_HAS_STRERROR 1
                                        /*Compiler/platform supports strerror ()*/
#define ACE_HAS_UALARM 1
                                        /*Platform supports ualarm()*/
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
#define ACE_LACKS_MKFIFO 1
                                        /*Platform lacks mkfifo() e.g.,
                                        VxWorks, Chorus, pSoS, and WinNT.*/
#define ACE_LACKS_NETDB_REENTRANT_FUNCTIONS 1
                                        /*Platform does not support
                                        reentrant netdb functions
                                        (getprotobyname_r,
                                        getprotobynumber_r,
                                        gethostbyaddr_r,
                                        gethostbyname_r,
                                        getservbyname_r).*/
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
#define ACE_LACKS_SETSCHED
                                        /*Platform lacks
                                        pthread_attr_setsched()
                                        (e.g. MVS)*/
//#define ACE_LACKS_SOCKETPAIR 1
                                        /*Platform lacks the
                                        socketpair() call (e.g., SCO
                                        UNIX)*/
#define ACE_LACKS_SYSCALL 1
                                        /*Platform doesn't have
                                        syscall() prototype*/

/* (missing) standard data types */
//#define ACE_LACKS_TIMESPEC_T    1
#define ACE_LACKS_CONST_TIMESPEC_PTR 1

#define ACE_LACKS_SUSECONDS_T 1

#define ACE_HAS_IDTYPE_T 1
                                        /*Compiler/platform supports
                                        idtype_t.*/
#define ACE_HAS_SIGINFO_T 1
                                        /*Platform supports SVR4
                                        extended signals*/
#define ACE_HAS_XPG4_MULTIBYTE_CHAR 1
                                        /*Platform has support for
                                        multi-byte character support
                                        compliant with the XPG4
                                        Worldwide Portability
                                        Interface wide-character
                                        classification.*/
#define ACE_HAS_SIZET_SOCKET_LEN 1
                                        /*OS/compiler uses size_t *
                                        rather than int * for socket
                                        lengths*/
#define ACE_HAS_SSIZE_T 1
                                        /*Compiler supports the ssize_t
                                        typedef*/
#define ACE_LACKS_PRI_T 1
                                        /*Platform lacks pri_t (e.g.,
                                        Tandem NonStop UNIX).*/
#define ACE_LACKS_SEMBUF_T 1
                                        /*Platform lacks struct sembuf
                                        (e.g., Win32 and VxWorks)*/
#define ACE_LACKS_STRRECVFD 1
                                        /*Platform doesn't define struct
                                        strrecvfd.*/
#define ACE_LACKS_T_ERRNO 1
                                        /*Header files lack t_errno for
                                        TLI*/

/* POSIX threads ompatibilities */
#define ACE_LACKS_RWLOCK_T 1
#define ACE_LACKS_PTHREAD_KILL 1
#define ACE_LACKS_THREAD_PROCESS_SCOPING 1

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

#define ACE_LACKS_UNNAMED_SEMAPHORE 1

#define ACE_MT_SAFE 1
                                        /*Compile using multi-thread libraries*/
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
#define ACE_LACKS_PTHREAD_SIGMASK 1
                                        /*Platform lacks pthread_sigmask ().*/
#define ACE_LACKS_PTHREAD_THR_SIGSETMASK 1
                                        /*Platform lacks
                                        pthread_thr_sigsetmask (e.g.,
                                        MVS, HP/UX, and OSF/1 3.2)*/
#define ACE_NEEDS_HUGE_THREAD_STACKSIZE (64U*1024)
                                        /*Required by platforms with small default stacks.*/
#define ACE_HAS_PTHREAD_SETCONCURRENCY
#define ACE_HAS_PTHREAD_GETCONCURRENCY


/* language/platform conformance */
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
#define ACE_HAS_AUTOMATIC_INIT_FINI 1
                                        /*Compiler/platform correctly
                                        calls init()/fini() for shared
                                        libraries*/
#define ACE_LACKS_UNIX_SIGNALS 1

#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES 1
                                        /*Prototypes for both signal()
                                        and struct sigaction are
                                        consistent.*/
#define ACE_HAS_CPLUSPLUS_HEADERS 1
                                        /*Compiler/platform has
                                        correctly prototyped header
                                        files*/
#define ACE_HAS_EXCEPTIONS 1
                                        /*Compiler supports C++
                                        exception handling*/
#define ACE_LACKS_LINEBUFFERED_STREAMBUF 1

#define ACE_HAS_GPERF 1
                                        /*The GPERF utility is compiled
                                        for this platform*/
#define ACE_HAS_IP_MULTICAST 1
                                        /*Platform supports IP multicast*/
#define ACE_HAS_POSIX_NONBLOCK 1
                                        /*Platform supports POSIX
                                        O_NONBLOCK semantics*/
#define ACE_HAS_POSIX_TIME 1
                                        /*Platform supports the POSIX
                                        struct timespec type*/
#define ACE_HAS_BROKEN_POSIX_TIME 1
                                        /* but in <timers.h> */
#define ACE_HAS_STRPTIME 1
                                        /*Enables ACE_OS::strptime ().*/
#define ACE_HAS_STANDARD_CPP_LIBRARY 1
                                        /*Platform/compiler supports
                                        Standard C++ Library*/
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

#define ACE_HAS_TEMPLATE_TYPEDEFS 1
                                        /*Compiler implements templates
                                        that support typedefs inside
                                        of classes used as formal
                                        arguments to a template
                                        class.*/
#define ACE_LACKS_NAMED_POSIX_SEM 1
                                        /*Platform lacks named POSIX
                                        semaphores (e.g., Chorus)*/
#define ACE_LACKS_SYSV_SHMEM 1
                                        /*Platform lacks System V shared
                                        memory (e.g., Win32 and
                                        VxWorks)*/
#define ACE_LACKS_UNIX_DOMAIN_SOCKETS 1
                                        /*ACE platform has no UNIX
                                        domain sockets*/
#define ACE_LACKS_UNIX_SYSLOG 1

#define ACE_HAS_SOCK_BUF_SIZE_MAX

#endif
