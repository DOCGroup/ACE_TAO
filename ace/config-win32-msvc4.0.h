/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for Windows NT
// platforms using the Microsoft Visual C++ 4.0 compiler.

#if !defined (ACE_CONFIG_H)
#define ACE_CONFIG_H

// Needed for timeval.
#include <winsock.h> 

#if defined (_MSC_VER)
// "C4355: 'this' : used in base member initializer list"
#pragma warning(disable:4355) // disable C4514 warning
// #pragma warning(default:4355)   // use this to reenable, if desired
#endif /* _MSC_VER */
// While digging the MSVC 4.0 include files, I found how to disable MSVC 
// warnings:
// --Amos Shapira

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

#ifdef _AFXDLL        // May be defined by MSVC++ IDE
#include <afxwin.h>   // He is doing MFC
#define	_INC_WINDOWS  // Prevent winsock.h from including windows.h
#endif
#ifdef _WINDLL        // May be defined by MSVC++ IDE
#include <afxwin.h>   // He is doing MFC
#define	_INC_WINDOWS  // Prevent winsock.h from including windows.h
#endif

#ifndef __AFX_H__     // set in afxwin.h
#include <windows.h>  // if he's not doing MFC, snag this
#endif

// Needed for timeval.
#include <winsock.h> 

#define ACE_HAS_UNICODE

// Uncomment these if you want to integrate ACE and Orbix in Win32.
// #define ACE_HAS_ORBIX
// #define ACE_HAS_MT_ORBIX

#define ACE_HAS_TEMPLATE_TYPEDEFS
#define ACE_LACKS_SBRK
#define ACE_LACKS_UTSNAME_T
#define ACE_LACKS_SEMBUF_T
#define ACE_LACKS_MSGBUF_T
#define ACE_LACKS_SYSV_SHMEM

// Build as as a DLL.  Zap this line if you want to build a static
// lib.
#define ACE_HAS_DLL

// Compiler/platform correctly calls init()/fini() for shared
// libraries. - applied for DLLs ?
//define ACE_HAS_AUTOMATIC_INIT_FINI

// Compiler doesn't support static data member templates.
// #define ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES

// Version 1.1 of WinSock
#define ACE_WSOCK_VERSION 1, 1
#define ACE_LACKS_RECVMSG
#define ACE_LACKS_SENDMSG

// Platform supports POSIX O_NONBLOCK semantics.
//define ACE_HAS_POSIX_NONBLOCK

#define ACE_LACKS_MODE_MASKS
#define ACE_LACKS_STRRECVFD

// Compiler/platform has correctly prototyped header files.
#define ACE_HAS_CPLUSPLUS_HEADERS

// Platform supports IP multicast
#define ACE_HAS_IP_MULTICAST

// Sockets may be called in multi-threaded programs.
#define ACE_HAS_MT_SAFE_SOCKETS

// Platform contains <poll.h>.
//define ACE_HAS_POLL

// Platform supports POSIX timers via timestruc_t.
//define ACE_HAS_POSIX_TIME

// Platform supports the /proc file system.
//define ACE_HAS_PROC_FS

// Platform supports the rusage struct.
#define ACE_HAS_GETRUSAGE

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// Andreas Ueltschi tells me this is a good thing...
#define ACE_HAS_SVR5_GETTIMEOFDAY

// Compiler/platform supports poll().
//define ACE_HAS_SVR4_POLL

// Compiler/platform supports SVR4 signal typedef.
//define ACE_HAS_SVR4_SIGNAL_T

// Platform provides <sys/filio.h> header.
//define ACE_HAS_SYS_FILIO_H

// Compiler/platform supports sys_siglist array.
//define ACE_HAS_SYS_SIGLIST

/* Turn off the following four defines if you want to disable threading. */
// Compile using multi-thread libraries.
#define ACE_MT_SAFE

// Platform supports threads.
#define ACE_HAS_THREADS

// Platform supports Windows32 threads.
#define ACE_HAS_WTHREADS

// Compiler/platform has thread-specific storage
#define ACE_HAS_THREAD_SPECIFIC_STORAGE

#define ACE_WIN32
#define ACE_HAS_TEMPLATE_INSTANTIATION

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

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */
// #define ACE_NLOGGING

// Defines the page size of the system.
#define ACE_PAGE_SIZE 4096

// I'm pretty sure NT lacks these
#define ACE_LACKS_UNIX_DOMAIN_SOCKETS

// Windows NT needs readv() and writev()
#define ACE_NEEDS_WRITEV
#define ACE_NEEDS_READV

// Comment this out for now since it will break existing application
// code.
#define ACE_HAS_STRICT
#endif /* ACE_CONFIG_H */
