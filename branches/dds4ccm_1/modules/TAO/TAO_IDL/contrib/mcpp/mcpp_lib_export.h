
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl MCPP_LIB
// ------------------------------
#ifndef MCPP_LIB_EXPORT_H
#define MCPP_LIB_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (MCPP_LIB_HAS_DLL)
#  define MCPP_LIB_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && MCPP_LIB_HAS_DLL */

#if !defined (MCPP_LIB_HAS_DLL)
#  define MCPP_LIB_HAS_DLL 1
#endif /* ! MCPP_LIB_HAS_DLL */

#if defined (MCPP_LIB_HAS_DLL) && (MCPP_LIB_HAS_DLL == 1)
#  if defined (MCPP_LIB_BUILD_DLL)
#    define MCPP_LIB_Export ACE_Proper_Export_Flag
#    define MCPP_LIB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define MCPP_LIB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* MCPP_LIB_BUILD_DLL */
#    define MCPP_LIB_Export ACE_Proper_Import_Flag
#    define MCPP_LIB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define MCPP_LIB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* MCPP_LIB_BUILD_DLL */
#else /* MCPP_LIB_HAS_DLL == 1 */
#  define MCPP_LIB_Export
#  define MCPP_LIB_SINGLETON_DECLARATION(T)
#  define MCPP_LIB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* MCPP_LIB_HAS_DLL == 1 */

// Set MCPP_LIB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (MCPP_LIB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define MCPP_LIB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define MCPP_LIB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !MCPP_LIB_NTRACE */

#if (MCPP_LIB_NTRACE == 1)
#  define MCPP_LIB_TRACE(X)
#else /* (MCPP_LIB_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define MCPP_LIB_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (MCPP_LIB_NTRACE == 1) */

#endif /* MCPP_LIB_EXPORT_H */

// End of auto generated file.
