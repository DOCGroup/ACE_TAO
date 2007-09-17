
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl EVENT_TYPES_EXEC
// ------------------------------
#ifndef EVENT_TYPES_EXEC_EXPORT_H
#define EVENT_TYPES_EXEC_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (EVENT_TYPES_EXEC_HAS_DLL)
#  define EVENT_TYPES_EXEC_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && EVENT_TYPES_EXEC_HAS_DLL */

#if !defined (EVENT_TYPES_EXEC_HAS_DLL)
#  define EVENT_TYPES_EXEC_HAS_DLL 1
#endif /* ! EVENT_TYPES_EXEC_HAS_DLL */

#if defined (EVENT_TYPES_EXEC_HAS_DLL) && (EVENT_TYPES_EXEC_HAS_DLL == 1)
#  if defined (EVENT_TYPES_EXEC_BUILD_DLL)
#    define EVENT_TYPES_EXEC_Export ACE_Proper_Export_Flag
#    define EVENT_TYPES_EXEC_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define EVENT_TYPES_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* EVENT_TYPES_EXEC_BUILD_DLL */
#    define EVENT_TYPES_EXEC_Export ACE_Proper_Import_Flag
#    define EVENT_TYPES_EXEC_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define EVENT_TYPES_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* EVENT_TYPES_EXEC_BUILD_DLL */
#else /* EVENT_TYPES_EXEC_HAS_DLL == 1 */
#  define EVENT_TYPES_EXEC_Export
#  define EVENT_TYPES_EXEC_SINGLETON_DECLARATION(T)
#  define EVENT_TYPES_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* EVENT_TYPES_EXEC_HAS_DLL == 1 */

// Set EVENT_TYPES_EXEC_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (EVENT_TYPES_EXEC_NTRACE)
#  if (ACE_NTRACE == 1)
#    define EVENT_TYPES_EXEC_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define EVENT_TYPES_EXEC_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !EVENT_TYPES_EXEC_NTRACE */

#if (EVENT_TYPES_EXEC_NTRACE == 1)
#  define EVENT_TYPES_EXEC_TRACE(X)
#else /* (EVENT_TYPES_EXEC_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define EVENT_TYPES_EXEC_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (EVENT_TYPES_EXEC_NTRACE == 1) */

#endif /* EVENT_TYPES_EXEC_EXPORT_H */

// End of auto generated file.
