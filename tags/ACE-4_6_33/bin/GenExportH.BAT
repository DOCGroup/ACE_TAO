@Echo off
REM
REM $Id$
REM
REM Usage:
REM	GenExportH.BAT <EXPORT_TAG>   >   <export_file.h>
REM
REM Author: Nanbor Wang <nanbor@cs.wustl.edu>
REM

if "%1" == "" goto emptyexport
Echo // -*- C++ -*-
Echo // $Id$
Echo // Definition for Win32 Export directives.
Echo // This file is generated automatically by
Echo // ${ACE_ROOT}/GenExportH.BAT
Echo // ------------------------------
Echo #if !defined (%1_EXPORT_H)
Echo #define %1_EXPORT_H
Echo.
Echo #include "ace/OS.h"
Echo.
Echo #if !defined (%1_HAS_DLL)
Echo #define %1_HAS_DLL 1
Echo #endif /* ! %1_HAS_DLL */
Echo.
Echo #if defined (%1_HAS_DLL)
Echo #  if (%1_HAS_DLL == 1)
Echo #    if defined (%1_BUILD_DLL)
Echo #      define %1_Export ACE_Proper_Export_Flag
Echo #      define %1_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
Echo #    else
Echo #      define %1_Export ACE_Proper_Import_Flag
Echo #      define %1_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
Echo #    endif /* %1_BUILD_DLL */
Echo #  else
Echo #    define %1_Export
Echo #    define %1_SINGLETON_DECLARATION(T)
Echo #  endif   /* ! %1_HAS_DLL == 1 */
Echo #else
Echo #  define %1_Export
Echo #  define %1_SINGLETON_DECLARATION(T)
Echo #endif     /* %1_HAS_DLL */
Echo.
Echo #endif     /* %1_EXPORT_H */
Echo.
Echo // End of auto generated file.
goto endrun
:emptyexport
Echo #error usage: GenExportH.BAT EXPORT_TAG (Remember to redirect the output to a file.)
:endrun