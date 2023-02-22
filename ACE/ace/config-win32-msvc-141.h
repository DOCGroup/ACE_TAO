/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   config-win32-msvc-141.h
 *
 *  @brief  Microsoft Visual C++ 14.1 configuration file.
 *
 *  This file is the ACE configuration file for Microsoft Visual C++ 14.1 (as released with Visual Studio 2017).
 *
 *  @note Do not include this file directly, include config-win32.h instead.
 */
//=============================================================================

#ifndef ACE_CONFIG_WIN32_MSVC_141_H
#define ACE_CONFIG_WIN32_MSVC_141_H
#include /**/ "ace/pre.h"

#ifndef ACE_CONFIG_WIN32_H
#error Use config-win32.h in config.h instead of this header
#endif /* ACE_CONFIG_WIN32_H */

#ifndef ACE_WIN32_VC14
#  define ACE_WIN32_VC14
#endif
#ifndef ACE_WIN32_VC141
#  define ACE_WIN32_VC141
#endif

// Windows' timeval is non-conformant (defined in terms of long instead of
// time_t) and VC8 changed time_t to a 64-bit value even when compiling a
// 32-bit application. Therefore, ace/Time_Value needs to rearrange a few
// things for this compiler. See Time_Value.h for complete details.
#if !defined (_USE_32BIT_TIME_T)
#  define ACE_HAS_TIME_T_LONG_MISMATCH
#endif

#define ACE_HAS_ITOA

#define ACE_ITOA_EQUIVALENT ::_itoa
#define ACE_STRCASECMP_EQUIVALENT ::_stricmp
#define ACE_STRNCASECMP_EQUIVALENT ::_strnicmp
#define ACE_WCSDUP_EQUIVALENT ::_wcsdup
#define ACE_FILENO_EQUIVALENT(X) (_get_osfhandle (::_fileno (X)))

#define ACE_HAS_SIG_ATOMIC_T

#define ACE_LACKS_STRPTIME

#define ACE_HAS_INTRIN_H
#define ACE_HAS_INTRINSIC_INTERLOCKED

#define ACE_HAS_INTRINSIC_BYTESWAP

#define ACE_LACKS_STRRECVFD

// Platform provides ACE_TLI function prototypes.
// For Win32, this is not really true, but saves a lot of hassle!
#define ACE_HAS_TLI_PROTOTYPES

// Platform support linebuffered streaming is broken
#define ACE_LACKS_LINEBUFFERED_STREAMBUF

// ace/iostream.h does not work with the standard cpp library (yet).
#if !defined (ACE_USES_OLD_IOSTREAMS)
# define ACE_LACKS_ACE_IOSTREAM
#endif /* ! ACE_USES_OLD_IOSTREAMS */

// There are too many instances of this warning to fix it right now.
// Maybe in the future.

// Disable warning of using Microsoft Extension.
#pragma warning(disable:4231)

// 'class1' : inherits 'class2::member' via dominance
#pragma warning(disable:4250)

#if !defined (ACE_HAS_TR24731_2005_CRT)
#  define ACE_HAS_TR24731_2005_CRT
#endif

// A template can not be exported. Only an instantiation may be exported.
#define ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT

// Windows Vista and Windows Server 2008 and newer do have native condition
// variables, but this is commented out because the support in ACE hasn't
// been completed
// #if defined (_WIN32_WINNT) && (_WIN32_WINNT >= 0x0600)
// # define ACE_HAS_WTHREADS_CONDITION_VARIABLE
// # undef ACE_LACKS_COND_T
// #endif

#define ACE_HAS_POSIX_TIME 1
#define ACE_LACKS_TIMESPEC_T 1

// According to MS the Visual Studio 2015 C-runtime has a
// C99 compliant vsnprintf/vsnwprintf, this is a change compared to
// previous versions
#define ACE_HAS_C99_VSNPRINTF
#define ACE_HAS_C99_VSNWPRINTF

// Visual Studio 2015 has 3 parameter wcstok
#define ACE_HAS_3_PARAM_WCSTOK

// Visual Studio 2015 has adequate C++11 support
#define ACE_HAS_CPP11

#define ACE_PUTENV_EQUIVALENT ::_putenv
#define ACE_TEMPNAM_EQUIVALENT ::_tempnam
#define ACE_STRDUP_EQUIVALENT ::_strdup
#define ACE_MKDIR_EQUIVALENT ::_mkdir
#define ACE_ACCESS_EQUIVALENT ::_access
#define ACE_CHDIR_EQUIVALENT ::_chdir
#define ACE_RMDIR_EQUIVALENT ::_rmdir
#define ACE_GETCWD_EQUIVALENT ::_getcwd
#define ACE_SWAB_EQUIVALENT ::_swab
#define ACE_UNLINK_EQUIVALENT ::_unlink

#define ACE_HAS_STRNLEN
#define ACE_HAS_WCSNLEN

#define ACE_LACKS_STRUCT_DIR
#define ACE_LACKS_OPENDIR
#define ACE_LACKS_CLOSEDIR
#define ACE_LACKS_READDIR

#define ACE_LACKS_MODE_T
#define ACE_LACKS_PID_T

#define ACE_LACKS_NLINK_T
#define ACE_LACKS_UID_T
#define ACE_LACKS_GID_T

#if _MSVC_LANG >= 201402L
# define ACE_HAS_CPP14
#endif /* _MSVC_LANG >= 201402L */

#if _MSVC_LANG >= 201703L
# define ACE_HAS_CPP17
#endif /* _MSVC_LANG >= 201703L */

#if _MSVC_LANG >= 202002L
# define ACE_HAS_CPP20
#endif /* _MSVC_LANG >= 202002L */

#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_WIN32_MSVC_141_H */
