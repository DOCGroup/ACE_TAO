// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl catior
// ------------------------------
#ifndef CATIOR_EXPORT_H
#define CATIOR_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (CATIOR_HAS_DLL)
#  define CATIOR_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && CATIOR_HAS_DLL */

#if !defined (CATIOR_HAS_DLL)
#  define CATIOR_HAS_DLL 1
#endif /* ! CATIOR_HAS_DLL */

#if defined (CATIOR_HAS_DLL) && (CATIOR_HAS_DLL == 1)
#  if defined (CATIOR_BUILD_DLL)
#    define catior_Export ACE_Proper_Export_Flag
#    define CATIOR_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define CATIOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* CATIOR_BUILD_DLL */
#    define catior_Export ACE_Proper_Import_Flag
#    define CATIOR_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define CATIOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* CATIOR_BUILD_DLL */
#else /* CATIOR_HAS_DLL == 1 */
#  define catior_Export
#  define CATIOR_SINGLETON_DECLARATION(T)
#  define CATIOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* CATIOR_HAS_DLL == 1 */

// Set CATIOR_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (CATIOR_NTRACE)
#  if (ACE_NTRACE == 1)
#    define CATIOR_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define CATIOR_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !CATIOR_NTRACE */

#if (CATIOR_NTRACE == 1)
#  define CATIOR_TRACE(X)
#else /* (CATIOR_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define CATIOR_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (CATIOR_NTRACE == 1) */

#endif /* CATIOR_EXPORT_H */

// End of auto generated file.
