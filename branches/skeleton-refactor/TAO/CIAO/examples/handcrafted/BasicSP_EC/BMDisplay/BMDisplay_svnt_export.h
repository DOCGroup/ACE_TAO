
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl BMDISPLAY_SVNT
// ------------------------------
#ifndef BMDISPLAY_SVNT_EXPORT_H
#define BMDISPLAY_SVNT_EXPORT_H

#include "ace/config-all.h"

#if !defined (BMDISPLAY_SVNT_HAS_DLL)
#  define BMDISPLAY_SVNT_HAS_DLL 1
#endif /* ! BMDISPLAY_SVNT_HAS_DLL */

#if defined (BMDISPLAY_SVNT_HAS_DLL) && (BMDISPLAY_SVNT_HAS_DLL == 1)
#  if defined (BMDISPLAY_SVNT_BUILD_DLL)
#    define BMDISPLAY_SVNT_Export ACE_Proper_Export_Flag
#    define BMDISPLAY_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define BMDISPLAY_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* BMDISPLAY_SVNT_BUILD_DLL */
#    define BMDISPLAY_SVNT_Export ACE_Proper_Import_Flag
#    define BMDISPLAY_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define BMDISPLAY_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* BMDISPLAY_SVNT_BUILD_DLL */
#else /* BMDISPLAY_SVNT_HAS_DLL == 1 */
#  define BMDISPLAY_SVNT_Export
#  define BMDISPLAY_SVNT_SINGLETON_DECLARATION(T)
#  define BMDISPLAY_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* BMDISPLAY_SVNT_HAS_DLL == 1 */

// Set BMDISPLAY_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (BMDISPLAY_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define BMDISPLAY_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define BMDISPLAY_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !BMDISPLAY_SVNT_NTRACE */

#if (BMDISPLAY_SVNT_NTRACE == 1)
#  define BMDISPLAY_SVNT_TRACE(X)
#else /* (BMDISPLAY_SVNT_NTRACE == 1) */
#  define BMDISPLAY_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (BMDISPLAY_SVNT_NTRACE == 1) */

#endif /* BMDISPLAY_SVNT_EXPORT_H */

// End of auto generated file.
