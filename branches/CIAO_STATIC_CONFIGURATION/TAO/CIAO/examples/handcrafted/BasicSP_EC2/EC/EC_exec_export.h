
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl EC_EXEC
// ------------------------------
#ifndef EC_EXEC_EXPORT_H
#define EC_EXEC_EXPORT_H

#include "ace/config-all.h"

#if !defined (EC_EXEC_HAS_DLL)
#  define EC_EXEC_HAS_DLL 1
#endif /* ! EC_EXEC_HAS_DLL */

#if defined (EC_EXEC_HAS_DLL) && (EC_EXEC_HAS_DLL == 1)
#  if defined (EC_EXEC_BUILD_DLL)
#    define EC_EXEC_Export ACE_Proper_Export_Flag
#    define EC_EXEC_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define EC_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* EC_EXEC_BUILD_DLL */
#    define EC_EXEC_Export ACE_Proper_Import_Flag
#    define EC_EXEC_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define EC_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* EC_EXEC_BUILD_DLL */
#else /* EC_EXEC_HAS_DLL == 1 */
#  define EC_EXEC_Export
#  define EC_EXEC_SINGLETON_DECLARATION(T)
#  define EC_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* EC_EXEC_HAS_DLL == 1 */

// Set EC_EXEC_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (EC_EXEC_NTRACE)
#  if (ACE_NTRACE == 1)
#    define EC_EXEC_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define EC_EXEC_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !EC_EXEC_NTRACE */

#if (EC_EXEC_NTRACE == 1)
#  define EC_EXEC_TRACE(X)
#else /* (EC_EXEC_NTRACE == 1) */
#  define EC_EXEC_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (EC_EXEC_NTRACE == 1) */

#endif /* EC_EXEC_EXPORT_H */

// End of auto generated file.
