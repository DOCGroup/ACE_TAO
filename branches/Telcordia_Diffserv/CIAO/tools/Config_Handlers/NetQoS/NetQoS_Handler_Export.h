
// -*- C++ -*-
// $Id: $
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl NetQoS_Handler
// ------------------------------
#ifndef NETQOS_HANDLER_EXPORT_H
#define NETQOS_HANDLER_EXPORT_H

#include "ace/config-all.h"

#if !defined (NETQOS_HANDLER_HAS_DLL)
#  define NETQOS_HANDLER_HAS_DLL 1
#endif /* ! NETQOS_HANDLER_HAS_DLL */

#if defined (NETQOS_HANDLER_HAS_DLL) && (NETQOS_HANDLER_HAS_DLL == 1)
#  if defined (NETQOS_HANDLER_BUILD_DLL)
#    define NetQoS_Handler_Export ACE_Proper_Export_Flag
#    define NETQOS_HANDLER_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define NETQOS_HANDLER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* NETQOS_HANDLER_BUILD_DLL */
#    define NetQoS_Handler_Export ACE_Proper_Import_Flag
#    define NETQOS_HANDLER_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define NETQOS_HANDLER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* NETQOS_HANDLER_BUILD_DLL */
#else /* NETQOS_HANDLER_HAS_DLL == 1 */
#  define NetQoS_Handler_Export
#  define NETQOS_HANDLER_SINGLETON_DECLARATION(T)
#  define NETQOS_HANDLER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* NETQOS_HANDLER_HAS_DLL == 1 */

// Set NETQOS_HANDLER_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (NETQOS_HANDLER_NTRACE)
#  if (ACE_NTRACE == 1)
#    define NETQOS_HANDLER_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define NETQOS_HANDLER_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !NETQOS_HANDLER_NTRACE */

#if (NETQOS_HANDLER_NTRACE == 1)
#  define NETQOS_HANDLER_TRACE(X)
#else /* (NETQOS_HANDLER_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define NETQOS_HANDLER_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (NETQOS_HANDLER_NTRACE == 1) */

#endif /* NETQOS_HANDLER_EXPORT_H */

// End of auto generated file.
