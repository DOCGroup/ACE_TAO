
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl SLDEX
// ------------------------------
#ifndef SLDEX_EXPORT_H
#define SLDEX_EXPORT_H

#include "ace/config-all.h"

#if !defined (SLDEX_HAS_DLL)
#  define SLDEX_HAS_DLL 1
#endif /* ! SLDEX_HAS_DLL */

#if defined (SLDEX_HAS_DLL) && (SLDEX_HAS_DLL == 1)
#  if defined (SLDEX_BUILD_DLL)
#    define SLDEX_Export ACE_Proper_Export_Flag
#    define SLDEX_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define SLDEX_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* SLDEX_BUILD_DLL */
#    define SLDEX_Export ACE_Proper_Import_Flag
#    define SLDEX_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define SLDEX_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* SLDEX_BUILD_DLL */
#else /* SLDEX_HAS_DLL == 1 */
#  define SLDEX_Export
#  define SLDEX_SINGLETON_DECLARATION(T)
#  define SLDEX_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* SLDEX_HAS_DLL == 1 */

#endif /* SLDEX_EXPORT_H */

// End of auto generated file.
