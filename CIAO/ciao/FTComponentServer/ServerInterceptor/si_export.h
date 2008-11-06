// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl SI
// ------------------------------
#ifndef SI_EXPORT_H
#define SI_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (SI_HAS_DLL)
#  define SI_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && SI_HAS_DLL */

#if !defined (SI_HAS_DLL)
#  define SI_HAS_DLL 1
#endif /* ! SI_HAS_DLL */

#if defined (SI_HAS_DLL) && (SI_HAS_DLL == 1)
#  if defined (SI_BUILD_DLL)
#    define SI_Export ACE_Proper_Export_Flag
#    define SI_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define SI_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* SI_BUILD_DLL */
#    define SI_Export ACE_Proper_Import_Flag
#    define SI_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define SI_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* SI_BUILD_DLL */
#else /* SI_HAS_DLL == 1 */
#  define SI_Export
#  define SI_SINGLETON_DECLARATION(T)
#  define SI_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* SI_HAS_DLL == 1 */

// Set SI_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (SI_NTRACE)
#  if (ACE_NTRACE == 1)
#    define SI_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define SI_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !SI_NTRACE */

#if (SI_NTRACE == 1)
#  define SI_TRACE(X)
#else /* (SI_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define SI_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (SI_NTRACE == 1) */

#endif /* SI_EXPORT_H */

// End of auto generated file.
