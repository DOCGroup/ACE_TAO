
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl TRIGGER_SVNT
// ------------------------------
#ifndef TRIGGER_SVNT_EXPORT_H
#define TRIGGER_SVNT_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (TRIGGER_SVNT_HAS_DLL)
#  define TRIGGER_SVNT_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && TRIGGER_SVNT_HAS_DLL */

#if !defined (TRIGGER_SVNT_HAS_DLL)
#  define TRIGGER_SVNT_HAS_DLL 1
#endif /* ! TRIGGER_SVNT_HAS_DLL */

#if defined (TRIGGER_SVNT_HAS_DLL) && (TRIGGER_SVNT_HAS_DLL == 1)
#  if defined (TRIGGER_SVNT_BUILD_DLL)
#    define TRIGGER_SVNT_Export ACE_Proper_Export_Flag
#    define TRIGGER_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TRIGGER_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TRIGGER_SVNT_BUILD_DLL */
#    define TRIGGER_SVNT_Export ACE_Proper_Import_Flag
#    define TRIGGER_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TRIGGER_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TRIGGER_SVNT_BUILD_DLL */
#else /* TRIGGER_SVNT_HAS_DLL == 1 */
#  define TRIGGER_SVNT_Export
#  define TRIGGER_SVNT_SINGLETON_DECLARATION(T)
#  define TRIGGER_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TRIGGER_SVNT_HAS_DLL == 1 */

// Set TRIGGER_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (TRIGGER_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define TRIGGER_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define TRIGGER_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !TRIGGER_SVNT_NTRACE */

#if (TRIGGER_SVNT_NTRACE == 1)
#  define TRIGGER_SVNT_TRACE(X)
#else /* (TRIGGER_SVNT_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define TRIGGER_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (TRIGGER_SVNT_NTRACE == 1) */

#endif /* TRIGGER_SVNT_EXPORT_H */

// End of auto generated file.
