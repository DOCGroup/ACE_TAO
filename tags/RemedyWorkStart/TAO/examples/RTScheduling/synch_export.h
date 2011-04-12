
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl SYNCH
// ------------------------------
#ifndef SYNCH_EXPORT_H
#define SYNCH_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (SYNCH_HAS_DLL)
#  define SYNCH_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && SYNCH_HAS_DLL */

#if !defined (SYNCH_HAS_DLL)
#  define SYNCH_HAS_DLL 1
#endif /* ! SYNCH_HAS_DLL */

#if defined (SYNCH_HAS_DLL) && (SYNCH_HAS_DLL == 1)
#  if defined (SYNCH_BUILD_DLL)
#    define SYNCH_Export ACE_Proper_Export_Flag
#    define SYNCH_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define SYNCH_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* SYNCH_BUILD_DLL */
#    define SYNCH_Export ACE_Proper_Import_Flag
#    define SYNCH_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define SYNCH_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* SYNCH_BUILD_DLL */
#else /* SYNCH_HAS_DLL == 1 */
#  define SYNCH_Export
#  define SYNCH_SINGLETON_DECLARATION(T)
#  define SYNCH_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* SYNCH_HAS_DLL == 1 */

// Set SYNCH_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (SYNCH_NTRACE)
#  if (ACE_NTRACE == 1)
#    define SYNCH_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define SYNCH_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !SYNCH_NTRACE */

#if (SYNCH_NTRACE == 1)
#  define SYNCH_TRACE(X)
#else /* (SYNCH_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define SYNCH_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (SYNCH_NTRACE == 1) */

#endif /* SYNCH_EXPORT_H */

// End of auto generated file.
