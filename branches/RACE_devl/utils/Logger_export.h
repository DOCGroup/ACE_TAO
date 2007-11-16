
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl LOGGER
// ------------------------------
#ifndef LOGGER_EXPORT_H
#define LOGGER_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (LOGGER_HAS_DLL)
#  define LOGGER_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && LOGGER_HAS_DLL */

#if !defined (LOGGER_HAS_DLL)
#  define LOGGER_HAS_DLL 1
#endif /* ! LOGGER_HAS_DLL */

#if defined (LOGGER_HAS_DLL) && (LOGGER_HAS_DLL == 1)
#  if defined (LOGGER_BUILD_DLL)
#    define LOGGER_Export ACE_Proper_Export_Flag
#    define LOGGER_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define LOGGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* LOGGER_BUILD_DLL */
#    define LOGGER_Export ACE_Proper_Import_Flag
#    define LOGGER_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define LOGGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* LOGGER_BUILD_DLL */
#else /* LOGGER_HAS_DLL == 1 */
#  define LOGGER_Export
#  define LOGGER_SINGLETON_DECLARATION(T)
#  define LOGGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* LOGGER_HAS_DLL == 1 */

// Set LOGGER_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (LOGGER_NTRACE)
#  if (ACE_NTRACE == 1)
#    define LOGGER_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define LOGGER_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !LOGGER_NTRACE */

#if (LOGGER_NTRACE == 1)
#  define LOGGER_TRACE(X)
#else /* (LOGGER_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define LOGGER_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (LOGGER_NTRACE == 1) */

#endif /* LOGGER_EXPORT_H */

// End of auto generated file.
