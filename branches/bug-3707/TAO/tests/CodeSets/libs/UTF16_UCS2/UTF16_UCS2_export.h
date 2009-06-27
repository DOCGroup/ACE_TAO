
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s UTF16_UCS2
// ------------------------------
#ifndef UTF16_UCS2_EXPORT_H
#define UTF16_UCS2_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (UTF16_UCS2_HAS_DLL)
#  define UTF16_UCS2_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && UTF16_UCS2_HAS_DLL */

#if !defined (UTF16_UCS2_HAS_DLL)
#  define UTF16_UCS2_HAS_DLL 1
#endif /* ! UTF16_UCS2_HAS_DLL */

#if defined (UTF16_UCS2_HAS_DLL) && (UTF16_UCS2_HAS_DLL == 1)
#  if defined (UTF16_UCS2_BUILD_DLL)
#    define UTF16_UCS2_Export ACE_Proper_Export_Flag
#    define UTF16_UCS2_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define UTF16_UCS2_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* UTF16_UCS2_BUILD_DLL */
#    define UTF16_UCS2_Export ACE_Proper_Import_Flag
#    define UTF16_UCS2_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define UTF16_UCS2_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* UTF16_UCS2_BUILD_DLL */
#else /* UTF16_UCS2_HAS_DLL == 1 */
#  define UTF16_UCS2_Export
#  define UTF16_UCS2_SINGLETON_DECLARATION(T)
#  define UTF16_UCS2_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* UTF16_UCS2_HAS_DLL == 1 */

// Set UTF16_UCS2_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (UTF16_UCS2_NTRACE)
#  if (ACE_NTRACE == 1)
#    define UTF16_UCS2_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define UTF16_UCS2_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !UTF16_UCS2_NTRACE */

#if (UTF16_UCS2_NTRACE == 1)
#  define UTF16_UCS2_TRACE(X)
#else /* (UTF16_UCS2_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define UTF16_UCS2_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (UTF16_UCS2_NTRACE == 1) */

#endif /* UTF16_UCS2_EXPORT_H */

// End of auto generated file.
