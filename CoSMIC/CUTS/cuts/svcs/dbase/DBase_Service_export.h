
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl DBASE_SVC
// ------------------------------
#ifndef DBASE_SVC_EXPORT_H
#define DBASE_SVC_EXPORT_H

#include "ace/config-all.h"

#if !defined (DBASE_SVC_HAS_DLL)
#  define DBASE_SVC_HAS_DLL 1
#endif /* ! DBASE_SVC_HAS_DLL */

#if defined (DBASE_SVC_HAS_DLL) && (DBASE_SVC_HAS_DLL == 1)
#  if defined (DBASE_SVC_BUILD_DLL)
#    define DBASE_SVC_Export ACE_Proper_Export_Flag
#    define DBASE_SVC_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define DBASE_SVC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* DBASE_SVC_BUILD_DLL */
#    define DBASE_SVC_Export ACE_Proper_Import_Flag
#    define DBASE_SVC_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define DBASE_SVC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* DBASE_SVC_BUILD_DLL */
#else /* DBASE_SVC_HAS_DLL == 1 */
#  define DBASE_SVC_Export
#  define DBASE_SVC_SINGLETON_DECLARATION(T)
#  define DBASE_SVC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* DBASE_SVC_HAS_DLL == 1 */

// Set DBASE_SVC_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (DBASE_SVC_NTRACE)
#  if (ACE_NTRACE == 1)
#    define DBASE_SVC_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define DBASE_SVC_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !DBASE_SVC_NTRACE */

#if (DBASE_SVC_NTRACE == 1)
#  define DBASE_SVC_TRACE(X)
#else /* (DBASE_SVC_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define DBASE_SVC_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (DBASE_SVC_NTRACE == 1) */

#endif /* DBASE_SVC_EXPORT_H */

// End of auto generated file.
