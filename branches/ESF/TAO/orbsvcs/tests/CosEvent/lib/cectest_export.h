// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by
// ${ACE_ROOT}/GenExportH.BAT
// ------------------------------
#if !defined (CEC_Test_EXPORT_H)
#define CEC_Test_EXPORT_H

#include "ace/OS.h"

#if !defined (CEC_Test_HAS_DLL)
#define CEC_Test_HAS_DLL 1
#endif /* ! CEC_Test_HAS_DLL */

#if defined (CEC_Test_HAS_DLL)
#  if (CEC_Test_HAS_DLL == 1)
#    if defined (CEC_Test_BUILD_DLL)
#      define CEC_Test_Export ACE_Proper_Export_Flag
#      define CEC_Test_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    else
#      define CEC_Test_Export ACE_Proper_Import_Flag
#      define CEC_Test_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    endif /* CEC_Test_BUILD_DLL */
#  else
#    define CEC_Test_Export
#    define CEC_Test_SINGLETON_DECLARATION(T)
#  endif   /* ! CEC_Test_HAS_DLL == 1 */
#else
#  define CEC_Test_Export
#  define CEC_Test_SINGLETON_DECLARATION(T)
#endif     /* CEC_Test_HAS_DLL */

#endif     /* CEC_Test_EXPORT_H */

// End of auto generated file.
