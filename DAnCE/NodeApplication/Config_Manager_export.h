
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl Config_Manager
// ------------------------------
#ifndef CONFIG_MANAGER_EXPORT_H
#define CONFIG_MANAGER_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (CONFIG_MANAGER_HAS_DLL)
#  define CONFIG_MANAGER_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && CONFIG_MANAGER_HAS_DLL */

#if !defined (CONFIG_MANAGER_HAS_DLL)
#  define CONFIG_MANAGER_HAS_DLL 1
#endif /* ! CONFIG_MANAGER_HAS_DLL */

#if defined (CONFIG_MANAGER_HAS_DLL) && (CONFIG_MANAGER_HAS_DLL == 1)
#  if defined (CONFIG_MANAGER_BUILD_DLL)
#    define Config_Manager_Export ACE_Proper_Export_Flag
#    define CONFIG_MANAGER_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define CONFIG_MANAGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* CONFIG_MANAGER_BUILD_DLL */
#    define Config_Manager_Export ACE_Proper_Import_Flag
#    define CONFIG_MANAGER_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define CONFIG_MANAGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* CONFIG_MANAGER_BUILD_DLL */
#else /* CONFIG_MANAGER_HAS_DLL == 1 */
#  define Config_Manager_Export
#  define CONFIG_MANAGER_SINGLETON_DECLARATION(T)
#  define CONFIG_MANAGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* CONFIG_MANAGER_HAS_DLL == 1 */

// Set CONFIG_MANAGER_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (CONFIG_MANAGER_NTRACE)
#  if (ACE_NTRACE == 1)
#    define CONFIG_MANAGER_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define CONFIG_MANAGER_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !CONFIG_MANAGER_NTRACE */

#if (CONFIG_MANAGER_NTRACE == 1)
#  define CONFIG_MANAGER_TRACE(X)
#else /* (CONFIG_MANAGER_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define CONFIG_MANAGER_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (CONFIG_MANAGER_NTRACE == 1) */

#endif /* CONFIG_MANAGER_EXPORT_H */

// End of auto generated file.
