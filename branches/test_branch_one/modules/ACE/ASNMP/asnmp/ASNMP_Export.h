
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl ASNMP
// ------------------------------
#ifndef ASNMP_EXPORT_H
#define ASNMP_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (ASNMP_HAS_DLL)
#  define ASNMP_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && ! ASNMP_HAS_DLL */

#if !defined (ASNMP_HAS_DLL)
#  define ASNMP_HAS_DLL 1
#endif /* ! ASNMP_HAS_DLL */

#if defined (ASNMP_HAS_DLL) && (ASNMP_HAS_DLL == 1)
#  if defined (ASNMP_BUILD_DLL)
#    define ASNMP_Export ACE_Proper_Export_Flag
#    define ASNMP_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define ASNMP_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* ASNMP_BUILD_DLL */
#    define ASNMP_Export ACE_Proper_Import_Flag
#    define ASNMP_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define ASNMP_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* ASNMP_BUILD_DLL */
#else /* ASNMP_HAS_DLL == 1 */
#  define ASNMP_Export
#  define ASNMP_SINGLETON_DECLARATION(T)
#  define ASNMP_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* ASNMP_HAS_DLL == 1 */

// Set ASNMP_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (ASNMP_NTRACE)
#  if (ACE_NTRACE == 1)
#    define ASNMP_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define ASNMP_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !ASNMP_NTRACE */

#if (ASNMP_NTRACE == 1)
#  define ASNMP_TRACE(X)
#else /* (ASNMP_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define ASNMP_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (ASNMP_NTRACE == 1) */

#endif /* ASNMP_EXPORT_H */

// End of auto generated file.
