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
#define ACE_OPENVMS
#define ACE_DLL_SUFFIX ACE_LIB_TEXT("")

#define ACE_HAS_DUMP    1

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

//#include <timers.h>
//#include <pthread.h>
//#include <sys/uio.h>

#define ACE_DEFAULT_BASE_ADDR ((char*)(0x30000000))

//#define __ACE_INLINE__ 1
//#define ACE_HAS_WCHAR 1
//#define ACE_LACKS_THREAD_STACK_ADDR 1
//#define ACE_HAS_STD_TEMPLATE_SPECIALIZATION 1
//#define ACE_HAS_STD_TEMPLATE_METHOD_SPECIALIZATION 1
//#define ACE_HAS_STD_TEMPLATE_CLASS_MEMBER_SPECIALIZATION 1
//#define ACE_HAS_TYPENAME_KEYWORD 1

//#define ACE_LACKS_POLL_H 1

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

#define ACE_LACKS_TIMESPEC_T    1
#define ACE_LACKS_CONST_TIMESPEC_PTR 1

#define ACE_LACKS_SHMID_DS_T  1
#define ACE_LACKS_MSQID_DS_T  1
#define ACE_LACKS_RWLOCK_T 1
//#define ACE_HAS_PTHREADS_UNIX98_EXT 1
//#define ACE_LACKS_MUTEXATTR_PSHARED 1
//#define ACE_LACKS_CONDATTR_PSHARED 1
#define ACE_LACKS_PTHREAD_KILL 1
#define ACE_LACKS_THREAD_PROCESS_SCOPING 1

#define ACE_LACKS_LINEBUFFERED_STREAMBUF 1

#define ACE_LACKS_PWD_REENTRANT_FUNCTIONS 1
#define ACE_LACKS_RAND_REENTRANT_FUNCTIONS 1

//#define ACE_CAST_CONST
					/*Used to work around broken
                                        SunCC ANSI casts that require
                                        an extra const.*/
//#define ACE_DEFINES_DEFAULT_WIN32_SECURITY_ATTRIBUTES
					/*
                                        Win32 only.  Users want to use
                                        a predefined security
                                        attributes defined in
                                        ACE_OS::default_win32_security_attributes
                                        as the default security
                                        object.*/
//#define ACE_DISABLE_DEBUG_DLL_CHECK
					/*Define this if you don't want
                                        debug version ACE search for
                                        debug version DLLs first
                                        before looking for the DLL
                                        names specified.*/
//#define ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER
					/*
                                        Application will allocate its
                                        own object manager.  This
                                        implicitly defines
                                        ACE_HAS_NONSTATIC_OBJECT_MANAGER.
                                        Usually used with MFC
                                        applications.*/
//#define ACE_MAIN
					/*Renames "main (int, char *[])",
                                        for platforms such as g++/VxWorks
                                        that don't allow "main".  Requires
                                        the use of
                                        ACE_HAS_NONSTATIC_OBJECT_MANAGER.*/
#define ACE_MT_SAFE 1
					/*Compile using multi-thread libraries*/
//#define ACE_NDEBUG
					/*Turns off debugging features*/
#define ACE_NEW_THROWS_EXCEPTIONS 1
					/*Compiler's 'new' throws exception on
                                        failure (ANSI C++ behavior).*/
//#define ACE_NLOGGING
					/*Turns off the LM_DEBUG and
                                        LM_ERROR logging macros...*/
//#define ACE_PAGE_SIZE
					/*Defines the page size of the
                                        system (not used on Win32 or
                                        with ACE_HAS_GETPAGESIZE).*/
//#define ACE_REDEFINES_XTI_FUNCTIONS
					/*Platform redefines the t_... names (UnixWare)*/
//#define ACE_SELECT_USES_INT 1
					/*Platform uses int for select()
                                        rather than fd_set*/
//#define ACE_TEMPLATES_REQUIRE_PRAGMA
					/*Compiler's template mechanism
                                        must use a pragma This is used
                                        for AIX's C++ compiler.*/
#define ACE_TEMPLATES_REQUIRE_SOURCE 1
					/*Compiler's template mechanim
                                        must see source code (i.e.,
                                        .cpp files).  This is used for
                                        GNU G++.*/
//#define ACE_TIMER_SKEW
					/*If a timed ::select () can return
                                        early, then ACE_TIMER_SKEW is the
                                        maximum adjustment, in microseconds,
                                        that ACE_Timer_Queue uses to
                                        compensate for the early return.*/
//#define ACE_TLI_TCP_DEVICE
					/*Device the platform uses for TCP on
                                        TLI.  Only needed if not /dev/tcp.*/
//#define ACE_USE_POLL
					/*The OS platform supports the
                                        poll() event demultiplexor*/
//#define ACE_USES_ASM_SYMBOL_IN_DLSYM
					/*Platform uses assembly symbols
                                        instead of C symbols in
                                        dlsym()*/
//#define ACE_USES_STATIC_MFC
					/*When linking MFC as a static library is desired*/
#define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 1
					/*Platform has its standard c++
                                        library in the namespace std.*/
//#define ACE_WSOCK_VERSION
					/*A parameter list indicating
                                        the version of WinSock (e.g.,
                                        "1, 1" is version 1.1).
*/
//#define ACE_HAS_AIO_CALLS
					/*Platform supports POSIX aio* calls.
                                        Corresponds to _POSIX_ASYNCHRONOUS_IO
                                        constant in <unistd.h>.*/
//#define ACE_HAS_ALT_CUSERID
					/*Use ACE's alternate cuserid()
                                        implementation since a system
                                        cuserid() may not exist, or it
                                        is not desirable to use it.
                                        The implementation requires
                                        ACE_LACKS_PWD_FUNCTIONS to be
                                        undefined and that the
                                        geteuid() system call exists.*/
//#define ACE_DEFAULT_THREAD_KEYS
					/*Number of TSS keys, with
                                        ACE_HAS_TSS_EMULATION _only_.
                                        Defaults to 64.*/
//#define ACE_DEFAULT_LD_SEARCH_PATH
					/*Specify the platform default search
                                        paths.  This macro should only be
                                        defined on platforms that don't
                                        support environment variables at all
                                        (i.e., Windows CE.)*/
//#define ACE_THREADS_DONT_INHERIT_LOG_MSG
					/*Specify this if you don't want
                                        threads to inherit parent
                                        thread's ACE_Log_Msg
                                        properties.*/
//#define ACE_THREAD_MANAGER_USES_SAFE_SPAWN
					/*Disable the "check before lock" feature
                                        in ACE_Thread_Manager.  Defining this
                                        macro avoids a potential race condition
                                        on platforms with aggressive read/write
                                        reordering.*/
//#define ACE_HAS_GNUG_PRE_2_8
					/*Compiling with g++ prior to
                                        version 2.8.0.*/
//#define ACE_HAS_PRIOCNTL
					/*OS has priocntl (2).*/
//#define ACE_HAS_RECURSIVE_MUTEXES
					/*Mutexes are inherently recursive (e.g., Win32)*/
//#define ACE_HAS_RECV_TIMEDWAIT
					/*Platform has the MIT pthreads
                                        APIs for*/
