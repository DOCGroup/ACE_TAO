
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s HTIOP
// ------------------------------
#ifndef HTIOP_EXPORT_H
#define HTIOP_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (HTIOP_HAS_DLL)
#  define HTIOP_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && HTIOP_HAS_DLL */

#if !defined (HTIOP_HAS_DLL)
#  define HTIOP_HAS_DLL 1
#endif /* ! HTIOP_HAS_DLL */

#if defined (HTIOP_HAS_DLL) && (HTIOP_HAS_DLL == 1)
#  if defined (HTIOP_BUILD_DLL)
#    define HTIOP_Export ACE_Proper_Export_Flag
#    define HTIOP_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define HTIOP_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* HTIOP_BUILD_DLL */
#    define HTIOP_Export ACE_Proper_Import_Flag
#    define HTIOP_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define HTIOP_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* HTIOP_BUILD_DLL */
#else /* HTIOP_HAS_DLL == 1 */
#  define HTIOP_Export
#  define HTIOP_SINGLETON_DECLARATION(T)
#  define HTIOP_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* HTIOP_HAS_DLL == 1 */

// Set HTIOP_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (HTIOP_NTRACE)
#  if (ACE_NTRACE == 1)
#    define HTIOP_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define HTIOP_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !HTIOP_NTRACE */

#if (HTIOP_NTRACE == 1)
#  define HTIOP_TRACE(X)
#else /* (HTIOP_NTRACE == 1) */
#  define HTIOP_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (HTIOP_NTRACE == 1) */

#endif /* HTIOP_EXPORT_H */

// End of auto generated file.
