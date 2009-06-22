
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl Swapping_Container
// ------------------------------
#ifndef SWAPPING_CONTAINER_EXPORT_H
#define SWAPPING_CONTAINER_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (SWAPPING_CONTAINER_HAS_DLL)
#  define SWAPPING_CONTAINER_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && SWAPPING_CONTAINER_HAS_DLL */

#if !defined (SWAPPING_CONTAINER_HAS_DLL)
#  define SWAPPING_CONTAINER_HAS_DLL 1
#endif /* ! SWAPPING_CONTAINER_HAS_DLL */

#if defined (SWAPPING_CONTAINER_HAS_DLL) && (SWAPPING_CONTAINER_HAS_DLL == 1)
#  if defined (SWAPPING_CONTAINER_BUILD_DLL)
#    define CIAO_Swapping_Container_Export ACE_Proper_Export_Flag
#    define SWAPPING_CONTAINER_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define SWAPPING_CONTAINER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* SWAPPING_CONTAINER_BUILD_DLL */
#    define CIAO_Swapping_Container_Export ACE_Proper_Import_Flag
#    define SWAPPING_CONTAINER_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define SWAPPING_CONTAINER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* SWAPPING_CONTAINER_BUILD_DLL */
#else /* SWAPPING_CONTAINER_HAS_DLL == 1 */
#  define CIAO_Swapping_Container_Export
#  define SWAPPING_CONTAINER_SINGLETON_DECLARATION(T)
#  define SWAPPING_CONTAINER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* SWAPPING_CONTAINER_HAS_DLL == 1 */

// Set SWAPPING_CONTAINER_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (SWAPPING_CONTAINER_NTRACE)
#  if (ACE_NTRACE == 1)
#    define SWAPPING_CONTAINER_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define SWAPPING_CONTAINER_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !SWAPPING_CONTAINER_NTRACE */

#if (SWAPPING_CONTAINER_NTRACE == 1)
#  define SWAPPING_CONTAINER_TRACE(X)
#else /* (SWAPPING_CONTAINER_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define SWAPPING_CONTAINER_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (SWAPPING_CONTAINER_NTRACE == 1) */

#endif /* SWAPPING_CONTAINER_EXPORT_H */

// End of auto generated file.
