
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl BATTLE_STUB
// ------------------------------
#ifndef BATTLE_STUB_EXPORT_H
#define BATTLE_STUB_EXPORT_H

#include "ace/config-all.h"

#if !defined (BATTLE_STUB_HAS_DLL)
#  define BATTLE_STUB_HAS_DLL 1
#endif /* ! BATTLE_STUB_HAS_DLL */

#if defined (BATTLE_STUB_HAS_DLL) && (BATTLE_STUB_HAS_DLL == 1)
#  if defined (BATTLE_STUB_BUILD_DLL)
#    define BATTLE_STUB_Export ACE_Proper_Export_Flag
#    define BATTLE_STUB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define BATTLE_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* BATTLE_STUB_BUILD_DLL */
#    define BATTLE_STUB_Export ACE_Proper_Import_Flag
#    define BATTLE_STUB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define BATTLE_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* BATTLE_STUB_BUILD_DLL */
#else /* BATTLE_STUB_HAS_DLL == 1 */
#  define BATTLE_STUB_Export
#  define BATTLE_STUB_SINGLETON_DECLARATION(T)
#  define BATTLE_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* BATTLE_STUB_HAS_DLL == 1 */

// Set BATTLE_STUB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (BATTLE_STUB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define BATTLE_STUB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define BATTLE_STUB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !BATTLE_STUB_NTRACE */

#if (BATTLE_STUB_NTRACE == 1)
#  define BATTLE_STUB_TRACE(X)
#else /* (BATTLE_STUB_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define BATTLE_STUB_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (BATTLE_STUB_NTRACE == 1) */

#endif /* BATTLE_STUB_EXPORT_H */

// End of auto generated file.
