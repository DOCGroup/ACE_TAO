
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl NodeApplication
// ------------------------------
#ifndef NODEAPPLICATION_EXPORT_H
#define NODEAPPLICATION_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (NODEAPPLICATION_HAS_DLL)
#  define NODEAPPLICATION_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && NODEAPPLICATION_HAS_DLL */

#if !defined (NODEAPPLICATION_HAS_DLL)
#  define NODEAPPLICATION_HAS_DLL 1
#endif /* ! NODEAPPLICATION_HAS_DLL */

#if defined (NODEAPPLICATION_HAS_DLL) && (NODEAPPLICATION_HAS_DLL == 1)
#  if defined (NODEAPPLICATION_BUILD_DLL)
#    define NodeApplication_Export ACE_Proper_Export_Flag
#    define NODEAPPLICATION_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define NODEAPPLICATION_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* NODEAPPLICATION_BUILD_DLL */
#    define NodeApplication_Export ACE_Proper_Import_Flag
#    define NODEAPPLICATION_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define NODEAPPLICATION_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* NODEAPPLICATION_BUILD_DLL */
#else /* NODEAPPLICATION_HAS_DLL == 1 */
#  define NodeApplication_Export
#  define NODEAPPLICATION_SINGLETON_DECLARATION(T)
#  define NODEAPPLICATION_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* NODEAPPLICATION_HAS_DLL == 1 */

// Set NODEAPPLICATION_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (NODEAPPLICATION_NTRACE)
#  if (ACE_NTRACE == 1)
#    define NODEAPPLICATION_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define NODEAPPLICATION_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !NODEAPPLICATION_NTRACE */

#if (NODEAPPLICATION_NTRACE == 1)
#  define NODEAPPLICATION_TRACE(X)
#else /* (NODEAPPLICATION_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define NODEAPPLICATION_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (NODEAPPLICATION_NTRACE == 1) */

#endif /* NODEAPPLICATION_EXPORT_H */

// End of auto generated file.
