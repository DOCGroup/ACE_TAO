
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl RT_CCM_Handlers
// ------------------------------
#ifndef RT_CCM_HANDLERS_EXPORT_H
#define RT_CCM_HANDLERS_EXPORT_H

#include "ace/config-all.h"

#if !defined (RT_CCM_HANDLERS_HAS_DLL)
#  define RT_CCM_HANDLERS_HAS_DLL 1
#endif /* ! RT_CCM_HANDLERS_HAS_DLL */

#if defined (RT_CCM_HANDLERS_HAS_DLL) && (RT_CCM_HANDLERS_HAS_DLL == 1)
#  if defined (RT_CCM_HANDLERS_BUILD_DLL)
#    define RT_CCM_Handlers_Export ACE_Proper_Export_Flag
#    define RT_CCM_HANDLERS_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define RT_CCM_HANDLERS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* RT_CCM_HANDLERS_BUILD_DLL */
#    define RT_CCM_Handlers_Export ACE_Proper_Import_Flag
#    define RT_CCM_HANDLERS_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define RT_CCM_HANDLERS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* RT_CCM_HANDLERS_BUILD_DLL */
#else /* RT_CCM_HANDLERS_HAS_DLL == 1 */
#  define RT_CCM_Handlers_Export
#  define RT_CCM_HANDLERS_SINGLETON_DECLARATION(T)
#  define RT_CCM_HANDLERS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* RT_CCM_HANDLERS_HAS_DLL == 1 */

// Set RT_CCM_HANDLERS_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (RT_CCM_HANDLERS_NTRACE)
#  if (ACE_NTRACE == 1)
#    define RT_CCM_HANDLERS_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define RT_CCM_HANDLERS_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !RT_CCM_HANDLERS_NTRACE */

#if (RT_CCM_HANDLERS_NTRACE == 1)
#  define RT_CCM_HANDLERS_TRACE(X)
#else /* (RT_CCM_HANDLERS_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define RT_CCM_HANDLERS_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (RT_CCM_HANDLERS_NTRACE == 1) */

#endif /* RT_CCM_HANDLERS_EXPORT_H */

// End of auto generated file.
