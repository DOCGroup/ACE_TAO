
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl GPS_STUB
// ------------------------------
#ifndef GPS_STUB_EXPORT_H
#define GPS_STUB_EXPORT_H

#include "ace/config-all.h"

#if !defined (GPS_STUB_HAS_DLL)
#  define GPS_STUB_HAS_DLL 1
#endif /* ! GPS_STUB_HAS_DLL */

#if defined (GPS_STUB_HAS_DLL) && (GPS_STUB_HAS_DLL == 1)
#  if defined (GPS_STUB_BUILD_DLL)
#    define GPS_STUB_Export ACE_Proper_Export_Flag
#    define GPS_STUB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define GPS_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* GPS_STUB_BUILD_DLL */
#    define GPS_STUB_Export ACE_Proper_Import_Flag
#    define GPS_STUB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define GPS_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* GPS_STUB_BUILD_DLL */
#else /* GPS_STUB_HAS_DLL == 1 */
#  define GPS_STUB_Export
#  define GPS_STUB_SINGLETON_DECLARATION(T)
#  define GPS_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* GPS_STUB_HAS_DLL == 1 */

// Set GPS_STUB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (GPS_STUB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define GPS_STUB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define GPS_STUB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !GPS_STUB_NTRACE */

#if (GPS_STUB_NTRACE == 1)
#  define GPS_STUB_TRACE(X)
#else /* (GPS_STUB_NTRACE == 1) */
#  define GPS_STUB_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (GPS_STUB_NTRACE == 1) */

#endif /* GPS_STUB_EXPORT_H */

// End of auto generated file.
