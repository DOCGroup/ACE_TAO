
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl HUD_SKEL
// ------------------------------
#ifndef HUD_SKEL_EXPORT_H
#define HUD_SKEL_EXPORT_H

#include "ace/config-all.h"

#if !defined (HUD_SKEL_HAS_DLL)
#  define HUD_SKEL_HAS_DLL 1
#endif /* ! HUD_SKEL_HAS_DLL */

#if defined (HUD_SKEL_HAS_DLL) && (HUD_SKEL_HAS_DLL == 1)
#  if defined (HUD_SKEL_BUILD_DLL)
#    define HUD_SKEL_Export ACE_Proper_Export_Flag
#    define HUD_SKEL_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define HUD_SKEL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* HUD_SKEL_BUILD_DLL */
#    define HUD_SKEL_Export ACE_Proper_Import_Flag
#    define HUD_SKEL_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define HUD_SKEL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* HUD_SKEL_BUILD_DLL */
#else /* HUD_SKEL_HAS_DLL == 1 */
#  define HUD_SKEL_Export
#  define HUD_SKEL_SINGLETON_DECLARATION(T)
#  define HUD_SKEL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* HUD_SKEL_HAS_DLL == 1 */

// Set HUD_SKEL_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (HUD_SKEL_NTRACE)
#  if (ACE_NTRACE == 1)
#    define HUD_SKEL_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define HUD_SKEL_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !HUD_SKEL_NTRACE */

#if (HUD_SKEL_NTRACE == 1)
#  define HUD_SKEL_TRACE(X)
#else /* (HUD_SKEL_NTRACE == 1) */
#  define HUD_SKEL_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (HUD_SKEL_NTRACE == 1) */

#endif /* HUD_SKEL_EXPORT_H */

// End of auto generated file.
