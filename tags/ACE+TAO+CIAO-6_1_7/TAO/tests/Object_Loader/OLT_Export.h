
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef OLT_EXPORT_H
#define OLT_EXPORT_H

#include "ace/config-all.h"

#if defined (TAO_AS_STATIC_LIBS)
#  if !defined (OLT_HAS_DLL)
#    define OLT_HAS_DLL 0
#  endif /* ! OLT_HAS_DLL */
#else
#  if !defined (OLT_HAS_DLL)
#    define OLT_HAS_DLL 1
#  endif /* ! OLT_HAS_DLL */
#endif

#if defined (OLT_HAS_DLL) && (OLT_HAS_DLL == 1)
#  if defined (OLT_BUILD_DLL)
#    define OLT_Export ACE_Proper_Export_Flag
#    define OLT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define OLT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* OLT_BUILD_DLL */
#    define OLT_Export ACE_Proper_Import_Flag
#    define OLT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define OLT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* OLT_BUILD_DLL */
#else /* OLT_HAS_DLL == 1 */
#  define OLT_Export
#  define OLT_SINGLETON_DECLARATION(T)
#  define OLT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* OLT_HAS_DLL == 1 */

#endif /* OLT_EXPORT_H */

// End of auto generated file.
