
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl TAO_RTScheduler
// ------------------------------
#ifndef TAO_RTSCHEDULER_EXPORT_H
#define TAO_RTSCHEDULER_EXPORT_H

#include "ace/config-all.h"

#if !defined (TAO_RTSCHEDULER_HAS_DLL)
#  define TAO_RTSCHEDULER_HAS_DLL 1
#endif /* ! TAO_RTSCHEDULER_HAS_DLL */

#if defined (TAO_RTSCHEDULER_HAS_DLL) && (TAO_RTSCHEDULER_HAS_DLL == 1)
#  if defined (TAO_RTSCHEDULER_BUILD_DLL)
#    define TAO_RTScheduler_Export ACE_Proper_Export_Flag
#    define TAO_RTSCHEDULER_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TAO_RTSCHEDULER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TAO_RTSCHEDULER_BUILD_DLL */
#    define TAO_RTScheduler_Export ACE_Proper_Import_Flag
#    define TAO_RTSCHEDULER_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TAO_RTSCHEDULER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TAO_RTSCHEDULER_BUILD_DLL */
#else /* TAO_RTSCHEDULER_HAS_DLL == 1 */
#  define TAO_RTScheduler_Export
#  define TAO_RTSCHEDULER_SINGLETON_DECLARATION(T)
#  define TAO_RTSCHEDULER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TAO_RTSCHEDULER_HAS_DLL == 1 */

// Set TAO_RTSCHEDULER_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (TAO_RTSCHEDULER_NTRACE)
#  if (ACE_NTRACE == 1)
#    define TAO_RTSCHEDULER_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define TAO_RTSCHEDULER_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !TAO_RTSCHEDULER_NTRACE */

#if (TAO_RTSCHEDULER_NTRACE == 1)
#  define TAO_RTSCHEDULER_TRACE(X)
#else /* (TAO_RTSCHEDULER_NTRACE == 1) */
#  define TAO_RTSCHEDULER_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (TAO_RTSCHEDULER_NTRACE == 1) */

#endif /* TAO_RTSCHEDULER_EXPORT_H */

// End of auto generated file.
