/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for HP
// platforms running HP-UX 11.00 using aC++, CC, or gcc (2.95 and up).

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H
#include "ace/pre.h"

#if defined (__GNUG__)

// config-g++-common.h undef's ACE_HAS_STRING_CLASS with -frepo, so
// this must appear before its #include.
#  define ACE_HAS_STRING_CLASS

#  include "ace/config-g++-common.h"

// gcc 2.95.2 supplies the ssize_t typedef.
#  define ACE_HAS_SSIZE_T

// gcc 3.0 claims to have wide character stuff, but (at least the version
// built by HP) can't actually compile it. It refers to a wctype.h file
// that's only available with aC++.

#else

// The following configuration section is designed to work for HP
// platforms running HP/UX 11.x with either of the HP C++ compilers.
// There isn't a predefined macro for all cases of the compilers that
// can be used to tell them apart from other compilers (e.g. __KCC, etc.)
// only to tell C++ from aC++, using the value of __cplusplus.
//
// NOTE - HP advises people on 11.x to use aC++ since the older C++ doesn't
// support 64-bit or kernel threads.  So, though this file has the C++ info
// in it, it's copied from the 10.x file and hasn't been verified.

// There are 2 compiler-specific sections, plus a 3rd for common to both.
// First is the HP C++ section...
#  if __cplusplus < 199707L

#    define ACE_HAS_BROKEN_HPUX_TEMPLATES

// Compiler can't handle calls like foo->operator T *()
#    define ACE_HAS_BROKEN_CONVERSIONS

// Necessary with some compilers to pass ACE_TTY_IO as parameter to
// DEV_Connector.
#    define ACE_NEEDS_DEV_IO_CONVERSION

// Compiler's template mechanism must see source code (i.e., .C files).
#    define ACE_TEMPLATES_REQUIRE_SOURCE

// Compiler's template mechanism requires the use of explicit C++
// specializations for all used templates.
#    define ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION

// The HP/UX compiler doesn't support volatile!!!!
#    define volatile

#  else  // aC++ definitions

// Parts of TAO (at least) use __HP_aCC to detect this compiler, but the
// macro is not set until A.03.13. If it's not set, set it - it won't be an
// HP-advertised value, but we don't check the value/version - just whether
// it's set or not.
#    if !defined (__HP_aCC)
#      define __HP_aCC
#    endif /* __HP_aCC */

// Precompiler needs extra flags to ignore "invalid #pragma directive"
#    define ACE_CC_PREPROCESSOR_ARGS "-E +W 67"

// Compiler supports ANSI casts
#    define ACE_HAS_ANSI_CASTS

// Compiler can't handle calls like foo->operator T *()
#    define ACE_HAS_BROKEN_CONVERSIONS

// Compiler supports C++ exception handling. However, the user can ask for this
// to be turned off. If so (using make exceptions=0) then this def is not set.
// By default, it is set in wrapper_macros.GNU.
// #    define ACE_HAS_EXCEPTIONS 1

// If the platform_macros.GNU file turned on ACE_HAS_STANDARD_CPP_LIBRARY
// then we're using the -AA option, so we have standard C++ library,
// including the standard iostreams. Else, we have the old iostreams.
#    if defined (ACE_HAS_STANDARD_CPP_LIBRARY)
#      define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 1
#    else
#      define ACE_USES_OLD_IOSTREAMS
#    endif /* ACE_HAS_STANDARD_CPP_LIBRARY */

#    define ACE_HAS_TYPENAME_KEYWORD

// Compiler implements templates that support typedefs inside of classes
// used as formal arguments to a template class.
#    define ACE_HAS_TEMPLATE_TYPEDEFS

// This is legit for A.03.05 - not sure A.03.04, but it should be.
#    define ACE_HAS_USING_KEYWORD

// Platform lacks streambuf "linebuffered ()".
#    define ACE_LACKS_LINEBUFFERED_STREAMBUF 1

