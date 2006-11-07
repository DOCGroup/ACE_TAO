
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl SA_BUILDER
// ------------------------------
#ifndef SA_BUILDER_EXPORT_H
#define SA_BUILDER_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (SA_BUILDER_HAS_DLL)
#  define SA_BUILDER_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && SA_BUILDER_HAS_DLL */

#if !defined (SA_BUILDER_HAS_DLL)
#  define SA_BUILDER_HAS_DLL 1
#endif /* ! SA_BUILDER_HAS_DLL */

#if defined (SA_BUILDER_HAS_DLL) && (SA_BUILDER_HAS_DLL == 1)
#  if defined (SA_BUILDER_BUILD_DLL)
#    define SA_BUILDER_Export ACE_Proper_Export_Flag
#    define SA_BUILDER_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define SA_BUILDER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* SA_BUILDER_BUILD_DLL */
#    define SA_BUILDER_Export ACE_Proper_Import_Flag
#    define SA_BUILDER_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define SA_BUILDER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* SA_BUILDER_BUILD_DLL */
#else /* SA_BUILDER_HAS_DLL == 1 */
#  define SA_BUILDER_Export
#  define SA_BUILDER_SINGLETON_DECLARATION(T)
#  define SA_BUILDER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* SA_BUILDER_HAS_DLL == 1 */

// Set SA_BUILDER_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (SA_BUILDER_NTRACE)
#  if (ACE_NTRACE == 1)
#    define SA_BUILDER_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define SA_BUILDER_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !SA_BUILDER_NTRACE */

#if (SA_BUILDER_NTRACE == 1)
#  define SA_BUILDER_TRACE(X)
#else /* (SA_BUILDER_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define SA_BUILDER_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (SA_BUILDER_NTRACE == 1) */

#endif /* SA_BUILDER_EXPORT_H */

// End of auto generated file.
