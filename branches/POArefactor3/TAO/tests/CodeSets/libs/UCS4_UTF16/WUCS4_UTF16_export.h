
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef UCS4_UTF16_EXPORT_H
#define UCS4_UTF16_EXPORT_H

#include "ace/config-all.h"

#if defined (TAO_AS_STATIC_LIBS)
#  if !defined (UCS4_UTF16_HAS_DLL)
#    define UCS4_UTF16_HAS_DLL 0
#  endif /* ! UCS4_UTF16_HAS_DLL */
#else
#  if !defined (UCS4_UTF16_HAS_DLL)
#    define UCS4_UTF16_HAS_DLL 1
#  endif /* ! UCS4_UTF16_HAS_DLL */
#endif

#if defined (UCS4_UTF16_HAS_DLL) && (UCS4_UTF16_HAS_DLL == 1)
#  if defined (UCS4_UTF16_BUILD_DLL)
#    define UCS4_UTF16_Export ACE_Proper_Export_Flag
#    define UCS4_UTF16_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define UCS4_UTF16_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* UCS4_UTF16_BUILD_DLL */
#    define UCS4_UTF16_Export ACE_Proper_Import_Flag
#    define UCS4_UTF16_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define UCS4_UTF16_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* UCS4_UTF16_BUILD_DLL */
#else /* UCS4_UTF16_HAS_DLL == 1 */
#  define UCS4_UTF16_Export
#  define UCS4_UTF16_SINGLETON_DECLARATION(T)
#  define UCS4_UTF16_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* UCS4_UTF16_HAS_DLL == 1 */

#endif /* UCS4_UTF16_EXPORT_H */

// End of auto generated file.