//#define ACE_HAS_RECVFROM_TIMEDWAIT
					/*timed send/recv operations*/
//#define ACE_HAS_RECVMSG_TIMEDWAIT
					/*
ACE_HAS_RLIMIT_RESOURCE_ENUM            Platform has enum instead of
                                        int for first argument to
                                        ::{get,set}rlimit ().  The
                                        value of this macro is the
                                        enum definition, e.g., enum
                                        __rlimit_resource, for Linux
                                        glibc 2.0.*/
//#define ACE_HAS_RUSAGE_WHO_ENUM
					/*Platform has enum instead of
                                        int for first argument to
                                        ::getrusage ().  The value of
                                        this macro is the enum
                                        definition, e.g., enum
                                        __rusage_who, for Linux glibc
                                        2.0.*/
//#define ACE_HAS_SEND_TIMEDWAIT
//#define ACE_HAS_SENDTO_TIMEDWAIT
//#define ACE_HAS_SENDMSG_TIMEDWAIT
//#define ACE_HAS_STDARG_THR_DEST 1
					/*Platform has void (*)(...)
                                        prototype for
                                        pthread_key_create()
                                        destructor (e.g., LynxOS).*/
#define ACE_HAS_SNPRINTF 1
					/*Platform offers snprintf().*/
//#define ACE_HAS_STL_MAP_CONFLICT
					/*Used when users want to
                                        compile ACE with STL and STL
                                        map class conflicts with
                                        <net/if.h> map struct.*/
//#define ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS
					/*Platform/compiler supports
                                        Win32 structural exceptions*/
//#define ACE_HAS_READ_TIMEDWAIT
//#define ACE_HAS_READV_TIMEDWAIT
//#define ACE_HAS_WRITE_TIMEDWAIT
//#define ACE_HAS_WRITEV_TIMEDWAIT

//#define ACE_HAS_4_4BSD_SENDMSG_RECVMSG
					/*Platform has BSD 4.4
                                        sendmsg()/recvmsg() APIs.*/
#define ACE_HAS_P_READ_WRITE
					/*Platform has pread() and
                                        pwrite() support*/
//#define ACE_HAS_64BIT_LONGS
					/*Platform has 64bit longs and
                                        32bit ints.  NOTE: this macro
                                        is deprecated.  Instead, use
                                        ACE_SIZEOF_LONG == 8.*/
//#define ACE_HAS_AIX_BROKEN_SOCKET_HEADER
					/*Platform, such as AIX4, needs
                                        to wrap #include of
                                        sys/socket.h with
                                        #undef///#define of
                                        __cplusplus.*/
//#define ACE_HAS_AIX_HI_RES_TIMER
					/*Platform has AIX4
                                        ::read_real_time ()*/
//#define ACE_HAS_ALLOCA
					/*Compiler/platform supports
                                        alloca()*/
//#define ACE_HAS_ALLOCA_H
					/*Compiler/platform has
                                        <alloca.h>*/
//#define ACE_HAS_ALPHA_TIMER 1 // only linux !!! // gp
					/*CPU is an Alpha, with the rpcc
                                        instruction to read the tick timer.
                                        Limited to 32 bits, so not recommended.*/
#define ACE_HAS_AUTOMATIC_INIT_FINI 1
					/*Compiler/platform correctly
                                        calls init()/fini() for shared
                                        libraries*/
//#define ACE_HAS_BIG_FD_SET
					/*Compiler/platform has typedef
                                        u_long fdmask (e.g., Linux and
                                        SCO).*/
//#define ACE_HAS_WORKING_EXPLICIT_TEMPLATE_DESTRUCTOR
					/*
                                        Compiler handles explicit calling of
                                        template destructor correctly. See
                                        "ace/OS.h" for details.*/
//#define ACE_HAS_BROKEN_ACCEPT_ADDR
					/*Platform can't correctly deal
                                        with a NULL addr to accept()
                                        (e.g, VxWorks).*/
//#define ACE_HAS_BROKEN_NAMESPACES
					/*Compiler/platform doesn't
                                        support namespaces (or the
                                        support is not fully
                                        implemented.)*/
//#define ACE_HAS_BROKEN_BITSHIFT
					/*Compiler has integer overflow
                                        problem with bit-shift
                                        operations.*/
//#define ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS
					/*Compiler can't handle const char *
                                        as rvalue in conditional operator.*/
//#define ACE_HAS_BROKEN_CONVERSIONS
					/*Compiler can't handle calls
                                        like foo->operator T *()*/
//#define ACE_HAS_BROKEN_CTIME
					/*Compiler/platform uses macro
                                        for ctime (e.g., MVS)*/
//#define ACE_HAS_BROKEN_DGRAM_SENDV
					/*Platform sendv() does not work
                                        properly with datagrams,
                                        i.e. it fails when the iovec
                                        size is IOV_MAX.*/
//#define ACE_HAS_BROKEN_ENUMS
					/*Compiler can't handle large
                                        enums (e.g., HP/UX C++)*/
//#define ACE_HAS_BROKEN_HPUX_TEMPLATES 1
					/*Earlier versions of HP/UX C++
                                        are damned...*/
//#define ACE_HAS_BROKEN_MAP_FAILED
					/*Platform doesn't cast MAP_FAILED
                                        to a void *.*/
//#define ACE_HAS_BROKEN_MSG_H 1
					/*Platform headers don't support
                                        <msg.h> prototypes*/
//#define ACE_HAS_BROKEN_MMAP_H
					/*HP/UX does not wrap the
                                        mmap(2) header files with
                                        extern "C".*/
//#define ACE_HAS_BROKEN_NESTED_TEMPLATES
					/*MSVC has trouble with defining
                                        STL containers for nested
                                        structs and classes*/
//#define ACE_HAS_BROKEN_POSIX_TIME
					/*Platform defines struct
                                        timespec in <sys/timers.h>*/
//#define ACE_HAS_BROKEN_RANDR
					/*OS/compiler's header files are
                                        inconsistent with libC
                                        definition of rand_r().*/
//#define ACE_HAS_BROKEN_READV()
					/*OS/Compiler's header files are
                                        not consistent with readv()
                                        definition.*/
//#define ACE_HAS_BROKEN_SAP_ANY
					/*Compiler can't handle the
                                        static ACE_Addr::sap_any
                                        construct.*/
//#define ACE_HAS_BROKEN_SENDMSG
					/*OS/compiler omits the const
                                        from the sendmsg() prototype.*/
//#define ACE_HAS_BROKEN_SETRLIMIT
					/*OS/compiler omits the const
                                        from the rlimit parameter in
                                        the setrlimit() prototype.*/
//#define ACE_HAS_BROKEN_T_ERROR
					/*Compiler/platform has the wrong
                                        prototype for t_error(), i.e.,
                                        t_error(char *) rather than
                                        t_error(const char *).*/
//#define ACE_HAS_BROKEN_TIMESPEC_MEMBERS
					/*platform define struct
                                        timespec members as ts_sec and
                                        ts_nsec instead of tv_sec and
                                        tv_nsec.  This is highly
                                        non-portable.  Currently only
                                        FreeBSD 2.1.x uses it.*/