// Lack of (and broken) support for placement operator delete is a known
// bug by HP, at least as of aC++ A.03.10. It may be fixed later, and if so
// this would change to be a #if against an appropriate value of __HP_aCC
#    define ACE_LACKS_PLACEMENT_OPERATOR_DELETE

// Compiler's 'new' throws exceptions on failure, regardless of whether or
// not exception handling is enabled in the compiler options. Fortunately,
// new(nothrow_t) is offered.
#    define ACE_NEW_THROWS_EXCEPTIONS
#    define ACE_HAS_NEW_NOTHROW

// Compiler's template mechanism must see source code (i.e., .C files).
#    define ACE_TEMPLATES_REQUIRE_SOURCE

// Compiler supports template specialization.
#    define ACE_HAS_TEMPLATE_SPECIALIZATION
// ... and uses the template<> syntax
#    define ACE_HAS_STD_TEMPLATE_SPECIALIZATION
#    define ACE_HAS_STD_TEMPLATE_METHOD_SPECIALIZATION

// Preprocessor needs some help with data types
#    if defined (__LP64__)
#      define ACE_SIZEOF_LONG 8
#    else
#      define ACE_SIZEOF_LONG 4
#    endif

// Platform has XPG4 wide character support
#    define ACE_HAS_XPG4_MULTIBYTE_CHAR
#    define ACE_HAS_WCHAR

#  endif /* __cplusplus < 199707L */

// Compiler supports the ssize_t typedef.
#  define ACE_HAS_SSIZE_T

// Compiler doesn't handle 'signed char' correctly (used in ace/IOStream.h)
#  define ACE_LACKS_SIGNED_CHAR

#endif /* __GNUG__, HP */

// KCC Specific Section
#if defined(__KCC)
#  include "ace/config-kcc-common.h"
#  undef ACE_HAS_STD_TEMPLATE_METHOD_SPECIALIZATION
#  undef ACE_CC_PREPROCESSOR_ARGS
#endif

//*********************************************************************
//
// From here down is the compiler-INdependent OS settings.
//
//*********************************************************************

// Compiling for HPUX.
#if !defined (HPUX)
#define HPUX
#endif /* HPUX */
#define HPUX_11

#ifndef _HPUX_SOURCE
#define _HPUX_SOURCE
#endif

#include /**/ <sys/stdsyms.h>

////////////////////////////////////////////////////////////////////////////
//
// General OS information - see README for more details on what they mean
//
///////////////////////////////////////////////////////////////////////////

// HP/UX needs to have these addresses in a special range.
// If this is on a 64-bit model, the default is to use 64-bit addressing.
// It can also be set so that the mapped region is shareable with 32-bit
// programs.  To enable the 32/64 sharing, comment out the first definition
// of ACE_DEFAULT_BASE_ADDR and uncomment the two lines after it.
// Note - there's a compiler bug on aC++ A.03.04 in 64-bit mode which prevents
// these from working as-is.  So, there's some hackery in Naming_Context.cpp
// and Memory_Pool.cpp which works around it.  It requires the
// ACE_DEFAULT_BASE_ADDRL definition below - make sure it has the same
// value as what you use for ACE_DEFAULT_BASE_ADDR.  This is allegedly fixed
// in A.03.10 on the June Applications CD.
#if defined (__LP64__)
#  define ACE_DEFAULT_BASE_ADDR ((char *) 0x0000001100000000)
//#  define ACE_DEFAULT_BASE_ADDR ((char *) 0x80000000)
//#  define ACE_OS_EXTRA_MMAP_FLAGS MAP_ADDR32

#  define ACE_DEFAULT_BASE_ADDRL (0x0000001100000000)
//#  define ACE_DEFAULT_BASE_ADDRL (0x80000000)
#else
#  define ACE_DEFAULT_BASE_ADDR ((char *) 0x80000000)
#endif  /* __LP64__ */

