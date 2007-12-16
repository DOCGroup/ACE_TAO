
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef PLUGTOP_EXPORT_H
#define PLUGTOP_EXPORT_H

#include "ace/config-all.h"

#if defined (TAO_AS_STATIC_LIBS)
#  if !defined (PLUGTOP_HAS_DLL)
#    define PLUGTOP_HAS_DLL 0
#  endif /* ! PLUGTOP_HAS_DLL */
#else
#  if !defined (PLUGTOP_HAS_DLL)
#    define PLUGTOP_HAS_DLL 1
#  endif /* ! PLUGTOP_HAS_DLL */
#endif

#if defined (PLUGTOP_HAS_DLL) && (PLUGTOP_HAS_DLL == 1)
#  if defined (PLUGTOP_BUILD_DLL)
#    define PLUGTOP_Export ACE_Proper_Export_Flag
#    define PLUGTOP_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define PLUGTOP_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* PLUGTOP_BUILD_DLL */
#    define PLUGTOP_Export ACE_Proper_Import_Flag
#    define PLUGTOP_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define PLUGTOP_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* PLUGTOP_BUILD_DLL */
#else /* PLUGTOP_HAS_DLL == 1 */
#  define PLUGTOP_Export
#  define PLUGTOP_SINGLETON_DECLARATION(T)
#  define PLUGTOP_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* PLUGTOP_HAS_DLL == 1 */

#endif /* PLUGTOP_EXPORT_H */

// End of auto generated file.
