/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for HP
// platforms running HP/UX 11.x with either of the HP C++ compilers.
// This file contains compiler-specific settings, and uses the common
// HP-UX file (config-hpux-11.x.h) for the platform information.
//
// NOTE - HP advises people on 11.x to use aC++ since the older C++ doesn't
// support 64-bit or kernel threads.  So, though this file has the C++ info
// in it, it's copied from the 10.x file and hasn't been verified.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H

// There are 2 compiler-specific sections, plus a 3rd for common to both.
// First is the HP C++ section...
#if __cplusplus < 199707L

#  define ACE_HAS_BROKEN_HPUX_TEMPLATES

// Compiler can't handle calls like foo->operator T *()
#  define ACE_HAS_BROKEN_CONVERSIONS

// Necessary with some compilers to pass ACE_TTY_IO as parameter to
// DEV_Connector.
#  define ACE_NEEDS_DEV_IO_CONVERSION

// Compiler's template mechanism must see source code (i.e., .C files).
#  define ACE_TEMPLATES_REQUIRE_SOURCE

// Compiler's template mechanism requires the use of explicit C++
// specializations for all used templates.
#  define ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION

// The HP/UX compiler doesn't support volatile!!!!
#  define volatile

#else  // aC++ definitions

// Compiler supports ANSI casts
#  define ACE_HAS_ANSI_CASTS

// Compiler can't handle calls like foo->operator T *()
#  define ACE_HAS_BROKEN_CONVERSIONS

// Compiler supports C++ exception handling
#  define ACE_HAS_EXCEPTIONS

// Compiler enforces the "One Definition Rule"
#  define ACE_HAS_ONE_DEFINITION_RULE

#  define ACE_HAS_TYPENAME_KEYWORD

// Compiler implements templates that support typedefs inside of classes
// used as formal arguments to a template class.
#  define ACE_HAS_TEMPLATE_TYPEDEFS

// This is legit for A.03.05 - not sure A.03.04, but it should be.
#  define ACE_HAS_USING_KEYWORD

// Platform lacks streambuf "linebuffered ()".
#  define ACE_LACKS_LINEBUFFERED_STREAMBUF

// Compiler's 'new' throws exceptions on failure.
#  define ACE_NEW_THROWS_EXCEPTIONS

// Compiler's template mechanism must see source code (i.e., .C files).
#  define ACE_TEMPLATES_REQUIRE_SOURCE

// Compiler supports template specialization.
#  define ACE_HAS_TEMPLATE_SPECIALIZATION

// Preprocessor needs some help with data types
#  if defined (__LP64__)
#    define ACE_SIZEOF_LONG 8
#  else
#    define ACE_SIZEOF_LONG 4
#  endif

#endif /* __cplusplus < 199707L */

// Compiler supports the ssize_t typedef.
#define ACE_HAS_SSIZE_T
#define ACE_HAS_UALARM

// Optimize ACE_Handle_Set for select().
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

// Compiler doesn't handle 'signed char' correctly (used in ace/IOStream.h)
#define ACE_LACKS_SIGNED_CHAR

#define ACE_HAS_GPERF

#include "ace/config-hpux11.h"		/* OS information */

#endif /* ACE_CONFIG_H */
