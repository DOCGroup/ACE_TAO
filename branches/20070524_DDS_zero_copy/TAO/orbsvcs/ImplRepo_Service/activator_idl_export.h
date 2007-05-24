
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s Activator
// ------------------------------
#ifndef ACTIVATOR_IDL_EXPORT_H
#define ACTIVATOR_IDL_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (ACTIVATOR_IDL_HAS_DLL)
#  define ACTIVATOR_IDL_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && ACTIVATOR_IDL_HAS_DLL */

#if !defined (ACTIVATOR_IDL_HAS_DLL)
#  define ACTIVATOR_IDL_HAS_DLL 1
#endif /* ! ACTIVATOR_IDL_HAS_DLL */

#if defined (ACTIVATOR_IDL_HAS_DLL) && (ACTIVATOR_IDL_HAS_DLL == 1)
#  if defined (ACTIVATOR_IDL_BUILD_DLL)
#    define ActivatorIDL_Export ACE_Proper_Export_Flag
#    define ACTIVATOR_IDL_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define ACTIVATOR_IDL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* ACTIVATOR_IDL_BUILD_DLL */
#    define ActivatorIDL_Export ACE_Proper_Import_Flag
#    define ACTIVATOR_IDL_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define ACTIVATOR_IDL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* ACTIVATOR_IDL_BUILD_DLL */
#else /* ACTIVATOR_IDL_HAS_DLL == 1 */
#  define ActivatorIDL_Export
#  define ACTIVATOR_IDL_SINGLETON_DECLARATION(T)
#  define ACTIVATOR_IDL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* ACTIVATOR_IDL_HAS_DLL == 1 */

// Set ACTIVATOR_IDL_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (ACTIVATOR_IDL_NTRACE)
#  if (ACE_NTRACE == 1)
#    define ACTIVATOR_IDL_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define ACTIVATOR_IDL_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !ACTIVATOR_IDL_NTRACE */

#if (ACTIVATOR_IDL_NTRACE == 1)
#  define ACTIVATOR_IDL_TRACE(X)
#else /* (ACTIVATOR_IDL_NTRACE == 1) */
#  define ACTIVATOR_IDL_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (ACTIVATOR_IDL_NTRACE == 1) */

#endif /* ACTIVATOR_IDL_EXPORT_H */

// End of auto generated file.
