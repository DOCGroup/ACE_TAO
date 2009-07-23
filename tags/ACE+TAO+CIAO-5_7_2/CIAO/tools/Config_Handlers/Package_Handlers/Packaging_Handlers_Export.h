
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl Packaging_Handlers
// ------------------------------
#ifndef PACKAGING_HANDLERS_EXPORT_H
#define PACKAGING_HANDLERS_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (PACKAGING_HANDLERS_HAS_DLL)
#  define PACKAGING_HANDLERS_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && PACKAGING_HANDLERS_HAS_DLL */

#if !defined (PACKAGING_HANDLERS_HAS_DLL)
#  define PACKAGING_HANDLERS_HAS_DLL 1
#endif /* ! PACKAGING_HANDLERS_HAS_DLL */

#if defined (PACKAGING_HANDLERS_HAS_DLL) && (PACKAGING_HANDLERS_HAS_DLL == 1)
#  if defined (PACKAGING_HANDLERS_BUILD_DLL)
#    define Packaging_Handlers_Export ACE_Proper_Export_Flag
#    define PACKAGING_HANDLERS_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define PACKAGING_HANDLERS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* PACKAGING_HANDLERS_BUILD_DLL */
#    define Packaging_Handlers_Export ACE_Proper_Import_Flag
#    define PACKAGING_HANDLERS_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define PACKAGING_HANDLERS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* PACKAGING_HANDLERS_BUILD_DLL */
#else /* PACKAGING_HANDLERS_HAS_DLL == 1 */
#  define Packaging_Handlers_Export
#  define PACKAGING_HANDLERS_SINGLETON_DECLARATION(T)
#  define PACKAGING_HANDLERS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* PACKAGING_HANDLERS_HAS_DLL == 1 */

// Set PACKAGING_HANDLERS_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (PACKAGING_HANDLERS_NTRACE)
#  if (ACE_NTRACE == 1)
#    define PACKAGING_HANDLERS_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define PACKAGING_HANDLERS_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !PACKAGING_HANDLERS_NTRACE */

#if (PACKAGING_HANDLERS_NTRACE == 1)
#  define PACKAGING_HANDLERS_TRACE(X)
#else /* (PACKAGING_HANDLERS_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define PACKAGING_HANDLERS_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (PACKAGING_HANDLERS_NTRACE == 1) */

#endif /* PACKAGING_HANDLERS_EXPORT_H */

// End of auto generated file.
