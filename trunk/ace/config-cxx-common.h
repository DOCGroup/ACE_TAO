// $Id$

#ifndef ACE_CXX_COMMON_H
#define ACE_CXX_COMMON_H

#if !defined (ACE_CONFIG_INCLUDE_CXX_COMMON)
# error ace/config-cxx-common.h: ACE configuration error!  Do not #include this file directly!
#endif

#if defined (__DECCXX)
# define ACE_HAS_STRING_CLASS
# define ACE_HAS_WORKING_EXPLICIT_TEMPLATE_DESTRUCTOR
# define ACE_LACKS_LINEBUFFERED_STREAMBUF
# define ACE_LACKS_SIGNED_CHAR
# if defined (linux)
#   define ACE_HAS_CPLUSPLUS_HEADERS
# endif /* linux */
# define DEC_CXX
# if (__DECCXX_VER >= 60090010)
    // DEC CXX 6.0 supports exceptions, etc., by default.  Exceptions
    // are enabled by platform_osf1_4.0.GNU/wrapper_macros.GNU.
#   if defined (ACE_HAS_EXCEPTIONS)  &&  !defined (linux)
#     define ACE_NEW_THROWS_EXCEPTIONS
#   endif /* ACE_HAS_EXCEPTIONS && ! linux */
#   define ACE_HAS_ANSI_CASTS
#   if !defined (__RTTI)
#     define ACE_LACKS_RTTI
#   endif
#   if !defined (linux)
#     define ACE_HAS_STDCPP_STL_INCLUDES
#   endif /* ! linux */
#   define ACE_HAS_TEMPLATE_SPECIALIZATION
#   define ACE_HAS_TEMPLATE_TYPEDEFS
#   define ACE_HAS_TYPENAME_KEYWORD
#   define ACE_HAS_USING_KEYWORD

//    9: nested comment not allowed.  (/usr/include/pdsc.h!) (nestcomment)
//  177: variable was declared but never referenced (declbutnotref)
//  193: zero used for undefined preprocessing identifier (undpreid)
//  236: controlling expression is constant (boolexprconst)
//  401: base_class_with_nonvirtual_dtor (basclsnondto)
// 1016: expected type is incompatible with declared type of int (incint)
// 1136: conversion to smaller size integer could lose data (intconlosbit)

#   pragma message disable basclsnondto
#   pragma message disable boolexprconst
#   pragma message disable undpreid

#   if (__DECCXX_VER >= 60190029)
      // 6.1-029 and later support msg 1136.  Disable it because it
      // causes warnings from ACE and/or TAO.
#     pragma message disable intconlosbit
#   endif /* __DECCXX_VER >= 60190029 */

#   if defined (DIGITAL_UNIX)  &&  DIGITAL_UNIX >= 0x40F
      // variable "PTHREAD_THIS_CATCH_NP" was declared but never referenced
#     pragma message disable declbutnotref
#   endif /* DIGITAL_UNIX >= 4.0f */

# else  /* __DECCXX_VER < 60090010 */
#   define ACE_LACKS_PRAGMA_ONCE
# endif /* __DECCXX_VER < 60090010 */
#else  /* ! __DECCXX */
# error ace/config-cxx-common.h can only be used with Compaq CXX!
#endif /* ! __DECCXX */

#endif /* ACE_CXX_COMMON_H */
