
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl TPLS
// ------------------------------
#ifndef TPLS_EXPORT_H
#define TPLS_EXPORT_H

#include "ace/config-all.h"

#if !defined (TPLS_HAS_DLL)
#  define TPLS_HAS_DLL 1
#endif /* ! TPLS_HAS_DLL */

#if defined (TPLS_HAS_DLL) && (TPLS_HAS_DLL == 1)
#  if defined (TPLS_BUILD_DLL)
#    define TPLS_Export ACE_Proper_Export_Flag
#    define TPLS_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TPLS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TPLS_BUILD_DLL */
#    define TPLS_Export ACE_Proper_Import_Flag
#    define TPLS_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TPLS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TPLS_BUILD_DLL */
#else /* TPLS_HAS_DLL == 1 */
#  define TPLS_Export
#  define TPLS_SINGLETON_DECLARATION(T)
#  define TPLS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TPLS_HAS_DLL == 1 */

#endif /* TPLS_EXPORT_H */

// End of auto generated file.
