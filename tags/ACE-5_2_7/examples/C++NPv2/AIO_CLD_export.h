
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl AIO_CLD
// ------------------------------
#ifndef AIO_CLD_EXPORT_H
#define AIO_CLD_EXPORT_H

#include "ace/config-all.h"

#if !defined (AIO_CLD_HAS_DLL)
#  define AIO_CLD_HAS_DLL 1
#endif /* ! AIO_CLD_HAS_DLL */

#if defined (AIO_CLD_HAS_DLL) && (AIO_CLD_HAS_DLL == 1)
#  if defined (AIO_CLD_BUILD_DLL)
#    define AIO_CLD_Export ACE_Proper_Export_Flag
#    define AIO_CLD_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define AIO_CLD_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* AIO_CLD_BUILD_DLL */
#    define AIO_CLD_Export ACE_Proper_Import_Flag
#    define AIO_CLD_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define AIO_CLD_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* AIO_CLD_BUILD_DLL */
#else /* AIO_CLD_HAS_DLL == 1 */
#  define AIO_CLD_Export
#  define AIO_CLD_SINGLETON_DECLARATION(T)
#  define AIO_CLD_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* AIO_CLD_HAS_DLL == 1 */

// Set AIO_CLD_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (AIO_CLD_NTRACE)
#  if (ACE_NTRACE == 1)
#    define AIO_CLD_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define AIO_CLD_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !AIO_CLD_NTRACE */

#if (AIO_CLD_NTRACE == 1)
#  define AIO_CLD_TRACE(X)
#else /* (AIO_CLD_NTRACE == 1) */
#  define AIO_CLD_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (AIO_CLD_NTRACE == 1) */

#endif /* AIO_CLD_EXPORT_H */

// End of auto generated file.
