/* -*- C++ -*- */
// $Id$

// The following configuration file contains the defines
// common to all Win32/MSVC/MFC combinations.

#ifndef ACE_WIN32_COMMON_H
#define ACE_WIN32_COMMON_H

// Complain if WIN32 if not already defined.
#if !defined (WIN32) && !defined (ACE_HAS_WINCE)
#error Please define WIN32 in your project settings.
#endif /* WIN32 */

// ---------------- platform features or lack of them -------------

#if !defined (ACE_HAS_WINCE)
// Platform supports pread() and pwrite()
#define ACE_HAS_P_READ_WRITE
#define ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS
#endif /* ! ACE_HAS_WINCE */

#define ACE_HAS_RECURSIVE_MUTEXES
#define ACE_HAS_WCHAR_TYPEDEFS_USHORT
#define ACE_HAS_MSG
#define ACE_HAS_SOCKADDR_MSG_NAME
#define ACE_LACKS_GETPGID
#define ACE_HAS_THREAD_SAFE_ACCEPT
#define ACE_HAS_EXCEPTIONS
#define ACE_HAS_BROKEN_NAMESPACES
#define ACE_HAS_WORKING_EXPLICIT_TEMPLATE_DESTRUCTOR
#define ACE_LACKS_GETHOSTENT
#define ACE_LACKS_SIGACTION
#define ACE_LACKS_SIGSET
#define ACE_LACKS_FORK
#define ACE_LACKS_UNIX_SIGNALS
#define ACE_LACKS_SBRK
#define ACE_LACKS_UTSNAME_T
#define ACE_LACKS_SEMBUF_T
#define ACE_LACKS_MSGBUF_T
#define ACE_LACKS_SYSV_SHMEM
#define ACE_LACKS_UNISTD_H
#define ACE_LACKS_RLIMIT

#define ACE_SIZEOF_LONG_LONG 8
typedef unsigned __int64 ACE_UINT64;

#if defined (ACE_HAS_ANSI_CASTS) && (ACE_HAS_ANSI_CASTS == 0)
#undef  ACE_HAS_ANSI_CASTS
#endif /* ACE_HAS_ANSI_CASTS && ACE_HAS_ANSI_CASTS == 0 */

#if _MSC_VER < 1000
# define ACE_LACKS_PRAGMA_ONCE
#endif /* _MSC_VER < 1000 */

// Only MSVC 5.0 definitions
#if (_MSC_VER >= 1100)
  #if !defined (ACE_HAS_WINCE)
    #define ACE_HAS_SIG_ATOMIC_T
  #endif /* ACE_HAS_WINCE */
#define ACE_HAS_TYPENAME_KEYWORD
#endif /* _MSC_VER >= 1100 */

// Optimize ACE_Handle_Set for select().
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

// Win32 has UNICODE support
#define ACE_HAS_UNICODE

// Compiler/platform correctly calls init()/fini() for shared
// libraries. - applied for DLLs ?
//define ACE_HAS_AUTOMATIC_INIT_FINI

// Compiler doesn't support static data member templates.
#define ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES

// Platform supports POSIX O_NONBLOCK semantics.
//define ACE_HAS_POSIX_NONBLOCK

#define ACE_LACKS_MODE_MASKS
#define ACE_LACKS_STRRECVFD

// Compiler/platform has correctly prototyped header files.
#define ACE_HAS_CPLUSPLUS_HEADERS

// Platform supports IP multicast
#define ACE_HAS_IP_MULTICAST

// Platform contains <poll.h>.
//define ACE_HAS_POLL

// Platform supports POSIX timers via timestruc_t.
//define ACE_HAS_POSIX_TIME

// Platform supports the /proc file system.
//define ACE_HAS_PROC_FS

#if (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0))
// Platform supports the rusage struct.
#define ACE_HAS_GETRUSAGE
#endif /* (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) */

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// Andreas Ueltschi tells me this is a good thing...
#define ACE_HAS_SVR5_GETTIMEOFDAY

// Compiler/platform supports SVR4 signal typedef.
//define ACE_HAS_SVR4_SIGNAL_T

