/* -*- C++ -*- */
// $Id$

// The following configuration file contains the defines
// common to all Borland compilers.

#ifndef ACE_WIN32_BORLAND_H
#define ACE_WIN32_BORLAND_H

# if defined(__BORLANDC__)

#   if defined (ACE_LACKS_MODE_MASKS)
#     undef ACE_LACKS_MODE_MASKS
#   endif /* defined (ACE_LACKS_MODE_MASKS) */
#   define ACE_LACKS_MODE_MASKS 1

#   if defined (ACE_HAS_USER_MODE_MASKS)
#     undef ACE_HAS_USER_MODE_MASKS
#   endif /* defined (ACE_HAS_USER_MODE_MASKS) */
#   define ACE_HAS_USER_MODE_MASKS 1

#   if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#     undef ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION
#   endif /* defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION) */
#   define ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION 1

#   if defined (ACE_EXPORT_NESTED_CLASSES)
#     undef ACE_EXPORT_NESTED_CLASSES
#   endif /* defined (ACE_EXPORT_NESTED_CLASSES) */
#   define ACE_EXPORT_NESTED_CLASSES 1

#   if defined (ACE_HAS_NONCONST_SELECT_TIMEVAL)
#     undef ACE_HAS_NONCONST_SELECT_TIMEVAL
#   endif /* defined (ACE_HAS_NONCONST_SELECT_TIMEVAL) */
#   define ACE_HAS_NONCONST_SELECT_TIMEVAL 1

#   if (__BORLANDC__ < 0x540)
#     if defined (ACE_HAS_BROKEN_SAP_ANY)
#       undef ACE_HAS_BROKEN_SAP_ANY
#     endif /* defined (ACE_HAS_BROKEN_SAP_ANY) */
#     define ACE_HAS_BROKEN_SAP_ANY 1
#   endif /* (__BORLANDC__ < 0x540) */

/*
 * Definitions common to Borland C++ 5.x, Borland C++ Builder 3.x,
 * and C++ Builder 4.x.
 */
#   if (__BORLANDC__ >= 0x500)
#     if !defined (ACE_HAS_WINCE)
#       if defined (ACE_HAS_SIG_ATOMIC_T)
#         undef ACE_HAS_SIG_ATOMIC_T
#       endif /* defined (ACE_HAS_SIG_ATOMIC_T) */
#       define ACE_HAS_SIG_ATOMIC_T 1
#     endif /* !defined (ACE_HAS_WINCE) */

#     if defined (ACE_HAS_TYPENAME_KEYWORD)
#       undef ACE_HAS_TYPENAME_KEYWORD
#     endif /* defined (ACE_HAS_TYPENAME_KEYWORD) */
#     define ACE_HAS_TYPENAME_KEYWORD 1

#     if defined (ACE_SIZEOF_LONG_DOUBLE)
#       undef ACE_SIZEOF_LONG_DOUBLE
#     endif /* defined (ACE_SIZEOF_LONG_DOUBLE) */
#     define ACE_SIZEOF_LONG_DOUBLE 10

#     if defined (ACE_HAS_USING_KEYWORD)
#       undef ACE_HAS_USING_KEYWORD
#     endif /* defined (ACE_HAS_USING_KEYWORD) */
#     define ACE_HAS_USING_KEYWORD 1

#     if defined (ACE_HAS_STANDARD_CPP_LIBRARY)
#       undef ACE_HAS_STANDARD_CPP_LIBRARY
#     endif /* defined (ACE_HAS_STANDARD_CPP_LIBRARY) */
#     define ACE_HAS_STANDARD_CPP_LIBRARY 0
#   endif /* (__BORLANDC__ >= 0x500) */

/*
 * Definitions for Borland C++ Builder 3.x and C++ Builder 4.x.
 */
#   if (__BORLANDC__ >= 0x0530)

#     if defined (ACE_HAS_STANDARD_CPP_LIBRARY)
#       undef ACE_HAS_STANDARD_CPP_LIBRARY
#     endif /* defined (ACE_HAS_STANDARD_CPP_LIBRARY) */
#     define ACE_HAS_STANDARD_CPP_LIBRARY 1

#     if defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB)
#       undef ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB
#     endif /* defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB) */
#     define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 1

#   endif /* (__BORLANDC__ >= 0x0530) */

/*
 * Definitions only for Borland C++ Builder 4.x
 */
#   if (__BORLANDC__ == 0x0540) 

// Compiler enforces need for 'template<>" when specializing template
// class methods.
#     define ACE_HAS_STD_TEMPLATE_SPECIALIZATION 

// Compiler enforces need for 'template<>" when specializing template
// class methods.
#     define ACE_HAS_STD_TEMPLATE_METHOD_SPECIALIZATION

#     if defined (ACE_HAS_TEMPLATE_TYPEDEFS)
#       undef ACE_HAS_TEMPLATE_TYPEDEFS
#     endif /* defined (ACE_HAS_TEMPLATE_TYPEDEFS) */
#     define ACE_HAS_TEMPLATE_TYPEDEFS 1

#     if defined (ACE_HAS_STRING_CLASS)
#       undef ACE_HAS_STRING_CLASS
#     endif /* defined (ACE_HAS_STRING_CLASS) */
#     define ACE_HAS_STRING_CLASS 1

#     if defined (ACE_HAS_STDCPP_STL_INCLUDES)
#       undef ACE_HAS_STDCPP_STL_INCLUDES
#     endif /* defined (ACE_HAS_STDCPP_STL_INCLUDES) */
#     define ACE_HAS_STDCPP_STL_INCLUDES 1

      /* ACE IOStreams don't work with standard C++ iostreams (yet) */
#     if defined (ACE_LACKS_ACE_IOSTREAM)
#       undef ACE_LACKS_ACE_IOSTREAM
#     endif /* defined (ACE_LACKS_ACE_IOSTREAM) */
#     define ACE_LACKS_ACE_IOSTREAM 1

      /* previously defined for MSVC */ 
#     if defined (ACE_HAS_WCHAR_TYPEDEFS_USHORT)
#       undef ACE_HAS_WCHAR_TYPEDEFS_USHORT
#     endif /* defined (ACE_HAS_WCHAR_TYPEDEFS_USHORT) */

      /* previously defined for MSVC */
#     if defined (ACE_HAS_BROKEN_NAMESPACES)
#       undef ACE_HAS_BROKEN_NAMESPACES
#     endif /* defined (ACE_HAS_BROKEN_NAMESPACES) */

      /* previously defined for MSVC */
#     if defined (ACE_HAS_WORKING_EXPLICIT_TEMPLATE_DESTRUCTOR)
#       undef ACE_HAS_WORKING_EXPLICIT_TEMPLATE_DESTRUCTOR
#     endif /* defined (ACE_HAS_WORKING_EXPLICIT_TEMPLATE_DESTRUCTOR) */

      /* need to ensure these are included before <iomanip> */
#     include <time.h>
#     include <stdlib.h>

#   endif /* (__BORLANDC__ == 0x0540) */

# endif /* defined(__BORLANDC__) */

#endif /* ACE_WIN32_BORLAND_H */

