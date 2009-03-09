
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl State_TS
// ------------------------------
#ifndef STATE_TS_EXPORT_H
#define STATE_TS_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (STATE_TS_HAS_DLL)
#  define STATE_TS_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && STATE_TS_HAS_DLL */

#if !defined (STATE_TS_HAS_DLL)
#  define STATE_TS_HAS_DLL 1
#endif /* ! STATE_TS_HAS_DLL */

#if defined (STATE_TS_HAS_DLL) && (STATE_TS_HAS_DLL == 1)
#  if defined (STATE_TS_BUILD_DLL)
#    define State_TS_Export ACE_Proper_Export_Flag
#    define STATE_TS_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define STATE_TS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* STATE_TS_BUILD_DLL */
#    define State_TS_Export ACE_Proper_Import_Flag
#    define STATE_TS_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define STATE_TS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* STATE_TS_BUILD_DLL */
#else /* STATE_TS_HAS_DLL == 1 */
#  define State_TS_Export
#  define STATE_TS_SINGLETON_DECLARATION(T)
#  define STATE_TS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* STATE_TS_HAS_DLL == 1 */

// Set STATE_TS_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (STATE_TS_NTRACE)
#  if (ACE_NTRACE == 1)
#    define STATE_TS_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define STATE_TS_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !STATE_TS_NTRACE */

#if (STATE_TS_NTRACE == 1)
#  define STATE_TS_TRACE(X)
#else /* (STATE_TS_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define STATE_TS_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (STATE_TS_NTRACE == 1) */

#endif /* STATE_TS_EXPORT_H */

// End of auto generated file.