// Platform provides <sys/filio.h> header.
//define ACE_HAS_SYS_FILIO_H

// Compiler/platform supports sys_siglist array.
//define ACE_HAS_SYS_SIGLIST

#define ACE_WIN32

//#define ACE_HAS_ALLOC_HOOKS
#define ACE_TEMPLATES_REQUIRE_SOURCE

// Platform supports ACE_TLI timod STREAMS module.
//define ACE_HAS_TIMOD_H

// Platform supports ACE_TLI tiuser header.
//define ACE_HAS_TIUSER_H

// Platform provides ACE_TLI function prototypes.
// For Win32, this is not really true, but saves a lot of hassle!
#define ACE_HAS_TLI_PROTOTYPES
#define ACE_HAS_GNU_CSTRING_H
// Platform supports ACE_TLI.
//define ACE_HAS_TLI

// I'm pretty sure NT lacks these
#define ACE_LACKS_UNIX_DOMAIN_SOCKETS

// Windows NT needs readv() and writev()
#define ACE_LACKS_WRITEV
#define ACE_LACKS_READV

#define ACE_LACKS_COND_T
#define ACE_LACKS_RWLOCK_T

#define ACE_LACKS_KEY_T

// Platform support for non-blocking connects is broken
#define ACE_HAS_BROKEN_NON_BLOCKING_CONNECTS

// Platform support linebuffered streaming is broken
#define ACE_LACKS_LINEBUFFERED_STREAMBUF

// Template specialization is supported
#define ACE_HAS_TEMPLATE_SPECIALIZATION

// No system support for replacing any previous mappings.
#define ACE_LACKS_AUTO_MMAP_REPLACEMENT

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
        #define ACE_NTRACE 1
#endif /* ACE_NTRACE */
// #define ACE_NLOGGING

// ----------------- "derived" defines and includes -----------

#if defined (ACE_HAS_STANDARD_CPP_LIBRARY) && (ACE_HAS_STANDARD_CPP_LIBRARY != 0)
        #if defined (_MSC_VER)
                #if (_MSC_VER > 1020)
                        // Platform has its Standard C++ library in the namespace std
                        #if !defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB)
                                    #define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB       1
                        #endif /* ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB */
                #else /* (_MSC_VER > 1020) */
                        #if defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB)
                                #undef ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB
                        #endif /* ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB */
                        #define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 0
                #endif /* (_MSC_VER > 1020) */
        #endif /* _MSC_VER */

        // ace/iostream.h does not work with the standard cpp library (yet).
        #if !defined (ACE_USES_OLD_IOSTREAMS)
                #define ACE_LACKS_ACE_IOSTREAM
        #endif /* ! ACE_USES_OLD_IOSTREAMS */
#else
        // iostream header lacks ipfx (), isfx (), etc., declarations
        #define ACE_LACKS_IOSTREAM_FX
#endif

#if defined (_MSC_VER)
        // While digging the MSVC 4.0 include files, I found how to disable
        // MSVC warnings: --Amos Shapira

        // "C4355: 'this' : used in base member initializer list"
        #pragma warning(disable:4355) /* disable C4514 warning */
        //      #pragma warning(default:4355)   // use this to reenable, if desired

        #pragma warning(disable:4201)  /* winnt.h uses nameless structs */

        #pragma warning(disable:4231)
        // Disable warning of using Microsoft Extension.

        // MSVC 4.0 or greater
        #if (_MSC_VER >= 1000)
                // Compiler/Platform supports the "using" keyword.
                #define ACE_HAS_USING_KEYWORD
        #endif

        // MSVC 2.2 or lower
        #if (_MSC_VER < 1000)
                // This needs to be here since MSVC++ 2.0 seems to have forgotten to
                // include it. Does anybody know which MSC_VER MSVC 2.0 has ?
                inline void *operator new (unsigned int, void *p) { return p; }
        #endif
#endif /* _MSC_VER */

