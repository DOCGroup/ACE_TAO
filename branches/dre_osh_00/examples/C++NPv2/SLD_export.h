
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl SLD
// ------------------------------
#ifndef SLD_EXPORT_H
#define SLD_EXPORT_H

#include "ace/config-all.h"

#if !defined (SLD_HAS_DLL)
#  define SLD_HAS_DLL 1
#endif /* ! SLD_HAS_DLL */

#if defined (SLD_HAS_DLL) && (SLD_HAS_DLL == 1)
#  if defined (SLD_BUILD_DLL)
#    define SLD_Export ACE_Proper_Export_Flag
#    define SLD_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define SLD_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* SLD_BUILD_DLL */
#    define SLD_Export ACE_Proper_Import_Flag
#    define SLD_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define SLD_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* SLD_BUILD_DLL */
#else /* SLD_HAS_DLL == 1 */
#  define SLD_Export
#  define SLD_SINGLETON_DECLARATION(T)
#  define SLD_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* SLD_HAS_DLL == 1 */

#endif /* SLD_EXPORT_H */

// End of auto generated file.
