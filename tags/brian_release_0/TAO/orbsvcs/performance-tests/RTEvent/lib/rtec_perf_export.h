
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef TAO_RTEC_PERF_EXPORT_H
#define TAO_RTEC_PERF_EXPORT_H

#include "ace/config-all.h"

#if !defined (TAO_RTEC_PERF_HAS_DLL)
#  define TAO_RTEC_PERF_HAS_DLL 1
#endif /* ! TAO_RTEC_PERF_HAS_DLL */

#if defined (TAO_RTEC_PERF_HAS_DLL) && (TAO_RTEC_PERF_HAS_DLL == 1)
#  if defined (TAO_RTEC_PERF_BUILD_DLL)
#    define TAO_RTEC_Perf_Export ACE_Proper_Export_Flag
#    define TAO_RTEC_PERF_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TAO_RTEC_PERF_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TAO_RTEC_PERF_BUILD_DLL */
#    define TAO_RTEC_Perf_Export ACE_Proper_Import_Flag
#    define TAO_RTEC_PERF_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TAO_RTEC_PERF_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TAO_RTEC_PERF_BUILD_DLL */
#else /* TAO_RTEC_PERF_HAS_DLL == 1 */
#  define TAO_RTEC_Perf_Export
#  define TAO_RTEC_PERF_SINGLETON_DECLARATION(T)
#  define TAO_RTEC_PERF_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TAO_RTEC_PERF_HAS_DLL == 1 */

#endif /* TAO_RTEC_PERF_EXPORT_H */

// End of auto generated file.
