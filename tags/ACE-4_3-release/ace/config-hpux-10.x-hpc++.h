/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for HP
// platforms running HP/UX 10.x with either of the HP C++ compilers.
// This file contains compiler-specific settings, and uses the common
// HP-UX file (config-hpux-10.x.h) for the platform information.

#if !defined (ACE_CONFIG_H)
#define ACE_CONFIG_H

// There are 2 compiler-specific sections, plus a 3rd for common to both.
// First is the HP C++ section...
#if __cplusplus < 199707L

// To use HP/C++ you need some patches, the following set
//
// PHSS_9855
// PHSS_9096
// PHSS_10261
// PHSS_9062
// PHSS_7505
// PHSS_8490
//
// worked for me (Carlos O'Ryan), but you should check by yourself.
// The most important one is PHSS_9855, but in depends on some of the
// others.

#  define ACE_HAS_BROKEN_HPUX_TEMPLATES

// Compiler can't handle calls like foo->operator T *()
#  define ACE_HAS_BROKEN_CONVERSIONS

// Compiler can't handle large enums (e.g., HP/UX C++)
#  define ACE_HAS_BROKEN_ENUMS

// Necessary with some compilers to pass ACE_TTY_IO as parameter to
// DEV_Connector.
#define ACE_NEEDS_DEV_IO_CONVERSION

// Compiler's template mechanism must see source code (i.e., .C files).
#  define ACE_TEMPLATES_REQUIRE_SOURCE

// Compiler's template mechanism requires the use of explicit C++
// specializations for all used templates.
#  define ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION

// The HP/UX compiler doesn't support volatile!!!!
#  define volatile

#else  // aC++ definitions

// Compiler can't handle calls like foo->operator T *()
#  define ACE_HAS_BROKEN_CONVERSIONS

// Compiler supports C++ exception handling
#  define ACE_HAS_EXCEPTIONS

#  define ACE_HAS_TYPENAME_KEYWORD

// Compiler implements templates that support typedefs inside of classes
// used as formal arguments to a template class. 
#  define ACE_HAS_TEMPLATE_TYPEDEFS

// Platform lacks streambuf "linebuffered ()".
#  define ACE_LACKS_LINEBUFFERED_STREAMBUF

// Compiler's template mechanism must see source code (i.e., .C files).
#  define ACE_TEMPLATES_REQUIRE_SOURCE

#endif /* __cplusplus < 199707L */

// Compiler supports the ssize_t typedef.
#define ACE_HAS_SSIZE_T

#include "ace/config-hpux-10.x.h"	/* OS information */

#endif /* ACE_CONFIG_H */