// Platform can do async I/O (aio_*)
#define ACE_HAS_AIO_CALLS
// ... but seems to require this in order to keep from hanging.  Needs some
// investigation, maybe with HP.  John Mulhern determined this value
// empirically.  YMMV.  If it does vary, set it up in your own config.h which
// then includes the ACE-supplied config.
#if !defined (ACE_INFINITE)
#  define ACE_INFINITE 10000000
#endif

// Manually tweak the malloc control block paddings to properly align
// things.
#define ACE_MALLOC_PADDING 16
#define ACE_MALLOC_ALIGN 8
#define ACE_PI_CONTROL_BLOCK_ALIGN_LONGS  3

// Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYSCALL_H
// But doesn't have a prototype for syscall()
#define ACE_LACKS_SYSCALL

// Platform supports POSIX 1.b clock_gettime ()
#define ACE_HAS_CLOCK_GETTIME

// Prototypes for both signal() and struct sigaction are consistent.
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

// Compiler/platform has correctly prototyped header files.
#define ACE_HAS_CPLUSPLUS_HEADERS

// Compiler/platform has Dirent iterator functions.
#define ACE_HAS_DIRENT

// Platform supports getpagesize() call
#define ACE_HAS_GETPAGESIZE
// But we define this just to be safe
#define ACE_PAGE_SIZE 4096

// Can run gperf on this platform (needed for TAO)
#  define ACE_HAS_GPERF

// Optimize ACE_Handle_Set for select().
#  define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

// Platform supports IP multicast
#define ACE_HAS_IP_MULTICAST

// Platform supports recvmsg and sendmsg.
#define ACE_HAS_MSG

// Platform's select() has non-const timeval argument
#define ACE_HAS_NONCONST_SELECT_TIMEVAL

// Compiler/platform supports poll().
#define ACE_HAS_POLL

// Platform supports POSIX O_NONBLOCK semantics.
#define ACE_HAS_POSIX_NONBLOCK

// Platform supports the POSIX struct timespec type
#define ACE_HAS_POSIX_TIME

// Platform supports reentrant functions (all the POSIX *_r functions).
#define ACE_HAS_REENTRANT_FUNCTIONS
// ctime_r and asctime_r conform to POSIX.1c (2 param version)
#define ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R

// HP-UX 11 has reentrant netdb functions.  The catch is that the old
// functions (gethostbyname, etc.) are thread-safe and the _r versions are
// not used and will be removed at some point.  So, define things so
// the _r versions are not used.  This will slow things down a bit due to
// the extra mutex lock in the ACE_NETDBCALL_RETURN macro, and will be fixed
// in the future (problem ID P64).
#define ACE_LACKS_NETDB_REENTRANT_FUNCTIONS

// Platform has shm_open
#define ACE_HAS_SHM_OPEN

// Compiler/platform defines the sig_atomic_t typedef
#define ACE_HAS_SIG_ATOMIC_T

// Platform supports SVR4 extended signals
#define ACE_HAS_SIGINFO_T

// Platform doesn't detect a signal out of range unless it's way out of range.
#define ACE_HAS_SIGISMEMBER_BUG

#define ACE_HAS_UALARM

// Platform supports ucontext_t (which is used in the extended signal API).
#define ACE_HAS_UCONTEXT_T

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// SunOS 4 style prototype for gettimeofday
#define ACE_HAS_SUNOS4_GETTIMEOFDAY

// Platform supports SVR4 dynamic linking semantics, in 64-bit mode only.
// When used, this requires -ldl on the ACE library link line.
#ifdef __LP64__
#define ACE_HAS_SVR4_DYNAMIC_LINKING
#endif

// HP/UX has an undefined syscall for GETRUSAGE...
#define ACE_HAS_SYSCALL_GETRUSAGE
// Note, this only works if the flag is set above!
#define ACE_HAS_GETRUSAGE

// Platform has the sigwait function in a header file
#define ACE_HAS_SIGWAIT
#define ACE_HAS_SIGTIMEDWAIT

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC

