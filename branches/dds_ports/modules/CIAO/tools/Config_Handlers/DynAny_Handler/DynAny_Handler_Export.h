
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl DynAny_Handler
// ------------------------------
#ifndef DYNANY_HANDLER_EXPORT_H
#define DYNANY_HANDLER_EXPORT_H

#include "ace/config-all.h"

#if !defined (DYNANY_HANDLER_HAS_DLL)
#  define DYNANY_HANDLER_HAS_DLL 1
#endif /* ! DYNANY_HANDLER_HAS_DLL */

#if defined (DYNANY_HANDLER_HAS_DLL) && (DYNANY_HANDLER_HAS_DLL == 1)
#  if defined (DYNANY_HANDLER_BUILD_DLL)
#    define DynAny_Handler_Export ACE_Proper_Export_Flag
#    define DYNANY_HANDLER_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define DYNANY_HANDLER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* DYNANY_HANDLER_BUILD_DLL */
#    define DynAny_Handler_Export ACE_Proper_Import_Flag
#    define DYNANY_HANDLER_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define DYNANY_HANDLER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* DYNANY_HANDLER_BUILD_DLL */
#else /* DYNANY_HANDLER_HAS_DLL == 1 */
#  define DynAny_Handler_Export
#  define DYNANY_HANDLER_SINGLETON_DECLARATION(T)
#  define DYNANY_HANDLER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* DYNANY_HANDLER_HAS_DLL == 1 */

// Set DYNANY_HANDLER_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (DYNANY_HANDLER_NTRACE)
#  if (ACE_NTRACE == 1)
#    define DYNANY_HANDLER_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define DYNANY_HANDLER_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !DYNANY_HANDLER_NTRACE */

#if (DYNANY_HANDLER_NTRACE == 1)
#  define DYNANY_HANDLER_TRACE(X)
#else /* (DYNANY_HANDLER_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define DYNANY_HANDLER_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (DYNANY_HANDLER_NTRACE == 1) */

#endif /* DYNANY_HANDLER_EXPORT_H */

// End of auto generated file.
