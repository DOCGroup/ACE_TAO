// -*- C++ -*-
//$Id$

// The following configuration file contains defines for Borland compilers.

#ifndef ACE_CONFIG_WIN32_BORLAND_H
#define ACE_CONFIG_WIN32_BORLAND_H
#include "ace/pre.h"

#ifndef ACE_CONFIG_WIN32_H
#error Use config-win32.h in config.h instead of this header
#endif /* ACE_CONFIG_WIN32_H */

#include "config-borland-common.h"

# if (__BORLANDC__ == 0x540)
// The linker in C++Builder 4 has trouble with the large number of DLL
// function exports created when you compile without inline functions.
// Therefore we will always use inline functions with this version of
// the compiler.
#  if defined (__ACE_INLINE__)
#   undef __ACE_INLINE__
#  endif /* __ACE_INLINE__ */
#  define __ACE_INLINE__ 1
# else /* __BORLANDC__ == 0x540 */
// In later versions of C++Builder we will prefer inline functions by
// default. The debug configuration of ACE is built with functions
// out-of-line, so when linking your application against a debug ACE
// build, you can choose to use the out-of-line functions by adding
// ACE_NO_INLINE=1 to your project settings.
#  if !defined (__ACE_INLINE__)
#   define __ACE_INLINE__ 1
#  endif /* __ACE_INLINE__ */
# endif /* __BORLANDC__ == 0x540 */

// Automatically define WIN32 macro if the compiler tells us it is our
// target platform.
# if defined (__WIN32__) && !defined (WIN32)
#  define WIN32 1
# endif

// When building a VCL application, the main VCL header file should be
// included before anything else. You can define ACE_HAS_VCL=1 in your
// project settings to have this file included for you automatically.
# if defined (ACE_HAS_VCL) && (ACE_HAS_VCL != 0)
#  include /**/ <vcl.h>
# endif

# include "ace/config-win32-common.h"

# define ACE_STRCASECMP_EQUIVALENT ::stricmp
# define ACE_STRNCASECMP_EQUIVALENT ::strnicmp
# define ACE_WCSDUP_EQUIVALENT ::_wcsdup

# define ACE_LACKS_MODE_MASKS 1
# define ACE_WSTRING_HAS_USHORT_SUPPORT 1

#ifdef ACE_USES_STD_NAMESPACE_FOR_STDC_LIB
#undef ACE_USES_STD_NAMESPACE_FOR_STDC_LIB
#define ACE_USES_STD_NAMESPACE_FOR_STDC_LIB 1
#endif

#include "ace/post.h"
#endif /* ACE_CONFIG_WIN32_BORLAND_H */
