
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef TCF_EXPORT_H
#define TCF_EXPORT_H

#include "ace/config-all.h"

#if defined (TAO_AS_STATIC_LIBS)
#  if !defined (TCF_HAS_DLL)
#    define TCF_HAS_DLL 0
#  endif /* ! TCF_HAS_DLL */
#else
#  if !defined (TCF_HAS_DLL)
#    define TCF_HAS_DLL 1
#  endif /* ! TCF_HAS_DLL */
#endif

#if defined (TCF_HAS_DLL) && (TCF_HAS_DLL == 1)
#  if defined (TCF_BUILD_DLL)
#    define TCF_Export ACE_Proper_Export_Flag
#    define TCF_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TCF_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TCF_BUILD_DLL */
#    define TCF_Export ACE_Proper_Import_Flag
#    define TCF_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TCF_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TCF_BUILD_DLL */
#else /* TCF_HAS_DLL == 1 */
#  define TCF_Export
#  define TCF_SINGLETON_DECLARATION(T)
#  define TCF_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TCF_HAS_DLL == 1 */

#endif /* TCF_EXPORT_H */

// End of auto generated file.
