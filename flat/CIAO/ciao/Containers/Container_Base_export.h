
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl Container_Base
// ------------------------------
#ifndef CONTAINER_BASE_EXPORT_H
#define CONTAINER_BASE_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (CONTAINER_BASE_HAS_DLL)
#  define CONTAINER_BASE_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && CONTAINER_BASE_HAS_DLL */

#if !defined (CONTAINER_BASE_HAS_DLL)
#  define CONTAINER_BASE_HAS_DLL 1
#endif /* ! CONTAINER_BASE_HAS_DLL */

#if defined (CONTAINER_BASE_HAS_DLL) && (CONTAINER_BASE_HAS_DLL == 1)
#  if defined (CONTAINER_BASE_BUILD_DLL)
#    define Container_Base_Export ACE_Proper_Export_Flag
#    define CONTAINER_BASE_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define CONTAINER_BASE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* CONTAINER_BASE_BUILD_DLL */
#    define Container_Base_Export ACE_Proper_Import_Flag
#    define CONTAINER_BASE_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define CONTAINER_BASE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* CONTAINER_BASE_BUILD_DLL */
#else /* CONTAINER_BASE_HAS_DLL == 1 */
#  define Container_Base_Export
#  define CONTAINER_BASE_SINGLETON_DECLARATION(T)
#  define CONTAINER_BASE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* CONTAINER_BASE_HAS_DLL == 1 */

// Set CONTAINER_BASE_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (CONTAINER_BASE_NTRACE)
#  if (ACE_NTRACE == 1)
#    define CONTAINER_BASE_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define CONTAINER_BASE_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !CONTAINER_BASE_NTRACE */

#if (CONTAINER_BASE_NTRACE == 1)
#  define CONTAINER_BASE_TRACE(X)
#else /* (CONTAINER_BASE_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define CONTAINER_BASE_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (CONTAINER_BASE_NTRACE == 1) */

#endif /* CONTAINER_BASE_EXPORT_H */

// End of auto generated file.
