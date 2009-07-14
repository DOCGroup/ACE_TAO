
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl SENDER_EXEC_2
// ------------------------------
#ifndef SENDER_EXEC_2_EXPORT_H
#define SENDER_EXEC_2_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (SENDER_EXEC_2_HAS_DLL)
#  define SENDER_EXEC_2_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && SENDER_EXEC_2_HAS_DLL */

#if !defined (SENDER_EXEC_2_HAS_DLL)
#  define SENDER_EXEC_2_HAS_DLL 1
#endif /* ! SENDER_EXEC_2_HAS_DLL */

#if defined (SENDER_EXEC_2_HAS_DLL) && (SENDER_EXEC_2_HAS_DLL == 1)
#  if defined (SENDER_EXEC_2_BUILD_DLL)
#    define SENDER_EXEC_2_Export ACE_Proper_Export_Flag
#    define SENDER_EXEC_2_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define SENDER_EXEC_2_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* SENDER_EXEC_2_BUILD_DLL */
#    define SENDER_EXEC_2_Export ACE_Proper_Import_Flag
#    define SENDER_EXEC_2_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define SENDER_EXEC_2_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* SENDER_EXEC_2_BUILD_DLL */
#else /* SENDER_EXEC_2_HAS_DLL == 1 */
#  define SENDER_EXEC_2_Export
#  define SENDER_EXEC_2_SINGLETON_DECLARATION(T)
#  define SENDER_EXEC_2_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* SENDER_EXEC_2_HAS_DLL == 1 */

// Set SENDER_EXEC_2_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (SENDER_EXEC_2_NTRACE)
#  if (ACE_NTRACE == 1)
#    define SENDER_EXEC_2_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define SENDER_EXEC_2_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !SENDER_EXEC_2_NTRACE */

#if (SENDER_EXEC_2_NTRACE == 1)
#  define SENDER_EXEC_2_TRACE(X)
#else /* (SENDER_EXEC_2_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define SENDER_EXEC_2_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (SENDER_EXEC_2_NTRACE == 1) */

#endif /* SENDER_EXEC_2_EXPORT_H */

// End of auto generated file.
