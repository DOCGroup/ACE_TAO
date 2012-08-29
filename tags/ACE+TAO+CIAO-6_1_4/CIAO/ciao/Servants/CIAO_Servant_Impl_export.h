
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// ------------------------------
#ifndef CIAO_SERVANT_IMPL_EXPORT_H
#define CIAO_SERVANT_IMPL_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (CIAO_SERVANT_IMPL_HAS_DLL)
#  define CIAO_SERVANT_IMPL_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && CIAO_SERVANT_IMPL_HAS_DLL */

#if !defined (CIAO_SERVANT_IMPL_HAS_DLL)
#  define CIAO_SERVANT_IMPL_HAS_DLL 1
#endif /* ! CIAO_SERVANT_IMPL_HAS_DLL */

#if defined (CIAO_SERVANT_IMPL_HAS_DLL) && (CIAO_SERVANT_IMPL_HAS_DLL == 1)
#  if defined (CIAO_SERVANT_IMPL_BUILD_DLL)
#    define CIAO_Servant_Impl_Export ACE_Proper_Export_Flag
#    define CIAO_SERVANT_IMPL_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define CIAO_SERVANT_IMPL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* CIAO_SERVANT_IMPL_BUILD_DLL */
#    define CIAO_Servant_Impl_Export ACE_Proper_Import_Flag
#    define CIAO_SERVANT_IMPL_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define CIAO_SERVANT_IMPL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* CIAO_SERVANT_IMPL_BUILD_DLL */
#else /* CIAO_SERVANT_IMPL_HAS_DLL == 1 */
#  define CIAO_Servant_Impl_Export
#  define CIAO_SERVANT_IMPL_SINGLETON_DECLARATION(T)
#  define CIAO_SERVANT_IMPL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* CIAO_SERVANT_IMPL_HAS_DLL == 1 */

// Set CIAO_SERVANT_IMPL_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (CIAO_SERVANT_IMPL_NTRACE)
#  if (ACE_NTRACE == 1)
#    define CIAO_SERVANT_IMPL_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define CIAO_SERVANT_IMPL_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !CIAO_SERVANT_IMPL_NTRACE */

#if (CIAO_SERVANT_IMPL_NTRACE == 1)
#  define CIAO_SERVANT_IMPL_TRACE(X)
#else /* (CIAO_SERVANT_IMPL_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define CIAO_SERVANT_IMPL_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (CIAO_SERVANT_IMPL_NTRACE == 1) */

#endif /* CIAO_SERVANT_IMPL_EXPORT_H */

// End of auto generated file.
