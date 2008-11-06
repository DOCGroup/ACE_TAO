
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl SSA_Export
// ------------------------------
#ifndef SSA_EXPORT_EXPORT_H
#define SSA_EXPORT_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (SSA_EXPORT_HAS_DLL)
#  define SSA_EXPORT_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && SSA_EXPORT_HAS_DLL */

#if !defined (SSA_EXPORT_HAS_DLL)
#  define SSA_EXPORT_HAS_DLL 1
#endif /* ! SSA_EXPORT_HAS_DLL */

#if defined (SSA_EXPORT_HAS_DLL) && (SSA_EXPORT_HAS_DLL == 1)
#  if defined (SSA_EXPORT_BUILD_DLL)
#    define SSA_Export_Export ACE_Proper_Export_Flag
#    define SSA_EXPORT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define SSA_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* SSA_EXPORT_BUILD_DLL */
#    define SSA_Export_Export ACE_Proper_Import_Flag
#    define SSA_EXPORT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define SSA_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* SSA_EXPORT_BUILD_DLL */
#else /* SSA_EXPORT_HAS_DLL == 1 */
#  define SSA_Export_Export
#  define SSA_EXPORT_SINGLETON_DECLARATION(T)
#  define SSA_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* SSA_EXPORT_HAS_DLL == 1 */

// Set SSA_EXPORT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (SSA_EXPORT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define SSA_EXPORT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define SSA_EXPORT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !SSA_EXPORT_NTRACE */

#if (SSA_EXPORT_NTRACE == 1)
#  define SSA_EXPORT_TRACE(X)
#else /* (SSA_EXPORT_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define SSA_EXPORT_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (SSA_EXPORT_NTRACE == 1) */

#endif /* SSA_EXPORT_EXPORT_H */

// End of auto generated file.

// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl SSA
// ------------------------------
#ifndef SSA_EXPORT_H
#define SSA_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (SSA_HAS_DLL)
#  define SSA_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && SSA_HAS_DLL */

#if !defined (SSA_HAS_DLL)
#  define SSA_HAS_DLL 1
#endif /* ! SSA_HAS_DLL */

#if defined (SSA_HAS_DLL) && (SSA_HAS_DLL == 1)
#  if defined (SSA_BUILD_DLL)
#    define SSA_Export ACE_Proper_Export_Flag
#    define SSA_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define SSA_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* SSA_BUILD_DLL */
#    define SSA_Export ACE_Proper_Import_Flag
#    define SSA_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define SSA_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* SSA_BUILD_DLL */
#else /* SSA_HAS_DLL == 1 */
#  define SSA_Export
#  define SSA_SINGLETON_DECLARATION(T)
#  define SSA_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* SSA_HAS_DLL == 1 */

// Set SSA_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (SSA_NTRACE)
#  if (ACE_NTRACE == 1)
#    define SSA_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define SSA_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !SSA_NTRACE */

#if (SSA_NTRACE == 1)
#  define SSA_TRACE(X)
#else /* (SSA_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define SSA_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (SSA_NTRACE == 1) */

#endif /* SSA_EXPORT_H */

// End of auto generated file.