//#define ACE_HAS_BROKEN_WRITEV
					/*OS/compiler omits the const
                                        from the iovec parameter in
                                        the writev() prototype.*/
//#define ACE_HAS_BROKEN_XTI_MACROS
					/*OS header files have some
                                        problems with XTI (HP/UX 11).*/
//#define ACE_HAS_BSTRING
					/*Platform has <bstring.h>
                                        (which contains bzero()
                                        prototype)*/
//#define ACE_HAS_BYTESEX_H
					/*Platform has <bytesex.h>.*/
//#define ACE_HAS_CANCEL_IO
					/*Platform supports the Win32
                                        CancelIO() function (WinNT 4.0
                                        and beyond).*/
#define ACE_HAS_CHARPTR_DL 1
					/*OS/platform uses char * for
                                        dlopen/dlsym args, rather than
                                        const char *.*/
//#define ACE_HAS_CHARPTR_SOCKOPT
					/*OS/platform uses char * for
                                        sockopt, rather than const
                                        char **/
//#define ACE_HAS_CHARPTR_SPRINTF
					/*sprintf() returns char *
                                        rather than int (e.g., SunOS
                                        4.x)*/
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
//#define ACE_HAS_CYGWIN32_SOCKET_H
					/*Platform has cygwin32 socket.h*/
#define ACE_HAS_DIRENT 1
					/*Compiler/platform has Dirent
                                        iterator functions*/
//#define ACE_HAS_DLFCN_H_BROKEN_EXTERN_C
					/*For platforms, e.g., RedHat
                                        4.2/Linux 2.0.30/Alpha, that
                                        don't declare dl* functions as
                                        extern "C" in dlfcn.h.*/
//#define ACE_HAS_DLL
					/*Build ACE using the frigging
                                        PC DLL nonsense...*/
#define ACE_HAS_EXCEPTIONS 1
					/*Compiler supports C++
                                        exception handling*/
//#define ACE_HAS_BROKEN_UNEXPECTED_EXCEPTIONS
					/*Compiler does not call
                                        unexpected exception handler
                                        if exception not listed in
                                        exception specification is
                                        thrown.  In particular, the
                                        exception specification is not
                                        respected.*/
#define ACE_HAS_EXPLICIT_KEYWORD 1
					/*Compiler support explicit constructors.*/
//#define ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION 1
					/*Compiler's template
                                        instantiation mechanism
                                        supports the use of explicit
                                        C++ specializations for all
                                        used templates. This is also
                                        used for GNU G++ if you don't
                                        use the "repo" patches.*/
//#define ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
					/*When a base-class is a
                                        specialization of a class template
                                        then this class template must be
                                        explicitly exported*/
#define ACE_HAS_GETPAGESIZE 1
					/*Platform supports
                                        getpagesize() call (otherwise,
                                        ACE_PAGE_SIZE must be defined,
                                        except on Win32)*/
//#define ACE_HAS_GETRUSAGE
					/*Platform supports the
                                        getrusage() system call.*/
//#define ACE_HAS_GETRUSAGE_PROTO
					/*Platform has a getrusage ()
                                        prototype in sys/resource.h
                                        that differs from the one in
                                        ace/OS.i.*/
//#define ACE_HAS_GNU_CSTRING_H
					/*Denotes that GNU has cstring.h
                                        as standard which redefines
                                        memchr()*/
#define ACE_HAS_GPERF 1
					/*The GPERF utility is compiled
                                        for this platform*/
//#define ACE_HAS_GETIFADDRS
					/*This platform has ifaddrs.h and
                                        the getifaddrs() function.  This
                                        is used in preference to
                                        the SIOCGIFCONF ioctl call, since
                                        it is much simpler and supports
                                        IPv6 and non-IP interfaces better.*/
//#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT
					/*Optimize
                                        ACE_Handle_Set::count_bits for
                                        select() operations (common
                                        case)*/
//#define ACE_HAS_LLSEEK
					/*Platform supports llseek.*/
//#define ACE_HAS_HI_RES_TIMER
					/*Compiler/platform supports
                                        SunOS high resolution timers*/
#define ACE_HAS_IDTYPE_T 1
					/*Compiler/platform supports
                                        idtype_t.*/
//#define ACE_HAS_INLINED_OSCALLS
					/*Inline all the static class OS
                                        methods to remove call
                                        overhead*/
#define ACE_HAS_IP_MULTICAST 1
					/*Platform supports IP multicast*/
//#define ACE_HAS_IPV6 1
					/*Platform supports IPv6.*/
//#define ACE_USES_IPV4_IPV6_MIGRATION
					/*Enable IPv6 support in ACE on
					platforms that don't have IPv6
					turned on by default.*/
//#define ACE_HAS_IRIX62_THREADS
					/*Platform supports the very odd
                                        IRIX 6.2 threads...*/
//#define ACE_HAS_NONSTATIC_OBJECT_MANAGER
					/*Causes the ACE_Object_Manager
                                        instance to be created in main
                                        (int, char *[]), instead of as
                                        a static (global) instance.*/
//#define ACE_HAS_THR_KEYDELETE
					/*Platform supports
                                        thr_keydelete (e.g,. UNIXWARE)*/
//#define ACE_HAS_THR_MINSTACK
					/*Platform calls thr_minstack()
                                        rather than thr_min_stack()
                                        (e.g., Tandem).*/
//#define ACE_HAS_LIMITED_RUSAGE_T
					/*The rusage_t structure has
                                        only two fields.*/
//#define ACE_HAS_LIMITED_SELECT
					/*The select is unable to deal with
                                        large file descriptors.*/
//#define ACE_HAS_LONG_MAP_FAILED
					/*Platform defines MAP_FAILED as
                                        a long constant.*/
//#define ACE_HAS_MALLOC_STATS
					/*Enabled malloc statistics
                                        collection.*/
#define ACE_HAS_MEMCHR 1
					/*Use native implementation of memchr.*/
//#define ACE_HAS_MINIMAL_ACE_OS
					/*Disables some #includes in ace/OS.*.*/
//#define ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION
					/*Avoid #including ace/streams.h
                                        in OS.h.  Users must include
                                        ace/streams.h, <iostream>, or
                                        <iostream.h> explicitly in
                                        their code.  Some platforms,
                                        such as g++/VxWorks, have
                                        trouble compiling templates
                                        and iostreams header because
                                        of static variables in the
                                        stream headers.  This flag
                                        will also avoid extra
                                        compilation and runtime
                                        overheads on some platforms.*/
//#define ACE_HAS_MFC
					/*Platform supports Microsoft
                                        Foundation Classes*/
#define ACE_HAS_MSG
					/*Platform supports recvmsg and
                                        sendmsg*/
//#define ACE_HAS_MT_SAFE_MKTIME
					/*Platform supports MT safe
                                        mktime() call (do any of
                                        them?)*/
//#define ACE_HAS_MUTABLE_KEYWORD
					/*Compiler supports mutable.*/
//#define ACE_HAS_MUTEX_TIMEOUTS
					/*Compiler supports timed mutex
                                        acquisitions
                                        (e.g. pthread_mutex_timedlock()).*/
