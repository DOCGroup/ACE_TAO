
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -f ACE_HAS_DLL -s Compass
// ------------------------------
#ifndef COMPASS_EXPORT_H
#define COMPASS_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (COMPASS_HAS_DLL)
#  define COMPASS_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && COMPASS_HAS_DLL */

#if defined (ACE_HAS_DLL)
#  if !defined (COMPASS_HAS_DLL)
#    define COMPASS_HAS_DLL 0
#  endif /* ! COMPASS_HAS_DLL */
#else
#  if !defined (COMPASS_HAS_DLL)
#    define COMPASS_HAS_DLL 1
#  endif /* ! COMPASS_HAS_DLL */
#endif

#if defined (COMPASS_HAS_DLL) && (COMPASS_HAS_DLL == 1)
#  if defined (COMPASS_BUILD_DLL)
#    define Compass_Export ACE_Proper_Export_Flag
#    define COMPASS_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define COMPASS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* COMPASS_BUILD_DLL */
#    define Compass_Export ACE_Proper_Import_Flag
#    define COMPASS_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define COMPASS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* COMPASS_BUILD_DLL */
#else /* COMPASS_HAS_DLL == 1 */
#  define Compass_Export
#  define COMPASS_SINGLETON_DECLARATION(T)
#  define COMPASS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* COMPASS_HAS_DLL == 1 */

// Set COMPASS_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (COMPASS_NTRACE)
#  if (ACE_NTRACE == 1)
#    define COMPASS_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define COMPASS_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !COMPASS_NTRACE */

#if (COMPASS_NTRACE == 1)
#  define COMPASS_TRACE(X)
#else /* (COMPASS_NTRACE == 1) */
#  define COMPASS_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (COMPASS_NTRACE == 1) */

#endif /* COMPASS_EXPORT_H */

// End of auto generated file.
