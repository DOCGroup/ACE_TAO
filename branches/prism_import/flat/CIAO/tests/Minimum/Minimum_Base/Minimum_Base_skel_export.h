
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl MINIMUM_BASE_SVNT
// ------------------------------
#ifndef MINIMUM_BASE_SKEL_EXPORT_H
#define MINIMUM_BASE_SKEL_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (MINIMUM_BASE_SKEL_HAS_DLL)
#  define MINIMUM_BASE_SKEL_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && MINIMUM_BASE_SKEL_HAS_DLL */

#if !defined (MINIMUM_BASE_SKEL_HAS_DLL)
#  define MINIMUM_BASE_SKEL_HAS_DLL 1
#endif /* ! MINIMUM_BASE_SKEL_HAS_DLL */

#if defined (MINIMUM_BASE_SKEL_HAS_DLL) && (MINIMUM_BASE_SKEL_HAS_DLL == 1)
#  if defined (MINIMUM_BASE_SKEL_BUILD_DLL)
#    define MINIMUM_BASE_SKEL_Export ACE_Proper_Export_Flag
#    define MINIMUM_BASE_SKEL_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define MINIMUM_BASE_SKEL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* MINIMUM_BASE_SKEL_BUILD_DLL */
#    define MINIMUM_BASE_SKEL_Export ACE_Proper_Import_Flag
#    define MINIMUM_BASE_SKEL_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define MINIMUM_BASE_SKEL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* MINIMUM_BASE_SKEL_BUILD_DLL */
#else /* MINIMUM_BASE_SKEL_HAS_DLL == 1 */
#  define MINIMUM_BASE_SKEL_Export
#  define MINIMUM_BASE_SKEL_SINGLETON_DECLARATION(T)
#  define MINIMUM_BASE_SKEL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* MINIMUM_BASE_SKEL_HAS_DLL == 1 */

// Set MINIMUM_BASE_SKEL_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (MINIMUM_BASE_SKEL_NTRACE)
#  if (ACE_NTRACE == 1)
#    define MINIMUM_BASE_SKEL_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define MINIMUM_BASE_SKEL_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !MINIMUM_BASE_SKEL_NTRACE */

#if (MINIMUM_BASE_SKEL_NTRACE == 1)
#  define MINIMUM_BASE_SKEL_TRACE(X)
#else /* (MINIMUM_BASE_SKEL_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define MINIMUM_BASE_SKEL_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (MINIMUM_BASE_SKEL_NTRACE == 1) */

#endif /* MINIMUM_BASE_SKEL_EXPORT_H */

// End of auto generated file.