//#define ACE_HAS_NEW_NOTHROW
					/*Compiler offers new (nothrow).*/
//#define ACE_HAS_NONCONST_GETBY 1
					/*Platform uses non-const char *
                                        in calls to gethostbyaddr,
                                        gethostbyname, getservbyname*/
//#define ACE_HAS_NONCONST_MSGSND
					/*Platform has a non-const
                                        parameter to msgsend() (e.g.,
                                        SCO).*/
#define ACE_HAS_NONCONST_SELECT_TIMEVAL 1
					/*Platform's select() uses
                                        non-const timeval* (only found
                                        on Linux right now)*/
//#define ACE_HAS_OLD_MALLOC
					/*Compiler/platform uses old
                                        malloc()/free() prototypes
                                        (ugh)*/
//#define ACE_HAS_ONLY_SCHED_OTHER
					/*Platform, e.g., Solaris 2.5,
                                        only supports SCHED_OTHER
                                        POSIX scheduling policy.*/
#define ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R 1
					/*Uses ctime_r & asctime_r with
                                        only two parameters
                                        vs. three.*/
//#define ACE_HAS_OSF_TIMOD_H
					/*Platform supports the OSF TLI
                                        timod STREAMS module*/
#define ACE_HAS_3_PARAM_WCSTOK 1
					/*Platform has 3-parameter version
                                        of wcstok(), which was added in
                                        1994 in the ISO C standard Normative
                                        Addendum 1.  Other standards like XPG4
                                        define a 2 parameter wcstok().*/
//#define ACE_HAS_PENTIUM
					/*Platform is an Intel Pentium
                                        microprocessor.*/
//#define ACE_HAS_POLL
					/*Platform contains <poll.h>*/
//#define ACE_HAS_POSITION_INDEPENDENT_POINTERS
					/*Platform supports
                                        "position-independent" features
                                        provided by ACE_Based_Pointer<>.*/
//#define ACE_HAS_POSIX_MESSAGE_PASSING
					/*Platform supports POSIX message queues.
                                        Corresponds to _POSIX_MESSAGE_PASSING
                                        constant in <unistd.h>.*/
#define ACE_HAS_POSIX_NONBLOCK 1
					/*Platform supports POSIX
                                        O_NONBLOCK semantics*/
//#define ACE_HAS_POSIX_REALTIME_SIGNALS
					/*Platform supports POSIX RT signals.
                                        Corresponds to _POSIX_REALTIME_SIGNALS
                                        constant in <unistd.h>. */
//#define ACE_HAS_POSIX_SEM
					/*Platform supports POSIX
                                        real-time semaphores (e.g.,
                                        VxWorks and Solaris).  Corresponds
                                        to _POSIX_SEMAPHORES constant
                                        in <unistd.h>*/
#define ACE_HAS_POSIX_TIME 1
					/*Platform supports the POSIX
                                        struct timespec type*/
//#define ACE_HAS_PROC_FS
					/*Platform supports the /proc
                                        file system and defines tid_t
                                        in <sys/procfs.h>*/
//#define ACE_HAS_POWERPC_TIMER
					/*Platform supports PowerPC
                                        time-base register.*/
//#define ACE_HAS_PRUSAGE_T
					/*Platform supports the
                                        prusage_t struct*/
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
//#define ACE_HAS_PTHREADS_DRAFT4
					/*Platform's 'Pthreads' is .4a draft 4*/
//#define ACE_HAS_PTHREADS_DRAFT6
					/*Platform's 'Pthreads' is .4a draft 6*/
//#define ACE_HAS_PTHREADS_DRAFT7 1
					/*Platform's 'Pthreads' is .1c draft 7*/
#define ACE_HAS_PTHREADS_STD 1
					/*Platform supports POSIX.1c-1995 threads
                                        (This is the final standard
                                        Pthreads).*/
//#define ACE_HAS_PTHREADS_UNIX98_EXT 1
					/*Platform has the UNIX98 extensions to
                                        Pthreads (susp/cont, rwlocks)*/
//#define ACE_HAS_PTHREAD_CONDATTR_SETKIND_NP
					/*Platform has pthread_condattr_setkind_np().*/
//#define ACE_HAS_PTHREAD_MUTEXATTR_SETKIND_NP 1
					/*Platform has
                                        pthread_mutexattr_setkind_np().*/
#define ACE_HAS_PTHREAD_PROCESS_ENUM 1
					/*pthread.h declares an enum with
                                        PTHREAD_PROCESS_PRIVATE and
                                        PTHREAD_PROCESS_SHARED values.*/
//#define ACE_HAS_PTHREAD_SETSTACK
					/*Platform has pthread_attr_setstack().*/
//#define ACE_HAS_PURIFY
					/*Purify'ing.  Set by wrapper_macros.GNU.*/
//#define ACE_HAS_QUANTIFY
					/*Quantify'ing.  Set by wrapper_macros.GNU.*/
//#define ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS
					/*Platform will recurse
                                        infinitely on thread exits
                                        from TSS cleanup routines
                                        (e.g., AIX).*/
//#define ACE_HAS_REENTRANT_FUNCTIONS
					/*Platform supports reentrant
                                        functions (i.e., all the POSIX
                                        *_r functions).*/
#define ACE_HAS_XPG4_MULTIBYTE_CHAR 1
					/*Platform has support for
                                        multi-byte character support
                                        compliant with the XPG4
                                        Worldwide Portability
                                        Interface wide-character
                                        classification.*/
//#define ACE_HAS_REGEX
					/*Platform supports the POSIX
                                        regular expression library*/
//#define ACE_HAS_DLSYM_SEGFAULT_ON_INVALID_HANDLE
					/*For OpenBSD: The dlsym call
                                        segfaults when passed an invalid
                                        handle.  Other platforms handle
                                        this more gracefully.*/
//#define ACE_HAS_SELECT_H
					/*Platform has special header for select().*/
//#define ACE_USE_SELECT_REACTOR_FOR_REACTOR_IMPL
					/*For Win32: Use Select_Reactor
                                        as default implementation of
                                        Reactor instead of
                                        WFMO_Reactor.*/
//#define ACE_HAS_SEMUN
					/*Compiler/platform defines a
                                        union semun for SysV shared
                                        memory*/
//#define ACE_HAS_SET_T_ERRNO
					/*Platform has a function to set
                                        t_errno (e.g., Tandem).*/
#define ACE_HAS_SIGINFO_T 1
					/*Platform supports SVR4
                                        extended signals*/
#define ACE_HAS_SIGSUSPEND 1
					/*Platform supports sigsuspend()*/
//#define ACE_HAS_SIGISMEMBER_BUG
					/*Platform has bug with
                                        sigismember() (HP/UX 11).*/
#define ACE_HAS_SIG_MACROS 1
					/*Platform/compiler has macros
                                        for sig{empty,fill,add,del}set
                                        (e.g., SCO and FreeBSD)*/
//#define ACE_HAS_SIGNAL_OBJECT_AND_WAIT
					/*Platform supports the Win32
                                        SignalObjectAndWait() function
                                        (WinNT 4.0 and beyond).*/
