
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl DT_Creator
// ------------------------------
#ifndef DT_CREATOR_EXPORT_H
#define DT_CREATOR_EXPORT_H

#include "ace/config-all.h"

#if !defined (DT_CREATOR_HAS_DLL)
#  define DT_CREATOR_HAS_DLL 1
#endif /* ! DT_CREATOR_HAS_DLL */

#if defined (DT_CREATOR_HAS_DLL) && (DT_CREATOR_HAS_DLL == 1)
#  if defined (DT_CREATOR_BUILD_DLL)
#    define DT_Creator_Export ACE_Proper_Export_Flag
#    define DT_CREATOR_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define DT_CREATOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* DT_CREATOR_BUILD_DLL */
#    define DT_Creator_Export ACE_Proper_Import_Flag
#    define DT_CREATOR_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define DT_CREATOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* DT_CREATOR_BUILD_DLL */
#else /* DT_CREATOR_HAS_DLL == 1 */
#  define DT_Creator_Export
#  define DT_CREATOR_SINGLETON_DECLARATION(T)
#  define DT_CREATOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* DT_CREATOR_HAS_DLL == 1 */

// Set DT_CREATOR_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (DT_CREATOR_NTRACE)
#  if (ACE_NTRACE == 1)
#    define DT_CREATOR_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define DT_CREATOR_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !DT_CREATOR_NTRACE */

#if (DT_CREATOR_NTRACE == 1)
#  define DT_CREATOR_TRACE(X)
#else /* (DT_CREATOR_NTRACE == 1) */
#  define DT_CREATOR_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (DT_CREATOR_NTRACE == 1) */

#endif /* DT_CREATOR_EXPORT_H */

// End of auto generated file.
