/* -*- C++ -*- */
// $Id$

// This configuration file is designed to be included by another,
// specific configuration file.  It provides config information common
// to all KAI C++ platforms (KCC version 3.3a).
//
// KAI C++ 3.2d for Linux had a known problem with being unable to call
// static object constructors and destructors within shared libraries.
//
// KAI C++ 3.3a requires glibc 2. This configuration was tested with
// egcs 1.0.2 and glibc-2.0.7-7. The KCC_BASE/bin/KCC script requires
// the following settings for i386-linux to work:
//
//case i386-linux:
//    set common_na = "-Dlinux -Dunix -Di386"
//    set cpp_defines = "-D__CPLUSPLUS -D__inline__=inline -D__inline=inline -D__signed__=signed"
//    set c_defines = "-D__inline__= -D__inline= -D__signed__=signed"
//    set knr_c_defines = "-D__inline__= -D__inline= -D__signed__="
//    set ansi_c_defines = ""
//    breaksw
//

#ifndef ACE_KCC_COMMON_H
#define ACE_KCC_COMMON_H

#if (defined (i386) || defined (__i386__)) && !defined (ACE_SIZEOF_LONG_DOUBLE)
# define ACE_SIZEOF_LONG_DOUBLE 12
#endif /* i386 */

#if defined (i386) || defined (__i386__)
  // If running an Intel, assume that it's a Pentium so that
  // ACE_OS::gethrtime () can use the RDTSC instruction.  If
  // running a 486 or lower, be sure to comment this out.
  // (If not running an Intel CPU, this #define will not be seen
  //  because of the i386 protection, so it can be ignored.)
# define ACE_HAS_PENTIUM
#endif /* i386 */

// ****** Need to move this somewhere else
// #define TAO_YY_BREAK

#define ACE_SIZEOF_LONG_LONG 8
#define ACE_TEMPLATES_REQUIRE_SOURCE

#define ACE_HAS_TEMPLATE_SPECIALIZATION
#define ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION

// Compiler doesn't support static data member templates.
//#define ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES

#define ACE_HAS_USING_KEYWORD

// Define this if you want to use the standard C++ library
#define ACE_HAS_STANDARD_CPP_LIBRARY 1
#define ACE_HAS_STDCPP_STL_INCLUDES 1
#define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 1
#define ACE_LACKS_IOSTREAM_FX 1
#define ACE_LACKS_LINEBUFFERED_STREAMBUF 1
#define ACE_LACKS_UNBUFFERED_STREAMBUF 1
#define ACE_HAS_EXCEPTIONS 1
#define ACE_HAS_CPLUSPLUS_HEADERS

#define ACE_HAS_NONSTATIC_OBJECT_MANAGER 1

#endif /* ACE_KCC_COMMON_H */
