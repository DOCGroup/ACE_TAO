
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl CIAO_NAM
// ------------------------------
#ifndef CIAO_NAM_EXPORT_H
#define CIAO_NAM_EXPORT_H

#include "ace/config-all.h"

#if !defined (CIAO_NAM_HAS_DLL)
#  define CIAO_NAM_HAS_DLL 1
#endif /* ! CIAO_NAM_HAS_DLL */

#if defined (CIAO_NAM_HAS_DLL) && (CIAO_NAM_HAS_DLL == 1)
#  if defined (CIAO_NAM_BUILD_DLL)
#    define CIAO_NAM_Export ACE_Proper_Export_Flag
#    define CIAO_NAM_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define CIAO_NAM_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* CIAO_NAM_BUILD_DLL */
#    define CIAO_NAM_Export ACE_Proper_Import_Flag
#    define CIAO_NAM_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define CIAO_NAM_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* CIAO_NAM_BUILD_DLL */
#else /* CIAO_NAM_HAS_DLL == 1 */
#  define CIAO_NAM_Export
#  define CIAO_NAM_SINGLETON_DECLARATION(T)
#  define CIAO_NAM_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* CIAO_NAM_HAS_DLL == 1 */

// Set CIAO_NAM_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (CIAO_NAM_NTRACE)
#  if (ACE_NTRACE == 1)
#    define CIAO_NAM_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define CIAO_NAM_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !CIAO_NAM_NTRACE */

#if (CIAO_NAM_NTRACE == 1)
#  define CIAO_NAM_TRACE(X)
#else /* (CIAO_NAM_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define CIAO_NAM_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (CIAO_NAM_NTRACE == 1) */

#endif /* CIAO_NAM_EXPORT_H */

// End of auto generated file.
