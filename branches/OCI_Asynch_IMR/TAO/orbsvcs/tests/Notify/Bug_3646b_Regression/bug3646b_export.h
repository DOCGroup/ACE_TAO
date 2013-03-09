
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl bug3646b
// ------------------------------
#ifndef BUG3646b_EXPORT_H
#define BUG3646b_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (BUG3646b_HAS_DLL)
#  define BUG3646b_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && BUG3646b_HAS_DLL */

#if !defined (BUG3646b_HAS_DLL)
#  define BUG3646b_HAS_DLL 1
#endif /* ! BUG3646b_HAS_DLL */

#if defined (BUG3646b_HAS_DLL) && (BUG3646b_HAS_DLL == 1)
#  if defined (BUG3646b_BUILD_DLL)
#    define bug3646b_Export ACE_Proper_Export_Flag
#    define BUG3646b_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define BUG3646b_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* BUG3646b_BUILD_DLL */
#    define bug3646b_Export ACE_Proper_Import_Flag
#    define BUG3646b_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define BUG3646b_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* BUG3646b_BUILD_DLL */
#else /* BUG3646b_HAS_DLL == 1 */
#  define bug3646b_Export
#  define BUG3646b_SINGLETON_DECLARATION(T)
#  define BUG3646b_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* BUG3646b_HAS_DLL == 1 */

// Set BUG3646b_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (BUG3646b_NTRACE)
#  if (ACE_NTRACE == 1)
#    define BUG3646b_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define BUG3646b_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !BUG3646b_NTRACE */

#if (BUG3646b_NTRACE == 1)
#  define BUG3646b_TRACE(X)
#else /* (BUG3646b_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define BUG3646b_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (BUG3646b_NTRACE == 1) */

#endif /* BUG3646b_EXPORT_H */

// End of auto generated file.
