
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s Activator
// ------------------------------
#ifndef ACTIVATOR_EXPORT_H
#define ACTIVATOR_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (ACTIVATOR_HAS_DLL)
#  define ACTIVATOR_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && ACTIVATOR_HAS_DLL */

#if !defined (ACTIVATOR_HAS_DLL)
#  define ACTIVATOR_HAS_DLL 1
#endif /* ! ACTIVATOR_HAS_DLL */

#if defined (ACTIVATOR_HAS_DLL) && (ACTIVATOR_HAS_DLL == 1)
#  if defined (ACTIVATOR_BUILD_DLL)
#    define Activator_Export ACE_Proper_Export_Flag
#    define ACTIVATOR_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define ACTIVATOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* ACTIVATOR_BUILD_DLL */
#    define Activator_Export ACE_Proper_Import_Flag
#    define ACTIVATOR_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define ACTIVATOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* ACTIVATOR_BUILD_DLL */
#else /* ACTIVATOR_HAS_DLL == 1 */
#  define Activator_Export
#  define ACTIVATOR_SINGLETON_DECLARATION(T)
#  define ACTIVATOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* ACTIVATOR_HAS_DLL == 1 */

// Set ACTIVATOR_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (ACTIVATOR_NTRACE)
#  if (ACE_NTRACE == 1)
#    define ACTIVATOR_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define ACTIVATOR_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !ACTIVATOR_NTRACE */

#if (ACTIVATOR_NTRACE == 1)
#  define ACTIVATOR_TRACE(X)
#else /* (ACTIVATOR_NTRACE == 1) */
#  define ACTIVATOR_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (ACTIVATOR_NTRACE == 1) */

#endif /* ACTIVATOR_EXPORT_H */

// End of auto generated file.