//#define ACE_HAS_SIGNAL_SAFE_OS_CALLS 1
					/*Automatically restart OS
                                        system calls when EINTR occurs*/
#define ACE_HAS_SIGWAIT 1
					/*Platform/compiler has the
                                        sigwait(2) prototype*/
//#define ACE_HAS_SIG_ATOMIC_T
					/*Compiler/platform defines the
                                        sig_atomic_t typedef*/
#define ACE_HAS_SIG_C_FUNC 1
					/*Compiler requires extern "C"
                                        functions for signals.*/
//#define ACE_HAS_SIN_LEN
					/*Platform supports new BSD
                                        inet_addr len field.*/
#define ACE_HAS_SIZET_SOCKET_LEN 1
					/*OS/compiler uses size_t *
                                        rather than int * for socket
                                        lengths*/
//#define ACE_HAS_SOCKADDR_MSG_NAME
					/*Platform requires (struct
                                        sockaddr *) for msg_name field
                                        of struct msghdr.*/
//#define ACE_HAS_SOCKIO_H
					/*Compiler/platform provides the
                                        sockio.h file*/
//#define ACE_HAS_SOCKLEN_T
					/*Platform provides socklen_t
                                        type, such as Linux with
                                        glibc2.*/
//#define ACE_HAS_SPARCWORKS_401_SIGNALS
					/*Compiler has brain-damaged
                                        SPARCwork SunOS 4.x signal
                                        prototype...*/
#define ACE_HAS_SSIZE_T 1
					/*Compiler supports the ssize_t
                                        typedef*/
#define ACE_HAS_STRPTIME 1
					/*Enables ACE_OS::strptime ().*/
//#define ACE_HAS_STHREADS
					/*Platform supports Solaris
                                        threads*/
#define ACE_HAS_STANDARD_CPP_LIBRARY 1
					/*Platform/compiler supports
                                        Standard C++ Library*/
//#define ACE_HAS_STRBUF_T
					/*Compiler/platform supports
                                        struct strbuf*/
//#define ACE_HAS_STRDUP_EMULATION
					/*Platform/compiler lacks
                                        strdup() (e.g., VxWorks,
                                        Chorus, WinCE)*/
#define ACE_HAS_STRNLEN 1
					/*Platform supports strnlen(3).*/
#define ACE_HAS_STREAMS 1
					/*Platform supports STREAMS*/
//#define ACE_HAS_STREAM_PIPES
					/*Platform supports STREAM pipes*/
#define ACE_HAS_STRERROR 1
					/*Compiler/platform supports strerror ()*/
//#define ACE_HAS_STRICT
					/*Use the STRICT compilation mode on Win32.*/
#define ACE_HAS_STRING_CLASS 1
					/*Platform/Compiler supports a
                                        String class (e.g., GNU or
                                        Win32).*/
//#define ACE_HAS_STRUCT_NETDB_DATA
					/*Compiler/platform has strange
                                        hostent API for socket *_r()
                                        calls*/
//#define ACE_HAS_SUNOS4_GETTIMEOFDAY
					/*SunOS 4 style prototype.*/
//#define ACE_HAS_SUNOS4_SIGNAL_T
					/*Compiler has horrible SunOS
                                        4.x signal handlers...*/
#define ACE_HAS_SVR4_DYNAMIC_LINKING 1
					/*Compiler/platform supports
                                        SVR4 dynamic linking semantics*/
//#define ACE_HAS_SVR4_GETTIMEOFDAY 1
					/*Compiler/platform supports
                                        SVR4 gettimeofday() prototype*/
//#define ACE_HAS_SVR4_SIGNAL_T
					/*Compiler/platform supports
                                        SVR4 signal typedef*/
//#define ACE_HAS_SVR4_TLI
					/*Compiler/platform supports
                                        SVR4 TLI (in particular,
                                        T_GETNAME stuff)...*/
//#define ACE_HAS_SYSCALL_GETRUSAGE
					/*HP/UX has an undefined syscall
                                        for GETRUSAGE...*/
//#define ACE_HAS_SYSCALL_H
					/*Compiler/platform contains the
                                        <sys/syscall.h> file.*/
//#define ACE_HAS_SYSENT_H
					/*Platform provides <sysent.h>
                                        header*/
//#define ACE_HAS_SYSINFO
					/*Platform supports system
                                        configuration information*/
//#define ACE_HAS_SYSV_IPC
					/*Platform supports System V IPC
                                        (most versions of UNIX, but
                                        not Win32)*/
//#define ACE_HAS_SYS_ERRLIST
					/*Platform/compiler supports
                                        _sys_errlist symbol*/
//#define ACE_HAS_SYS_FILIO_H
					/*Platform provides
                                        <sys/filio.h> header*/
//#define ACE_HAS_SYS_SIGLIST
					/*Compiler/platform supports
                                        _sys_siglist array*/
//#define ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA 1
					/*Compiler's template
                                        instantiation mechanism
                                        supports the use of "#pragma
                                        instantiate".  Edison Design
                                        Group compilers, e.g., SGI C++
                                        and Green Hills 1.8.8 and
                                        later, support this.*/
#define ACE_HAS_TEMPLATE_SPECIALIZATION 1
					/*Compiler implements template
                                        specialization*/
#define ACE_HAS_TEMPLATE_TYPEDEFS 1
					/*Compiler implements templates
                                        that support typedefs inside
                                        of classes used as formal
                                        arguments to a template
                                        class.*/
// Specialkod för VMS ?
//#define ACE_HAS_TERM_IOCTLS 1
					/*Platform has terminal ioctl
                                        flags like TCGETS and TCSETS.*/
//#define ACE_HAS_LAZY_MAP_MANAGER
					/*ACE supports lazy Map Managers
                                        that allow deletion of entries
                                        during active iteration.*/
#define ACE_HAS_THREADS 1
					/*Platform supports threads*/
//#define ACE_HAS_THREAD_SAFE_ACCEPT
					/*Platform allows multiple
                                        threads to call accept() on
                                        the same port (e.g., WinNT).*/
//#define ACE_HAS_THREAD_SELF
					/*Platform has thread_self()
                                        rather than pthread_self()
                                        (e.g., DCETHREADS and AIX)*/
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
#define ACE_HAS_TIMEZONE_GETTIMEOFDAY
					/*Platform/compiler supports
                                        timezone * as second parameter
                                        to gettimeofday()*/
//#define ACE_HAS_TIMOD_H
					/*Platform supports TLI timod
                                        STREAMS module*/
//#define ACE_HAS_TIUSER_H
					/*Platform supports TLI tiuser
                                        header*/
//#define ACE_HAS_TLI
					/*Platform supports TLI.  Also
                                        see ACE_TLI_TCP_DEVICE.*/
//#define ACE_HAS_TLI_PROTOTYPES
					/*Platform provides TLI function
                                        prototypes*/
//#define ACE_HAS_TSS_EMULATION 1
					/*ACE provides TSS emulation.
                                        See also
                                        ACE_DEFAULT_THREAD_KEYS.*/
#define ACE_HAS_UALARM 1
					/*Platform supports ualarm()*/
