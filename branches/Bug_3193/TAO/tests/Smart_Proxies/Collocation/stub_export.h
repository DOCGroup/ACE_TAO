
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef MY_STUB_EXPORT_H
#define MY_STUB_EXPORT_H

#include "ace/config-all.h"

#if defined (TAO_AS_STATIC_LIBS)
#  if !defined (MY_STUB_HAS_DLL)
#    define MY_STUB_HAS_DLL 0
#  endif /* ! MY_STUB_HAS_DLL */
#else
#  if !defined (MY_STUB_HAS_DLL)
#    define MY_STUB_HAS_DLL 1
#  endif /* ! MY_STUB_HAS_DLL */
#endif

#if defined (MY_STUB_HAS_DLL) && (MY_STUB_HAS_DLL == 1)
#  if defined (MY_STUB_BUILD_DLL)
#    define MY_Stub_Export ACE_Proper_Export_Flag
#    define MY_STUB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define MY_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* MY_STUB_BUILD_DLL */
#    define MY_Stub_Export ACE_Proper_Import_Flag
#    define MY_STUB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define MY_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* MY_STUB_BUILD_DLL */
#else /* MY_STUB_HAS_DLL == 1 */
#  define MY_Stub_Export
#  define MY_STUB_SINGLETON_DECLARATION(T)
#  define MY_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* MY_STUB_HAS_DLL == 1 */

#endif /* MY_STUB_EXPORT_H */

// End of auto generated file.
