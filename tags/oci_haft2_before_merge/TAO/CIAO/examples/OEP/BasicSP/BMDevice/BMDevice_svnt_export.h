
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl BMDEVICE_SVNT
// ------------------------------
#ifndef BMDEVICE_SVNT_EXPORT_H
#define BMDEVICE_SVNT_EXPORT_H

#include "ace/config-all.h"

#if !defined (BMDEVICE_SVNT_HAS_DLL)
#  define BMDEVICE_SVNT_HAS_DLL 1
#endif /* ! BMDEVICE_SVNT_HAS_DLL */

#if defined (BMDEVICE_SVNT_HAS_DLL) && (BMDEVICE_SVNT_HAS_DLL == 1)
#  if defined (BMDEVICE_SVNT_BUILD_DLL)
#    define BMDEVICE_SVNT_Export ACE_Proper_Export_Flag
#    define BMDEVICE_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define BMDEVICE_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* BMDEVICE_SVNT_BUILD_DLL */
#    define BMDEVICE_SVNT_Export ACE_Proper_Import_Flag
#    define BMDEVICE_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define BMDEVICE_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* BMDEVICE_SVNT_BUILD_DLL */
#else /* BMDEVICE_SVNT_HAS_DLL == 1 */
#  define BMDEVICE_SVNT_Export
#  define BMDEVICE_SVNT_SINGLETON_DECLARATION(T)
#  define BMDEVICE_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* BMDEVICE_SVNT_HAS_DLL == 1 */

// Set BMDEVICE_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (BMDEVICE_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define BMDEVICE_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define BMDEVICE_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !BMDEVICE_SVNT_NTRACE */

#if (BMDEVICE_SVNT_NTRACE == 1)
#  define BMDEVICE_SVNT_TRACE(X)
#else /* (BMDEVICE_SVNT_NTRACE == 1) */
#  define BMDEVICE_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (BMDEVICE_SVNT_NTRACE == 1) */

#endif /* BMDEVICE_SVNT_EXPORT_H */

// End of auto generated file.
