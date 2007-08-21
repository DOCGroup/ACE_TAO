
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl NodeManager_svnt
// ------------------------------
#ifndef NODEMANAGER_SVNT_EXPORT_H
#define NODEMANAGER_SVNT_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (NODEMANAGER_SVNT_HAS_DLL)
#  define NODEMANAGER_SVNT_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && NODEMANAGER_SVNT_HAS_DLL */

#if !defined (NODEMANAGER_SVNT_HAS_DLL)
#  define NODEMANAGER_SVNT_HAS_DLL 1
#endif /* ! NODEMANAGER_SVNT_HAS_DLL */

#if defined (NODEMANAGER_SVNT_HAS_DLL) && (NODEMANAGER_SVNT_HAS_DLL == 1)
#  if defined (NODEMANAGER_SVNT_BUILD_DLL)
#    define NodeManager_svnt_Export ACE_Proper_Export_Flag
#    define NODEMANAGER_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define NODEMANAGER_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* NODEMANAGER_SVNT_BUILD_DLL */
#    define NodeManager_svnt_Export ACE_Proper_Import_Flag
#    define NODEMANAGER_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define NODEMANAGER_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* NODEMANAGER_SVNT_BUILD_DLL */
#else /* NODEMANAGER_SVNT_HAS_DLL == 1 */
#  define NodeManager_svnt_Export
#  define NODEMANAGER_SVNT_SINGLETON_DECLARATION(T)
#  define NODEMANAGER_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* NODEMANAGER_SVNT_HAS_DLL == 1 */

// Set NODEMANAGER_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (NODEMANAGER_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define NODEMANAGER_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define NODEMANAGER_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !NODEMANAGER_SVNT_NTRACE */

#if (NODEMANAGER_SVNT_NTRACE == 1)
#  define NODEMANAGER_SVNT_TRACE(X)
#else /* (NODEMANAGER_SVNT_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define NODEMANAGER_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (NODEMANAGER_SVNT_NTRACE == 1) */

#endif /* NODEMANAGER_SVNT_EXPORT_H */

// End of auto generated file.
