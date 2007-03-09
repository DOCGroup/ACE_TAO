
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl SANETFILEIN
// ------------------------------
#ifndef SANETFILEIN_EXPORT_H
#define SANETFILEIN_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (SANETFILEIN_HAS_DLL)
#  define SANETFILEIN_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && SANETFILEIN_HAS_DLL */

#if !defined (SANETFILEIN_HAS_DLL)
#  define SANETFILEIN_HAS_DLL 1
#endif /* ! SANETFILEIN_HAS_DLL */

#if defined (SANETFILEIN_HAS_DLL) && (SANETFILEIN_HAS_DLL == 1)
#  if defined (SANETFILEIN_BUILD_DLL)
#    define SANETFILEIN_Export ACE_Proper_Export_Flag
#    define SANETFILEIN_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define SANETFILEIN_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* SANETFILEIN_BUILD_DLL */
#    define SANETFILEIN_Export ACE_Proper_Import_Flag
#    define SANETFILEIN_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define SANETFILEIN_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* SANETFILEIN_BUILD_DLL */
#else /* SANETFILEIN_HAS_DLL == 1 */
#  define SANETFILEIN_Export
#  define SANETFILEIN_SINGLETON_DECLARATION(T)
#  define SANETFILEIN_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* SANETFILEIN_HAS_DLL == 1 */

// Set SANETFILEIN_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (SANETFILEIN_NTRACE)
#  if (ACE_NTRACE == 1)
#    define SANETFILEIN_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define SANETFILEIN_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !SANETFILEIN_NTRACE */

#if (SANETFILEIN_NTRACE == 1)
#  define SANETFILEIN_TRACE(X)
#else /* (SANETFILEIN_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define SANETFILEIN_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (SANETFILEIN_NTRACE == 1) */

#endif /* SANETFILEIN_EXPORT_H */

// End of auto generated file.
