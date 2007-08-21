
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl STOCK_BASE_SKEL
// ------------------------------
#ifndef STOCK_BASE_SKEL_EXPORT_H
#define STOCK_BASE_SKEL_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (STOCK_BASE_SKEL_HAS_DLL)
#  define STOCK_BASE_SKEL_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && STOCK_BASE_SKEL_HAS_DLL */

#if !defined (STOCK_BASE_SKEL_HAS_DLL)
#  define STOCK_BASE_SKEL_HAS_DLL 1
#endif /* ! STOCK_BASE_SKEL_HAS_DLL */

#if defined (STOCK_BASE_SKEL_HAS_DLL) && (STOCK_BASE_SKEL_HAS_DLL == 1)
#  if defined (STOCK_BASE_SKEL_BUILD_DLL)
#    define STOCK_BASE_SKEL_Export ACE_Proper_Export_Flag
#    define STOCK_BASE_SKEL_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define STOCK_BASE_SKEL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* STOCK_BASE_SKEL_BUILD_DLL */
#    define STOCK_BASE_SKEL_Export ACE_Proper_Import_Flag
#    define STOCK_BASE_SKEL_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define STOCK_BASE_SKEL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* STOCK_BASE_SKEL_BUILD_DLL */
#else /* STOCK_BASE_SKEL_HAS_DLL == 1 */
#  define STOCK_BASE_SKEL_Export
#  define STOCK_BASE_SKEL_SINGLETON_DECLARATION(T)
#  define STOCK_BASE_SKEL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* STOCK_BASE_SKEL_HAS_DLL == 1 */

// Set STOCK_BASE_SKEL_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (STOCK_BASE_SKEL_NTRACE)
#  if (ACE_NTRACE == 1)
#    define STOCK_BASE_SKEL_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define STOCK_BASE_SKEL_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !STOCK_BASE_SKEL_NTRACE */

#if (STOCK_BASE_SKEL_NTRACE == 1)
#  define STOCK_BASE_SKEL_TRACE(X)
#else /* (STOCK_BASE_SKEL_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define STOCK_BASE_SKEL_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (STOCK_BASE_SKEL_NTRACE == 1) */

#endif /* STOCK_BASE_SKEL_EXPORT_H */

// End of auto generated file.
