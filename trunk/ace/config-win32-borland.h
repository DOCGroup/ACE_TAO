/* -*- C++ -*- */
// $Id$

// The following configuration file contains the defines
// common to all Borland compilers.

#ifndef ACE_WIN32_BORLAND_H
#define ACE_WIN32_BORLAND_H

#if defined(__BORLANDC__)

#if defined (ACE_LACKS_MODE_MASKS)
#undef ACE_LACKS_MODE_MASKS
#endif /* defined (ACE_LACKS_MODE_MASKS) */

#define ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION
#define ACE_HAS_BROKEN_SAP_ANY

/*
 * Definitions common to Borland C++ 5.x and Borland C++ Builder 3.x.
 */
#if (__BORLANDC__ >= 0x500)
  #if !defined (ACE_HAS_WINCE)
    #define ACE_HAS_SIG_ATOMIC_T
  #endif /* !defined (ACE_HAS_WINCE) */

  #define ACE_HAS_TYPENAME_KEYWORD

  #ifdef ACE_SIZEOF_LONG_DOUBLE
  #undef ACE_SIZEOF_LONG_DOUBLE
  #endif
  #define ACE_SIZEOF_LONG_DOUBLE 10

  #ifdef ACE_HAS_USING_KEYWORD
  #undef ACE_HAS_USING_KEYWORD
  #endif
  #define ACE_HAS_USING_KEYWORD 1

  #ifdef ACE_HAS_STANDARD_CPP_LIBRARY
  #undef ACE_HAS_STANDARD_CPP_LIBRARY
  #endif
  #define ACE_HAS_STANDARD_CPP_LIBRARY 0
#endif /* (__BORLANDC__ >= 0x500) */

/*
 * Definitions only for Borland C++ Builder 3.x.
 */
#if (__BORLANDC__ >= 0x0530)

  #ifdef ACE_HAS_STANDARD_CPP_LIBRARY
  #undef ACE_HAS_STANDARD_CPP_LIBRARY
  #endif
  #define ACE_HAS_STANDARD_CPP_LIBRARY 1

  #ifdef ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB
  #undef ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB
  #endif
  #define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 1

#endif /* (__BORLANDC__ >= 0x0530) */

#endif /* defined(__BORLANDC__) */

#endif /* ACE_WIN32_BORLAND_H */