// MFC itself defines STRICT.
#if defined( ACE_HAS_MFC ) && (ACE_HAS_MFC != 0)
        #if !defined(ACE_HAS_STRICT)
                #define ACE_HAS_STRICT 1
        #endif
        #if (ACE_HAS_STRICT != 1)
                #undef ACE_HAS_STRICT
                #define ACE_HAS_STRICT 1
        #endif
#endif

// If you want to use highres timers, ensure that
// Build.Settings.C++.CodeGeneration.Processor is
// set to Pentium !
#if (_M_IX86 > 400)
        // Used for high resolution timers
        #define ACE_HAS_PENTIUM
#endif

#if defined(ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
        // Platform supports threads.
        #define ACE_HAS_THREADS

        // Platform supports Windows32 threads.
        #define ACE_HAS_WTHREADS

        // Compiler/platform has thread-specific storage
        #define ACE_HAS_THREAD_SPECIFIC_STORAGE

        // must have _MT defined to include multithreading
        // features from win32 headers
        #if !defined(_MT)
                #error You must link ACE against multi-threaded libraries.
        #endif /* _MT */
#endif /* ACE_MT_SAFE && ACE_MT_SAFE != 0 */

#if !defined (ACE_HAS_WINCE)
#if defined(ACE_HAS_DLL) && (ACE_HAS_DLL != 0)
        #if !defined(_DLL)
                #error You must link against (Debug) Multithreaded DLL run-time libraries.
        #endif /* !_DLL */
#endif  /* ACE_HAS_DLL && ACE_HAS_DLL != 0 */
#endif /* ACE_HAS_WINCE */

// We are using STL's min and max (in algobase.h).  Therefore the
// macros in window.h are extra
#if !defined (NOMINMAX)
        #define NOMINMAX
#endif /* NOMINMAX */

#if defined (_UNICODE)
        #if !defined (UNICODE)
                 #define UNICODE         /* UNICODE is used by Windows headers */
        #endif /* UNICODE */
#endif /* _UNICODE */

#if defined (UNICODE)
        #if !defined (_UNICODE)
                 #define _UNICODE        /* _UNICODE is used by C-runtime/MFC headers */
        #endif /* _UNICODE */
#endif /* UNICODE */

// If __ACE_INLINE__ is defined to be 0, we will undefine it
#if defined (__ACE_INLINE__) && (__ACE_INLINE__ == 0)
        #undef __ACE_INLINE__
#endif /* __ACE_INLINE__ */

#ifdef _DEBUG
#if !defined (ACE_HAS_WINCE)
        #include /**/ <crtdbg.h>
#endif /* ACE_HAS_WINCE */
#else
        // If we are making a release, and the user has not specified
        // inline directives, we will default to inline
        #if ! defined (__ACE_INLINE__)
                #define __ACE_INLINE__
        #endif /* __ACE_INLINE__ */
#endif

// We are build ACE and want to use MFC (multithreaded)
#if defined(ACE_HAS_MFC) && (ACE_HAS_MFC != 0) && defined (_MT)
        #if (ACE_HAS_DLL != 0) && defined(ACE_BUILD_DLL) && !defined (_WINDLL)
                // force multithreaded MFC DLL
                #define _WINDLL
        #endif /* _AFXDLL */
        #if !defined (_AFXDLL) && !defined (ACE_USES_STATIC_MFC)
                #define _AFXDLL
        #endif /* _AFXDLL */
#endif

// <windows.h> and MFC's <afxwin.h> are mutually
// incompatible. <windows.h> is brain-dead about MFC; it doesn't check
// to see whether MFC stuff is anticipated or already in progress
// before doing its thing. ACE needs (practically always) <winsock.h>,
// and winsock in turn needs support either from windows.h or from
// afxwin.h. One or the other, not both.
//
// The MSVC++ V4.0 environment defines preprocessor macros that
// indicate the programmer has chosen something from the
// Build-->Settings-->General-->MFC combo-box. <afxwin.h> defines a
// macro itself to protect against double inclusion. We'll take
// advantage of all this to select the proper support for winsock. -
// trl 26-July-1996

