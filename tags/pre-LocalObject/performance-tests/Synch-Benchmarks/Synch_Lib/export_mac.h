// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by
// ${ACE_ROOT}/GenExportH.BAT
// ------------------------------
#if !defined (SYNCHLIB_EXPORT_H)
#define SYNCHLIB_EXPORT_H

#include "ace/OS.h"

#if !defined (SYNCHLIB_HAS_DLL)
#define SYNCHLIB_HAS_DLL 1
#endif /* !SYNCHLIB_HAS_DLL */

#if defined (SYNCHLIB_HAS_DLL)
#  if (SYNCHLIB_HAS_DLL == 1)
#    if defined (SYNCHLIB_BUILD_DLL)
#      define SYNCHLIB_Export ACE_Proper_Export_Flag
#      define SYNCHLIB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    else
#      define SYNCHLIB_Export ACE_Proper_Import_Flag
#      define SYNCHLIB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    endif /* SYNCHLIB_BUILD_DLL */
#  else
#    define SYNCHLIB_Export
#    define SYNCHLIB_SINGLETON_DECLARATION(T)
#  endif   /* ! SYNCHLIB_HAS_DLL == 1 */
#else
#  define SYNCHLIB_Export
#  define SYNCHLIB_SINGLETON_DECLARATION(T)
#endif     /* SYNCHLIB_HAS_DLL */

#endif     /* SYNCHLIB_EXPORT_H */

// End of auto generated file.
