// -*- C++ -*-
// $Id$

#ifndef CIF_COMMON_EXPORT_H
#define CIF_COMMON_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (CIF_COMMON_HAS_DLL)
#  define CIF_COMMON_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && CIF_COMMON_HAS_DLL */

#if !defined (CIF_COMMON_HAS_DLL)
#  define CIF_COMMON_HAS_DLL 1
#endif /* ! CIF_COMMON_HAS_DLL */

#if defined (CIF_COMMON_HAS_DLL) && (CIF_COMMON_HAS_DLL == 1)
#  if defined (CIF_COMMON_BUILD_DLL)
#    define CIF_COMMON_Export ACE_Proper_Export_Flag
#    define CIF_COMMON_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define CIF_COMMON_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* CIF_COMMON_BUILD_DLL */
#    define CIF_COMMON_Export ACE_Proper_Import_Flag
#    define CIF_COMMON_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define CIF_COMMON_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* CIF_COMMON_BUILD_DLL */
#else /* CIF_COMMON_HAS_DLL == 1 */
#  define CIF_COMMON_Export
#  define CIF_COMMON_SINGLETON_DECLARATION(T)
#  define CIF_COMMON_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* CIF_COMMON_HAS_DLL == 1 */

// Set CIF_COMMON_NTRACE = 0 to turn on library-specific
// tracing even if tracing is turned off for ACE.
#if !defined (CIF_COMMON_NTRACE)
#  if (ACE_NTRACE == 1)
#    define CIF_COMMON_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define CIF_COMMON_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !CIF_COMMON_NTRACE */

#if (CIF_COMMON_NTRACE == 1)
#  define CIF_COMMON_TRACE(X)
#else /* (CIF_COMMON_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define CIF_COMMON_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (CIF_COMMON_NTRACE == 1) */

#endif /* CIF_COMMON_EXPORT_H */

