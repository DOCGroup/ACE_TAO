
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl CCM_Svnt
// ------------------------------
#ifndef CCM_SVNT_EXPORT_H
#define CCM_SVNT_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (CCM_SVNT_HAS_DLL)
#  define CCM_SVNT_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && CCM_SVNT_HAS_DLL */

#if !defined (CCM_SVNT_HAS_DLL)
#  define CCM_SVNT_HAS_DLL 1
#endif /* ! CCM_SVNT_HAS_DLL */

#if defined (CCM_SVNT_HAS_DLL) && (CCM_SVNT_HAS_DLL == 1)
#  if defined (CCM_SVNT_BUILD_DLL)
#    define CCM_Svnt_Export ACE_Proper_Export_Flag
#    define CCM_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define CCM_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* CCM_SVNT_BUILD_DLL */
#    define CCM_Svnt_Export ACE_Proper_Import_Flag
#    define CCM_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define CCM_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* CCM_SVNT_BUILD_DLL */
#else /* CCM_SVNT_HAS_DLL == 1 */
#  define CCM_Svnt_Export
#  define CCM_SVNT_SINGLETON_DECLARATION(T)
#  define CCM_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* CCM_SVNT_HAS_DLL == 1 */

// Set CCM_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (CCM_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define CCM_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define CCM_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !CCM_SVNT_NTRACE */

#if (CCM_SVNT_NTRACE == 1)
#  define CCM_SVNT_TRACE(X)
#else /* (CCM_SVNT_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define CCM_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (CCM_SVNT_NTRACE == 1) */

#endif /* CCM_SVNT_EXPORT_H */

// End of auto generated file.
