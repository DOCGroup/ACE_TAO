
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl RATEGEN_SKEL
// ------------------------------
#ifndef RATEGEN_SKEL_EXPORT_H
#define RATEGEN_SKEL_EXPORT_H

#include "ace/config-all.h"

#if !defined (RATEGEN_SKEL_HAS_DLL)
#  define RATEGEN_SKEL_HAS_DLL 1
#endif /* ! RATEGEN_SKEL_HAS_DLL */

#if defined (RATEGEN_SKEL_HAS_DLL) && (RATEGEN_SKEL_HAS_DLL == 1)
#  if defined (RATEGEN_SKEL_BUILD_DLL)
#    define RATEGEN_SKEL_Export ACE_Proper_Export_Flag
#    define RATEGEN_SKEL_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define RATEGEN_SKEL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* RATEGEN_SKEL_BUILD_DLL */
#    define RATEGEN_SKEL_Export ACE_Proper_Import_Flag
#    define RATEGEN_SKEL_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define RATEGEN_SKEL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* RATEGEN_SKEL_BUILD_DLL */
#else /* RATEGEN_SKEL_HAS_DLL == 1 */
#  define RATEGEN_SKEL_Export
#  define RATEGEN_SKEL_SINGLETON_DECLARATION(T)
#  define RATEGEN_SKEL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* RATEGEN_SKEL_HAS_DLL == 1 */

// Set RATEGEN_SKEL_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (RATEGEN_SKEL_NTRACE)
#  if (ACE_NTRACE == 1)
#    define RATEGEN_SKEL_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define RATEGEN_SKEL_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !RATEGEN_SKEL_NTRACE */

#if (RATEGEN_SKEL_NTRACE == 1)
#  define RATEGEN_SKEL_TRACE(X)
#else /* (RATEGEN_SKEL_NTRACE == 1) */
#  define RATEGEN_SKEL_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (RATEGEN_SKEL_NTRACE == 1) */

#endif /* RATEGEN_SKEL_EXPORT_H */

// End of auto generated file.
