
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl Basic_Configurator
// ------------------------------
#ifndef BASIC_CONFIGURATOR_EXPORT_H
#define BASIC_CONFIGURATOR_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (BASIC_CONFIGURATOR_HAS_DLL)
#  define BASIC_CONFIGURATOR_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && BASIC_CONFIGURATOR_HAS_DLL */

#if !defined (BASIC_CONFIGURATOR_HAS_DLL)
#  define BASIC_CONFIGURATOR_HAS_DLL 1
#endif /* ! BASIC_CONFIGURATOR_HAS_DLL */

#if defined (BASIC_CONFIGURATOR_HAS_DLL) && (BASIC_CONFIGURATOR_HAS_DLL == 1)
#  if defined (BASIC_CONFIGURATOR_BUILD_DLL)
#    define Basic_Configurator_Export ACE_Proper_Export_Flag
#    define BASIC_CONFIGURATOR_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define BASIC_CONFIGURATOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* BASIC_CONFIGURATOR_BUILD_DLL */
#    define Basic_Configurator_Export ACE_Proper_Import_Flag
#    define BASIC_CONFIGURATOR_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define BASIC_CONFIGURATOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* BASIC_CONFIGURATOR_BUILD_DLL */
#else /* BASIC_CONFIGURATOR_HAS_DLL == 1 */
#  define Basic_Configurator_Export
#  define BASIC_CONFIGURATOR_SINGLETON_DECLARATION(T)
#  define BASIC_CONFIGURATOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* BASIC_CONFIGURATOR_HAS_DLL == 1 */

// Set BASIC_CONFIGURATOR_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (BASIC_CONFIGURATOR_NTRACE)
#  if (ACE_NTRACE == 1)
#    define BASIC_CONFIGURATOR_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define BASIC_CONFIGURATOR_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !BASIC_CONFIGURATOR_NTRACE */

#if (BASIC_CONFIGURATOR_NTRACE == 1)
#  define BASIC_CONFIGURATOR_TRACE(X)
#else /* (BASIC_CONFIGURATOR_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define BASIC_CONFIGURATOR_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (BASIC_CONFIGURATOR_NTRACE == 1) */

#endif /* BASIC_CONFIGURATOR_EXPORT_H */

// End of auto generated file.
