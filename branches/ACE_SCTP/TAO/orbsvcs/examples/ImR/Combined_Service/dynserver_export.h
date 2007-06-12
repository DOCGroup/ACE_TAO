
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s DynServer
// ------------------------------
#ifndef DYNSERVER_EXPORT_H
#define DYNSERVER_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (DYNSERVER_HAS_DLL)
#  define DYNSERVER_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && DYNSERVER_HAS_DLL */

#if !defined (DYNSERVER_HAS_DLL)
#  define DYNSERVER_HAS_DLL 1
#endif /* ! DYNSERVER_HAS_DLL */

#if defined (DYNSERVER_HAS_DLL) && (DYNSERVER_HAS_DLL == 1)
#  if defined (DYNSERVER_BUILD_DLL)
#    define DynServer_Export ACE_Proper_Export_Flag
#    define DYNSERVER_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define DYNSERVER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* DYNSERVER_BUILD_DLL */
#    define DynServer_Export ACE_Proper_Import_Flag
#    define DYNSERVER_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define DYNSERVER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* DYNSERVER_BUILD_DLL */
#else /* DYNSERVER_HAS_DLL == 1 */
#  define DynServer_Export
#  define DYNSERVER_SINGLETON_DECLARATION(T)
#  define DYNSERVER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* DYNSERVER_HAS_DLL == 1 */

// Set DYNSERVER_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (DYNSERVER_NTRACE)
#  if (ACE_NTRACE == 1)
#    define DYNSERVER_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define DYNSERVER_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !DYNSERVER_NTRACE */

#if (DYNSERVER_NTRACE == 1)
#  define DYNSERVER_TRACE(X)
#else /* (DYNSERVER_NTRACE == 1) */
#  define DYNSERVER_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (DYNSERVER_NTRACE == 1) */

#endif /* DYNSERVER_EXPORT_H */

// End of auto generated file.
