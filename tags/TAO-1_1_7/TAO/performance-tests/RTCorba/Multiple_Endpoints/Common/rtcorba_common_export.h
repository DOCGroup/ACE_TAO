// $Id$

// Definition for Win32 Export directives.
// This file is generated automatically by
// ${TAO_ROOT}/TAO_IDL/GenExportH.BAT
// ------------------------------
#ifndef RTCORBA_COMMON_EXPORT_H
#define RTCORBA_COMMON_EXPORT_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (RTCORBA_COMMON_HAS_DLL)
#  if (RTCORBA_COMMON_HAS_DLL == 1)
#    if defined (RTCORBA_COMMON_BUILD_DLL)
#      define RTCORBA_COMMON_Export ACE_Proper_Export_Flag
#      define RTCORBA_COMMON_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    else
#      define RTCORBA_COMMON_Export ACE_Proper_Import_Flag
#      define RTCORBA_COMMON_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    endif /* RTCORBA_COMMON_BUILD_DLL */
#  else
#    define RTCORBA_COMMON_Export
#    define RTCORBA_COMMON_SINGLETON_DECLARATION(T)
#  endif   /* ! RTCORBA_COMMON_HAS_DLL == 1 */
#else
#  define RTCORBA_COMMON_Export
#  define RTCORBA_COMMON_SINGLETON_DECLARATION(T)
#endif     /* RTCORBA_COMMON_HAS_DLL */

#endif     /* RTCORBA_COMMON_EXPORT_H */
           // End of auto generated file.