// This is necessary since MFC users apparently can't #include
// <windows.h> directly.
#if defined (ACE_HAS_MFC) && (ACE_HAS_MFC != 0)
        #include /**/ <afxwin.h>   /* He is doing MFC */
                // Windows.h will be included via afxwin.h->afx.h->afx_ver_.h->afxv_w32.h
                // #define      _INC_WINDOWS  // Prevent winsock.h from including windows.h
#endif

#if !defined (_INC_WINDOWS)     /* Already include windows.h ? */
        // Must define strict before including windows.h !
        #if defined (ACE_HAS_STRICT) && (ACE_HAS_STRICT != 0) && !defined (STRICT)
                #define STRICT 1
        #endif /* ACE_HAS_STRICT */

        #if !defined (WIN32_LEAN_AND_MEAN)
                #define WIN32_LEAN_AND_MEAN
        #endif /* WIN32_LEAN_AND_MEAN */

        #if defined (_UNICODE)
                #if !defined (UNICODE)
                        #define UNICODE         /* UNICODE is used by Windows headers */
                #endif /* UNICODE */
        #endif /* _UNICODE */

        #if defined (UNICODE)
                #if !defined (_UNICODE)
                        #define _UNICODE        /* _UNICODE is used by C-runtime/MFC headers */
                #endif /* _UNICODE */
        #endif /* UNICODE */


#endif /* !defined (_INC_WINDOWS) */

// Always use WS2 when available
#if (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0))
        #if !defined(ACE_HAS_WINSOCK2)
                #define ACE_HAS_WINSOCK2 1
        #endif /* !defined(ACE_HAS_WINSOCK2) */
#endif /* (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) */

#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
        #if !defined (_WINSOCK2API_)
                // will also include windows.h, if not present
                #include /**/ <winsock2.h>
        #endif /* _WINSOCK2API */

        #if !defined _MSWSOCK_
                #include /**/ <mswsock.h>
        #endif /* _MSWSOCK_ */

        #if defined (_MSC_VER)
                #pragma comment(lib, "ws2_32.lib")
                #pragma comment(lib, "mswsock.lib")
        #endif /* _MSC_VER */

        #define ACE_WSOCK_VERSION 2, 0
#else
        #if !defined (_WINSOCKAPI_)
                // will also include windows.h, if not present
                #include /**/ <winsock.h>
        #endif /* _WINSOCKAPI */

        #if defined (_MSC_VER) && !defined (UNDER_CE)
                #pragma comment(lib, "wsock32.lib")
        #endif /* _MSC_VER */

        // We can't use recvmsg and sendmsg unless WinSock 2 is available
        #define ACE_LACKS_RECVMSG
        #define ACE_LACKS_SENDMSG

        // Version 1.1 of WinSock
        #define ACE_WSOCK_VERSION 1, 1
#endif /* ACE_HAS_WINSOCK2 */

#if defined (_MSC_VER)
        #pragma warning(default: 4201)  /* winnt.h uses nameless structs */
#endif /* _MSC_VER */

#if (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) && !defined (ACE_USES_WINCE_SEMA_SIMULATION)
        #define ACE_HAS_INTERLOCKED_EXCHANGEADD
        #define ACE_HAS_WIN32_TRYLOCK
        #define ACE_HAS_SIGNAL_OBJECT_AND_WAIT

        // If CancelIO is undefined get the updated sp2-sdk
        // from MS
        #define ACE_HAS_CANCEL_IO
#endif /* (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) && !defined (ACE_USES_WINCE_SEMA_SIMULATION) */

#if !defined (ACE_SEH_DEFAULT_EXCEPTION_HANDLING_ACTION)
#define ACE_SEH_DEFAULT_EXCEPTION_HANDLING_ACTION EXCEPTION_CONTINUE_SEARCH
#endif /* ACE_SEH_DEFAULT_EXCEPTION_HANDLING_ACTION */

// At least for Win32 - MSVC compiler (ver. 5)
#define ACE_UINT64_FORMAT_SPECIFIER "%I64u"

/*
 * Borland Compilers.
 */
#include <ace/config-win32-borland.h>

#endif /* ACE_WIN32_COMMON_H */
