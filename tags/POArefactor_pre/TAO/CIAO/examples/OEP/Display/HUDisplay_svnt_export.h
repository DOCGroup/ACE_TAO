
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl HUDISPLAY_SVNT
// ------------------------------
#ifndef HUDISPLAY_SVNT_EXPORT_H
#define HUDISPLAY_SVNT_EXPORT_H

#include "ace/config-all.h"

#if !defined (HUDISPLAY_SVNT_HAS_DLL)
#  define HUDISPLAY_SVNT_HAS_DLL 1
#endif /* ! HUDISPLAY_SVNT_HAS_DLL */

#if defined (HUDISPLAY_SVNT_HAS_DLL) && (HUDISPLAY_SVNT_HAS_DLL == 1)
#  if defined (HUDISPLAY_SVNT_BUILD_DLL)
#    define HUDISPLAY_SVNT_Export ACE_Proper_Export_Flag
#    define HUDISPLAY_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define HUDISPLAY_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* HUDISPLAY_SVNT_BUILD_DLL */
#    define HUDISPLAY_SVNT_Export ACE_Proper_Import_Flag
#    define HUDISPLAY_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define HUDISPLAY_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* HUDISPLAY_SVNT_BUILD_DLL */
#else /* HUDISPLAY_SVNT_HAS_DLL == 1 */
#  define HUDISPLAY_SVNT_Export
#  define HUDISPLAY_SVNT_SINGLETON_DECLARATION(T)
#  define HUDISPLAY_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* HUDISPLAY_SVNT_HAS_DLL == 1 */

// Set HUDISPLAY_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (HUDISPLAY_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define HUDISPLAY_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define HUDISPLAY_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !HUDISPLAY_SVNT_NTRACE */

#if (HUDISPLAY_SVNT_NTRACE == 1)
#  define HUDISPLAY_SVNT_TRACE(X)
#else /* (HUDISPLAY_SVNT_NTRACE == 1) */
#  define HUDISPLAY_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (HUDISPLAY_SVNT_NTRACE == 1) */

#endif /* HUDISPLAY_SVNT_EXPORT_H */

// End of auto generated file.
