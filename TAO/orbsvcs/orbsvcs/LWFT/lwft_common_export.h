
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef LWFT_COMMON_EXPORT_H
#define LWFT_COMMON_EXPORT_H

#include "ace/config-all.h"

#if defined (TAO_AS_STATIC_LIBS)
#  if !defined (LWFT_COMMON_HAS_DLL)
#    define LWFT_COMMON_HAS_DLL 0
#  endif /* ! LWFT_COMMON_HAS_DLL */
#else
#  if !defined (LWFT_COMMON_HAS_DLL)
#    define LWFT_COMMON_HAS_DLL 1
#  endif /* ! LWFT_COMMON_HAS_DLL */
#endif

#if defined (LWFT_COMMON_HAS_DLL) && (LWFT_COMMON_HAS_DLL == 1)
#  if defined (LWFT_COMMON_BUILD_DLL)
#    define LWFT_Common_Export ACE_Proper_Export_Flag
#    define LWFT_COMMON_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define LWFT_COMMON_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* LWFT_COMMON_BUILD_DLL */
#    define LWFT_Common_Export ACE_Proper_Import_Flag
#    define LWFT_COMMON_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define LWFT_COMMON_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* LWFT_COMMON_BUILD_DLL */
#else /* LWFT_COMMON_HAS_DLL == 1 */
#  define LWFT_Common_Export
#  define LWFT_COMMON_SINGLETON_DECLARATION(T)
#  define LWFT_COMMON_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* LWFT_COMMON_HAS_DLL == 1 */

#endif /* LWFT_COMMON_EXPORT_H */

// End of auto generated file.
