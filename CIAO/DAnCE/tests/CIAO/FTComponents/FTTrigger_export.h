
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl FTTRIGGER
// ------------------------------
#ifndef FTTRIGGER_EXPORT_H
#define FTTRIGGER_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (FTTRIGGER_HAS_DLL)
#  define FTTRIGGER_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && FTTRIGGER_HAS_DLL */

#if !defined (FTTRIGGER_HAS_DLL)
#  define FTTRIGGER_HAS_DLL 1
#endif /* ! FTTRIGGER_HAS_DLL */

#if defined (FTTRIGGER_HAS_DLL) && (FTTRIGGER_HAS_DLL == 1)
#  if defined (FTTRIGGER_BUILD_DLL)
#    define FTTRIGGER_Export ACE_Proper_Export_Flag
#    define FTTRIGGER_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define FTTRIGGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* FTTRIGGER_BUILD_DLL */
#    define FTTRIGGER_Export ACE_Proper_Import_Flag
#    define FTTRIGGER_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define FTTRIGGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* FTTRIGGER_BUILD_DLL */
#else /* FTTRIGGER_HAS_DLL == 1 */
#  define FTTRIGGER_Export
#  define FTTRIGGER_SINGLETON_DECLARATION(T)
#  define FTTRIGGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* FTTRIGGER_HAS_DLL == 1 */

// Set FTTRIGGER_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (FTTRIGGER_NTRACE)
#  if (ACE_NTRACE == 1)
#    define FTTRIGGER_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define FTTRIGGER_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !FTTRIGGER_NTRACE */

#if (FTTRIGGER_NTRACE == 1)
#  define FTTRIGGER_TRACE(X)
#else /* (FTTRIGGER_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define FTTRIGGER_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (FTTRIGGER_NTRACE == 1) */

#endif /* FTTRIGGER_EXPORT_H */

// End of auto generated file.
