
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl ACE_TMCast
// ------------------------------
#ifndef TMCAST_EXPORT_H
#define TMCAST_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (TMCAST_HAS_DLL)
#  define TMCAST_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && TMCAST_HAS_DLL */

#if !defined (TMCAST_HAS_DLL)
#define TMCAST_HAS_DLL 1
#endif /* ! TMCAST_HAS_DLL */

#if defined (TMCAST_HAS_DLL) && (TMCAST_HAS_DLL == 1)
#  if defined (TMCAST_BUILD_DLL)
#    define ACE_TMCast_Export ACE_Proper_Export_Flag
#    define TMCAST_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TMCAST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TMCAST_BUILD_DLL */
#    define ACE_TMCast_Export ACE_Proper_Import_Flag
#    define TMCAST_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TMCAST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TMCAST_BUILD_DLL */
#else /* TMCAST_HAS_DLL == 1 */
#  define ACE_TMCast_Export
#  define TMCAST_SINGLETON_DECLARATION(T)
#  define TMCAST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TMCAST_HAS_DLL == 1 */

// Set TMCAST_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (TMCAST_NTRACE)
#  if (ACE_NTRACE == 1)
#    define TMCAST_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define TMCAST_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !TMCAST_NTRACE */

#if (TMCAST_NTRACE == 1)
#  define TMCAST_TRACE(X)
#else /* (TMCAST_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define TMCAST_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (TMCAST_NTRACE == 1) */

#endif /* TMCAST_EXPORT_H */

// End of auto generated file.