//#define ACE_HAS_UCONTEXT_T
					/*Platform supports ucontext_t
                                        (which is used in the extended
                                        signal API).*/
//#define ACE_HAS_UNION_WAIT
					/*The wait() system call takes a
                                        (union wait *) rather than int
                                        **/
//#define ACE_HAS_UNIXWARE_SVR4_SIGNAL_T
					/*Has inconsistent SVR4 signal
                                        stuff, but not the same as the
                                        other platforms*/
#define ACE_HAS_USING_KEYWORD 1
					/*Compiler supports the new
                                        using keyword for C++
                                        namespaces.*/
//#define ACE_HAS_VERBOSE_NOTSUP 1
					/*Prints out console message in
                                        ACE_NOTSUP.  Useful for
                                        tracking down origin of
                                        ACE_NOTSUP.*/
#define ACE_HAS_VOIDPTR_MMAP 1
					/*Platform requires void * for
                                        mmap().*/
#define ACE_HAS_VOIDPTR_SOCKOPT 1
					/*OS/compiler uses void * arg 4
                                        setsockopt() rather than const
                                        char **/
//#define ACE_HAS_WCSNLEN
					/*Platform supports wcsnlen(3).*/
//#define ACE_HAS_WIN32_TRYLOCK
					/*The Win32 platform support
                                        TryEnterCriticalSection()
                                        (WinNT 4.0 and beyond)*/
//#define ACE_HAS_WINSOCK2
					/*The Win32 platform supports
                                        WinSock 2.0*/
//#define ACE_HAS_XLI
					/*Platform has the XLI version
                                        of TLI*/
//#define ACE_HAS_XT 1
					/*Platform has Xt and Motif*/
//#define ACE_HAS_XTI
					/*Platform has XTI
                                        (X/Open-standardized superset
                                        of TLI).  Implies ACE_HAS_TLI
                                        but uses a different header
                                        file.*/
//#define ACE_LACKS_ACCESS
					/*Platform lacks access() (e.g.,
                                        VxWorks and Chorus)*/
//#define ACE_LACKS_ACE_IOSTREAM
					/*Platform can not build
                                        ace/IOStream{,_T}.cpp.  This
                                        does not necessarily mean that
                                        the platform does not support
                                        iostreams.*/
//#define ACE_LACKS_AUTO_MMAP_REPLACEMENT
					/*No system support for replacing any
                                        previous mappings.*/
//#define ACE_LACKS_BSEARCH
					/*Compiler/platform lacks the
                                        standard C library bsearch()
                                        function*/
//#define ACE_LACKS_CLEARERR
					/*Platform lacks the clearerr system
                                        call.*/
//#define ACE_LACKS_CMSG_DATA_MACRO
					/*Platform has
                                        ACE_HAS_4_4BSD_SENDMSG_RECVMSG but does
                                        not define CMSG_DATA (cmsg) macro.*/
//#define ACE_LACKS_CMSG_DATA_MEMBER
					/*Platform has
                                        ACE_HAS_4_4BSD_SENDMSG_RECVMSG but its
                                        cmsghdr structure does not contain
                                        an 'unsigned char cmsg_data[0]'
                                        member.  (This may be
                                        'unsigned char __cmsg_data[0]' on some
                                        platforms, in which case we need
                                        another macro.)*/
//#define ACE_LACKS_COND_TIMEDWAIT_RESET
					/*pthread_cond_timedwait does
                                        *not* reset the time argument
                                        when the lock is acquired.*/
//#define ACE_LACKS_CONST_STRBUF_PTR
					/*Platform uses struct strbuf *
                                        rather than const struct
                                        strbuf * (e.g., HP/UX 10.x)*/
//#define ACE_LACKS_CONST_TIMESPEC_PTR 1
					/*Platform forgot const in
                                        cond_timewait (e.g., HP/UX).*/
//#define ACE_LACKS_COND_T
					/*Platform lacks condition
                                        variables (e.g., Win32 and
                                        VxWorks)*/
//#define ACE_LACKS_CONDATTR_PSHARED
					/*Platform has no implementation
                                        of
                                        pthread_condattr_setpshared(),
                                        even though it supports
                                        pthreads!*/
//#define ACE_LACKS_DIFFTIME
					/*Platform lacks difftime() implementation*/
//#define ACE_LACKS_FCNTL
					/*Platform lacks POSIX-style fcntl ().*/
//#define ACE_LACKS_FSYNC
					/*Platform lacks fsync().*/
//#define ACE_LACKS_INLINE_FUNCTIONS
					/*Platform can't handle "inline"
                                        keyword correctly.*/
//#define ACE_LACKS_EXEC
					/*Platform lacks the exec()
                                        family of system calls (e.g.,
                                        Win32, VxWorks, Chorus)*/
//#define ACE_LACKS_FILELOCKS 1
					/*Platform lacks file locking
                                        mechanism*/
//#define ACE_LACKS_FLOATING_POINT
					/*Platform does not support
                                        floating point operations
                                        (e.g., certain Chorus hardware
                                        platforms)*/
//#define ACE_LACKS_FORK 1
					/*Platform lacks the fork()
                                        system call (e.g., Win32,
                                        VxWorks, Chorus)*/
//#define ACE_LACKS_GETOPT_PROTO
					/*Platform lacks the getopt()
                                        prototype (e.g., LynxOS)*/
//#define ACE_LACKS_GETPGID 1
					/*Platform lacks getpgid() call
                                        (e.g., Win32, Chorus, and
                                        FreeBSD).*/
//#define ACE_LACKS_GETSERVBYNAME
					/*Platforms lacks
                                        getservbyname() (e.g., VxWorks
                                        and Chorus).*/
//#define ACE_LACKS_INET_ATON
					/*Platform lacks the inet_aton()
                                        function.*/
//#define ACE_LACKS_IOSTREAMS_TOTALLY
					/*Iostreams are not supported
                                        adequately on the given platform.*/
//#define ACE_LACKS_IOSTREAM_FX
					/*iostream header does not
                                        declare ipfx (), opfx (),
                                        etc.*/
//#define ACE_LACKS_KEY_T
					/*Platform lacks key_t (e.g.,
                                        Chorus, VxWorks, Win32)*/
//#define ACE_LACKS_LINEBUFFERED_STREAMBUF 1
					/*Platform lacks streambuf
                                        "linebuffered ()".*/
//#define ACE_LACKS_LONGLONG_T
					/*Compiler/platform does no
                                        supports the unsigned long
                                        long datatype.*/
#define ACE_LACKS_LSTAT 1
					/*Platform lacks the lstat() function.*/
#define ACE_LACKS_MADVISE 1
					/*Platform lacks madvise()
                                        (e.g., Linux)*/
#define ACE_LACKS_MALLOC_H 1
					/*Platform lacks malloc.h*/
//#define ACE_LACKS_MEMORY_H
					/*Platform lacks memory.h (e.g.,
                                        VxWorks and Chorus)*/
#define ACE_LACKS_MKFIFO 1
					/*Platform lacks mkfifo() e.g.,
                                        VxWorks, Chorus, pSoS, and WinNT.*/
//#define ACE_LACKS_MKTEMP
					/*ACE has no mktemp()*/