// accept() is thread-safe
#define ACE_HAS_THREAD_SAFE_ACCEPT

// Platform lacks a typedef for timespec_t, but has struct timespec
#define ACE_LACKS_TIMESPEC_T

// dlopen() takes a char* instead of const char*
#define ACE_HAS_CHARPTR_DL

// Shared library name/path components
#define ACE_DLL_SUFFIX  ".sl"
#if defined (__LP64__)
#  define ACE_LD_SEARCH_PATH "LD_LIBRARY_PATH"
#else
#  define ACE_LD_SEARCH_PATH "SHLIB_PATH"
#endif  /* __LP64__ */

//////////////////////////////////////////////////////////////////////////
//
// STREAMS information
//
//////////////////////////////////////////////////////////////////////////

// Platform supports STREAMS
#define ACE_HAS_STREAMS
// Compiler/platform supports struct strbuf.
#define ACE_HAS_STRBUF_T
// But the putmsg signature doesn't have it as const...
// Well, it really does, but it depends on preprocessor defines.
#define ACE_LACKS_CONST_STRBUF_PTR

// Platform supports STREAM pipes
// This is possible, but not by default - need to rebuild the kernel to
// get them enabled - see pipe(2) and "STREAMS/UX for the HP 9000"
// #define ACE_HAS_STREAM_PIPES

/////////////////////////////////////////////////////////////////////////
//
// TLI/XTI information
//
////////////////////////////////////////////////////////////////////////

// Platform supports XTI (includes TLI), including SVR4 facilities.
#define ACE_HAS_TLI
// ACE_HAS_SVR4_TLI should work on HP-UX, but doesn't yet.  Riverace
// problem ID P27.
//#define ACE_HAS_SVR4_TLI
// Platform uses <xti.h>, not tiuser.h
#define ACE_HAS_XTI
// But it has _terrno() outside the extern "C" stuff.
#define ACE_HAS_TIUSER_H_BROKEN_EXTERN_C
// Platform provides ACE_TLI function prototypes.
#define ACE_HAS_TLI_PROTOTYPES
// HP-UX 11.00 (at least at initial releases) has some busted macro defs
#define ACE_HAS_BROKEN_XTI_MACROS
// HP-UX 11 conforms to the XPG4 spec, which ACE calls broken for the
// errmsg not being const...
#define ACE_HAS_BROKEN_T_ERROR
// The definitions of TCP_NODELAY and TCP_MAXSEG conflict between
// sys/xti.h and netinet/tcp.h.
#define ACE_HAS_CONFLICTING_XTI_MACROS

/////////////////////////////////////////////////////////////////////////
//
// Threads information.
//
// Use of threads is controlled by the 'threads' argument to make.  See
// include/makeinclude/platform_hpux_aCC.GNU for details. If it's not set,
// the default is to enable it, since kernel threads are always available
// on HP-UX 11, as opposed to 10.x where it was optional software.
//
////////////////////////////////////////////////////////////////////////

#if defined (ACE_HAS_THREADS)
#  if (ACE_HAS_THREADS == 0)
#    undef ACE_HAS_THREADS
#  endif /* ACE_HAS_THREADS == 0 */
#else
#  define ACE_HAS_THREADS
#endif /* ACE_HAS_THREADS */

#if defined (ACE_HAS_THREADS)

#  if !defined (ACE_MT_SAFE)
#    define ACE_MT_SAFE 1
#  endif

#  define ACE_HAS_PTHREADS
#  define ACE_HAS_PTHREADS_STD
#  define ACE_HAS_PTHREADS_UNIX98_EXT

#  define ACE_HAS_THREAD_SPECIFIC_STORAGE
#endif /* ACE_HAS_THREADS */

#define ACE_HAS_POSIX_SEM
#define ACE_HAS_TERM_IOCTLS

// Turns off the tracing feature.
// To build with tracing enabled, make sure ACE_NTRACE is not defined
#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#include "ace/post.h"
#endif /* ACE_CONFIG_H */
