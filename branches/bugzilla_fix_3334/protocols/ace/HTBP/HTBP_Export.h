// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -d -s HTBP
// ------------------------------
#ifndef HTBP_EXPORT_H
#define HTBP_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (HTBP_HAS_DLL)
#  define HTBP_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && HTBP_HAS_DLL */

#if !defined (HTBP_HAS_DLL)
#  define HTBP_HAS_DLL 1
#endif /* ! HTBP_HAS_DLL */

#if defined (HTBP_HAS_DLL) && (HTBP_HAS_DLL == 1)
#  if defined (HTBP_BUILD_DLL)
#    define HTBP_Export ACE_Proper_Export_Flag
#    define HTBP_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define HTBP_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* HTBP_BUILD_DLL */
#    define HTBP_Export ACE_Proper_Import_Flag
#    define HTBP_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define HTBP_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* HTBP_BUILD_DLL */
#else /* HTBP_HAS_DLL == 1 */
#  define HTBP_Export
#  define HTBP_SINGLETON_DECLARATION(T)
#  define HTBP_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* HTBP_HAS_DLL == 1 */

// Set HTBP_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (HTBP_NTRACE)
#  if (ACE_NTRACE == 1)
#    define HTBP_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define HTBP_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !HTBP_NTRACE */

#if (HTBP_NTRACE == 1)
#  define HTBP_TRACE(X)
#else /* (HTBP_NTRACE == 1) */
#  define HTBP_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (HTBP_NTRACE == 1) */

#endif /* HTBP_EXPORT_H */

// End of auto generated file.