//#define ACE_LACKS_MMAP
					/*The platform doesn't have
                                        mmap(2) (e.g., SCO UNIX).*/
//#define ACE_LACKS_MODE_MASKS
					/*Platform/compiler doesn't have
                                        open() mode masks.*/
//#define ACE_LACKS_MPROTECT
					/*The platform doesn't have
                                        mprotect(2) (e.g., EPLX real
                                        time OS from CDC (based on
                                        LYNX))*/
//#define ACE_LACKS_MSG_ACCRIGHTS
					/*Platform defines ACE_HAS_MSG,
                                        but lacks msg_accrights{,len}.*/
//#define ACE_LACKS_MSG_WFMO
					/*Platform lacks
                                        MsgWaitForMultipleObjects
                                        (only needs to be defined when
                                        ACE_WIN32 is also defined).*/
//#define ACE_LACKS_MSGBUF_T 1
					/*Platform lacks struct msgbuf
                                        (e.g., NT and MSV).*/
//#define ACE_LACKS_MSYNC
					/*Platform lacks msync() (e.g.,
                                        Linux)*/
//#define ACE_LACKS_MUTEXATTR_PSHARED 1
					/*Platform lacks
                                        pthread_mutexattr_setpshared().*/
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
//#define ACE_LACKS_NEW_H
					/*OS doesn't have, or we don't want to
                                        use, new.h.*/
//#define ACE_LACKS_NULL_PTHREAD_STATUS
					/*OS requires non-null status pointer
                                        for ::pthread_join ().*/
#define ACE_LACKS_PARAM_H 1
					/*Platform lacks <sys/param.h>
                                        (e.g., MVS)*/
//#define ACE_LACKS_PERFECT_MULTICAST_FILTERING
					/*Platform lacks IGMPv3 "perfect" filtering
                                        of multicast dgrams at the socket level.
                                        If == 1, ACE_SOCK_Dgram_Mcast will bind
                                        the first joined multicast group to the
                                        socket, and all future joins on that
                                        socket will fail with an error.*/
//#define ACE_LACKS_POSIX_PROTOTYPES 1
					/*Platform lacks POSIX
                                        prototypes for certain System
                                        V functions like shared memory
                                        and message queues.*/
//#define ACE_LACKS_PRAGMA_ONCE
					/*Compiler complains about #pragma once*/
#define ACE_LACKS_PRI_T 1
					/*Platform lacks pri_t (e.g.,
                                        Tandem NonStop UNIX).*/
//#define ACE_LACKS_PTHREAD_CANCEL
					/*Platform lacks
                                        pthread_cancel().*/
#define ACE_LACKS_PTHREAD_SIGMASK 1
					/*Platform lacks pthread_sigmask ().*/
#define ACE_LACKS_PTHREAD_THR_SIGSETMASK 1
					/*Platform lacks
                                        pthread_thr_sigsetmask (e.g.,
                                        MVS, HP/UX, and OSF/1 3.2)*/
//#define ACE_LACKS_PWD_REENTRANT_FUNCTIONS 1
					/*Platform lacks getpwnam_r()
                                        methods (e.g., SGI 6.2).*/
//#define ACE_LACKS_QSORT
					/*Compiler/platform lacks the
                                        standard C library qsort()
                                        function*/
//#define ACE_LACKS_RAND_REENTRANT_FUNCTIONS 1
					/*Platform lacks rand_r()*/
#define ACE_LACKS_READLINK 1
					/*Platform lacks the readlink() function.*/
//#define ACE_LACKS_READV 1
					/*Platform doesn't define readv,
                                        so use our own*/
//#define ACE_LACKS_RENAME
					/*Platform lacks rename().*/
#define ACE_LACKS_RLIMIT 1
					/*Platform/compiler lacks
                                        {get,set}rlimit() function
                                        (e.g., VxWorks, Chorus, and
                                        SCO UNIX)*/
#define ACE_LACKS_RLIMIT_PROTOTYPE 1
					/*Platform/compiler lacks
                                        {get,set}rlimit() prototypes
                                        (e.g., Tandem)*/
//#define ACE_LACKS_RTTI
					/*Compiler does not support
                                          dynamic_cast.*/
//#define ACE_LACKS_READDIR_R
					/*Platform uses ACE_HAS_DIRENT
                                        but does not have readdir_r
                                        ().*/
//#define ACE_LACKS_RECVMSG 1
					/*Platform lacks recvmsg()
                                        (e.g., Linux)*/
//#define ACE_LACKS_RWLOCK_T 1
					/*Platform lacks readers/writer
                                        locks.*/
//#define ACE_LACKS_SBRK
					/*Platform lacks a working
                                        sbrk() (e.g., Win32 and
                                        VxWorks)*/
//#define ACE_LACKS_SEEKDIR
					/*Platform uses ACE_HAS_DIRENT
                                        but does not have seekdir ().*/
#define ACE_LACKS_SEMBUF_T 1
					/*Platform lacks struct sembuf
                                        (e.g., Win32 and VxWorks)*/
//#define ACE_LACKS_SETDETACH
					/*Platform lacks
                                        pthread_attr_setdetachstate()
                                        (e.g., HP/UX 10.x)*/
#define ACE_LACKS_SETSCHED
					/*Platform lacks
                                        pthread_attr_setsched()
                                        (e.g. MVS)*/
//#define ACE_LACKS_SIGACTION
					/*Platform lacks struct
                                        sigaction (e.g., Win32 and
                                        Chorus)*/
//#define ACE_LACKS_SIGNED_CHAR
					/*Platform lacks "signed char"
                                        type (broken!)*/
//#define ACE_LACKS_SIGSET
					/*Platform lacks signal sets
                                        (e.g., Chorus and Win32)*/
#define ACE_LACKS_SOME_POSIX_PROTOTYPES 1
					/*Platform lacks POSIX
                                        prototypes for certain System
                                        V functions like shared memory
                                        and message queues.*/
//#define ACE_LACKS_NATIVE_STRPTIME
					/*Platform/compiler lacks the strptime()
                                        function.*/
//#define ACE_LACKS_STRRCHR
					/*Platform/compiler lacks
                                        strrchr () function.*/
//#define ACE_LACKS_WCSRCHR
					/*Platform/compiler lacks wcsrchr ()
                                        function*/
//#define ACE_LACKS_SYS_NERR
					/*Platforms/compiler lacks the
                                        sys_nerr variable (e.g.,
                                        VxWorks and MVS).*/
//#define ACE_LACKS_SYSTIME_H
					/*<time.h> doesn't automatically
                                        #include <sys/time.h>*/
//#define ACE_LACKS_SYSV_MSG_H 1
					/*Platform lacks sys/msg.h
                                        (e.g., Chorus and VxWorks)*/
//#define ACE_LACKS_SENDMSG
					/*Platform lacks sendmsg()
                                        (e.g., Linux)*/
//#define ACE_LACKS_SI_ADDR
					/*Platform lacks the si_addr
                                        field of siginfo_t (e.g.,
                                        VxWorks and HP/UX 10.x)*/
#define ACE_LACKS_SYSV_SHMEM 1
					/*Platform lacks System V shared
                                        memory (e.g., Win32 and
                                        VxWorks)*/
