
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl Domain_Application
// ------------------------------
#ifndef DOMAIN_APPLICATION_EXPORT_H
#define DOMAIN_APPLICATION_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (DOMAIN_APPLICATION_HAS_DLL)
#  define DOMAIN_APPLICATION_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && DOMAIN_APPLICATION_HAS_DLL */

#if !defined (DOMAIN_APPLICATION_HAS_DLL)
#  define DOMAIN_APPLICATION_HAS_DLL 1
#endif /* ! DOMAIN_APPLICATION_HAS_DLL */

#if defined (DOMAIN_APPLICATION_HAS_DLL) && (DOMAIN_APPLICATION_HAS_DLL == 1)
#  if defined (DOMAIN_APPLICATION_BUILD_DLL)
#    define Domain_Application_Export ACE_Proper_Export_Flag
#    define DOMAIN_APPLICATION_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define DOMAIN_APPLICATION_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* DOMAIN_APPLICATION_BUILD_DLL */
#    define Domain_Application_Export ACE_Proper_Import_Flag
#    define DOMAIN_APPLICATION_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define DOMAIN_APPLICATION_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* DOMAIN_APPLICATION_BUILD_DLL */
#else /* DOMAIN_APPLICATION_HAS_DLL == 1 */
#  define Domain_Application_Export
#  define DOMAIN_APPLICATION_SINGLETON_DECLARATION(T)
#  define DOMAIN_APPLICATION_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* DOMAIN_APPLICATION_HAS_DLL == 1 */

// Set DOMAIN_APPLICATION_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (DOMAIN_APPLICATION_NTRACE)
#  if (ACE_NTRACE == 1)
#    define DOMAIN_APPLICATION_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define DOMAIN_APPLICATION_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !DOMAIN_APPLICATION_NTRACE */

#if (DOMAIN_APPLICATION_NTRACE == 1)
#  define DOMAIN_APPLICATION_TRACE(X)
#else /* (DOMAIN_APPLICATION_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define DOMAIN_APPLICATION_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (DOMAIN_APPLICATION_NTRACE == 1) */

#endif /* DOMAIN_APPLICATION_EXPORT_H */

// End of auto generated file.
