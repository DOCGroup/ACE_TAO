
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl HUD_STUB
// ------------------------------
#ifndef HUD_STUB_EXPORT_H
#define HUD_STUB_EXPORT_H

#include "ace/config-all.h"

#if !defined (HUD_STUB_HAS_DLL)
#  define HUD_STUB_HAS_DLL 1
#endif /* ! HUD_STUB_HAS_DLL */

#if defined (HUD_STUB_HAS_DLL) && (HUD_STUB_HAS_DLL == 1)
#  if defined (HUD_STUB_BUILD_DLL)
#    define HUD_STUB_Export ACE_Proper_Export_Flag
#    define HUD_STUB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define HUD_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* HUD_STUB_BUILD_DLL */
#    define HUD_STUB_Export ACE_Proper_Import_Flag
#    define HUD_STUB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define HUD_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* HUD_STUB_BUILD_DLL */
#else /* HUD_STUB_HAS_DLL == 1 */
#  define HUD_STUB_Export
#  define HUD_STUB_SINGLETON_DECLARATION(T)
#  define HUD_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* HUD_STUB_HAS_DLL == 1 */

// Set HUD_STUB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (HUD_STUB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define HUD_STUB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define HUD_STUB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !HUD_STUB_NTRACE */

#if (HUD_STUB_NTRACE == 1)
#  define HUD_STUB_TRACE(X)
#else /* (HUD_STUB_NTRACE == 1) */
#  define HUD_STUB_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (HUD_STUB_NTRACE == 1) */

#endif /* HUD_STUB_EXPORT_H */

// End of auto generated file.