#define ACE_LACKS_SIGINFO_H 1
					/*Platform lacks the siginfo.h
                                        include file (e.g., MVS)*/
//#define ACE_LACKS_SOCKET_BUFSIZ
					/*Platform doesn't support
                                        SO_SNDBUF/SO_RCVBUF*/
#define ACE_LACKS_SOCKETPAIR 1
					/*Platform lacks the
                                        socketpair() call (e.g., SCO
                                        UNIX)*/
//#define ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES
					/*Compiler doesn't support
                                        static data member templates*/
//#define ACE_LACKS_STRCASECMP
					/*Compiler/platform lacks
                                        strcasecmp() (e.g., DG/UX,
                                        UNIXWARE, VXWORKS)*/
#define ACE_LACKS_STRRECVFD 1
					/*Platform doesn't define struct
                                        strrecvfd.*/
#define ACE_LACKS_SYSCALL 1
					/*Platform doesn't have
                                        syscall() prototype*/
#define ACE_LACKS_T_ERRNO 1
					/*Header files lack t_errno for
                                        TLI*/
//#define ACE_LACKS_TCP_H
					/*Platform doesn't have
                                        netinet/tcp.h*/
//#define ACE_LACKS_TCP_NODELAY
					/*OS does not support TCP_NODELAY.*/
//#define ACE_LACKS_TELLDIR
					/*Platform uses ACE_HAS_DIRENT
                                        but does not have telldir ().*/
//#define ACE_LACKS_THREAD_STACK_SIZE
					/*Platform lacks
                                        pthread_attr_setstacksize()
                                        (e.g., Linux pthreads)*/
//#define ACE_LACKS_TIMEDWAIT_PROTOTYPES
					/*MIT pthreads platform lacks
                                        the timedwait prototypes*/
//#define ACE_LACKS_TIMESPEC_T
					/*Platform does not define
                                        timepec_t as a typedef for
                                        struct timespec.*/
//#define ACE_LACKS_TRUNCATE
					/*Platform doesn't have truncate()
                                        (e.g., vxworks)*/
//#define ACE_LACKS_U_LONGLONG_T
					/*Platform does not have
                                        u_longlong_t typedef, and
                                        "sun" is defined.*/
//#define ACE_LACKS_UALARM_PROTOTYPE
					/*Platform/compiler lacks the
                                        ualarm() prototype (e.g.,
                                        Solaris)*/
//#define ACE_LACKS_CHAR_RIGHT_SHIFTS
					/*Compiler does not have any istream
                                        operator>> for chars, u_chars, or
                                        signed chars.*/
//#define ACE_LACKS_CHAR_STAR_RIGHT_SHIFTS
					/*Compiler does not have
                                        operator>> (istream &, u_char *) or
                                        operator>> (istream &, signed char *)*/
#define ACE_LACKS_UCONTEXT_H 1
					/*Platform lacks the ucontext.h
                                        file*/
//#define ACE_LACKS_UNBUFFERED_STREAMBUF
					/*Platform lacks streambuf
                                        "unbuffered ()".*/
//#define ACE_LACKS_UNISTD_H
					/*Platform lacks the unistd.h
                                        file (e.g., VxWorks and Win32)*/
#define ACE_LACKS_UNIX_DOMAIN_SOCKETS 1
					/*ACE platform has no UNIX
                                        domain sockets*/
//#define ACE_LACKS_UNIX_SIGNALS
					/*Platform lacks full signal
                                        support (e.g., Win32 and
                                        Chorus).*/
#define ACE_LACKS_UNIX_SYSLOG 1

//#define ACE_LACKS_UTSNAME_T
					/*Platform lacks struct utsname
                                        (e.g., Win32 and VxWorks)*/
//#define ACE_LACKS_WILDCARD_BIND
					/*The bind() call will not
                                        select the port if it's 0.*/
//#define ACE_LACKS_WRITEV
					/*Platform doesn't define
                                        writev, so use our own*/
//#define ACE_LEGACY_MODE
					/*When defined, it will enable some code that is
                                        used to provide some support for backwards
                                        compatibility.*/
//#define ACE_NEEDS_DEV_IO_CONVERSION
					/*Necessary with some compilers
                                        to pass ACE_TTY_IO as
                                        parameter to DEV_Connector.*/
//#define ACE_NEEDS_FUNC_DEFINITIONS
					/*Compiler requires a definition
                                        for a "hidden" function, e.g.,
                                        a private, unimplemented copy
                                        constructor or assignment
                                        operator.  The SGI C++
                                        compiler needs this, in
                                        template classes, with
                                        ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA.*/
#define ACE_NEEDS_HUGE_THREAD_STACKSIZE (64U*1024)
					/*Required by platforms with small default stacks.*/
//#define ACE_NEEDS_LWP_PRIO_SET
					/*OS has LWPs, and when the
                                        priority of a bound thread is
                                        set, then the LWP priority
                                        must be set also.*/
//#define ACE_NEEDS_SCHED_H
					/*Platform needs to #include
                                        <sched.h>
                                        to get thread scheduling
                                        defs.*/
//#define ACE_NO_WIN32_LEAN_AND_MEAN
					/*If this is set, then ACE does not
                                        define WIN32_LEAN_AND_MEAN before
                                        including <windows.h>. Needed for
                                        code that uses non-lean Win32
                                        facilities such as COM.*/
//#define ACE_ONLY_LATEST_AND_GREATEST
					/*A macro that indicates that the "latest and greatest"
                                        features of ACE/TAO should be turned on.  It has been
                                        replaced by ACE_LEGACY_MODE, which has the opposite
                                        meaning but serves the same purpose.*/
//#define ACE_WSTRING_HAS_USHORT_SUPPORT
					/*If a platform has wchar_t as a separate type,
                                        then ACE_WString doesn't have a constructor that
                                        understands an ACE_USHORT16 string.  So this
                                        macro enables one. (mostly used my ACE Name Space).*/
//#define ACE_HAS_BROKEN_PREALLOCATED_OBJECTS_AFTER_FORK
					/*
					Under QNX/RTP the objects preallocated
					in ACE_OS_Object_Manager cannot be
					destroyed after a fork() call.
					Since these objects are only destroyed
					at application shutdown we take the
					simpler approach of not destroying
					them at all.
					Both QNX/RTP and LynxOS suffer from
					this problem.

                            --------------------------------

The following macros determine the svc.conf file format ACE uses.

Macro                                   Description
-----                                   -----------*/
//#define ACE_HAS_CLASSIC_SVC_CONF
					/*This macro forces ACE to use the classic
                                        svc.conf format.*/
//#define ACE_HAS_XML_SVC_CONF
					/*This macro forces ACE to use the XML
                                        svc.conf format.*/
//#define ACE_USES_CLASSIC_SVC_CONF
					/*This macro should be defined as 0 or 1, depending
                                        on the preferred svc.conf file format.
                                        Defining this macro to 0 means ACE will use XML
                                        svc.conf file format.  Defining it to 1 will
                                        force ACE to use the classic svc.conf format.
                                        ** This macro takes precedence over previous
                                        ** two macros. */
#endif
