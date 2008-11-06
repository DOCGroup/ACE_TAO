
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl CI
// ------------------------------
#ifndef CI_EXPORT_H
#define CI_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (CI_HAS_DLL)
#  define CI_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && CI_HAS_DLL */

#if !defined (CI_HAS_DLL)
#  define CI_HAS_DLL 1
#endif /* ! CI_HAS_DLL */

#if defined (CI_HAS_DLL) && (CI_HAS_DLL == 1)
#  if defined (CI_BUILD_DLL)
#    define CI_Export ACE_Proper_Export_Flag
#    define CI_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define CI_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* CI_BUILD_DLL */
#    define CI_Export ACE_Proper_Import_Flag
#    define CI_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define CI_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* CI_BUILD_DLL */
#else /* CI_HAS_DLL == 1 */
#  define CI_Export
#  define CI_SINGLETON_DECLARATION(T)
#  define CI_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* CI_HAS_DLL == 1 */

// Set CI_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (CI_NTRACE)
#  if (ACE_NTRACE == 1)
#    define CI_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define CI_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !CI_NTRACE */

#if (CI_NTRACE == 1)
#  define CI_TRACE(X)
#else /* (CI_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define CI_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (CI_NTRACE == 1) */

#endif /* CI_EXPORT_H */

// End of auto generated file.
