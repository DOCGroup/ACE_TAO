
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef ACTIVITY_EXPORT_H
#define ACTIVITY_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (ACTIVITY_HAS_DLL)
#  define ACTIVITY_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && ACTIVITY_HAS_DLL */

#if !defined (ACTIVITY_HAS_DLL)
#  define ACTIVITY_HAS_DLL 1
#endif /* ! ACTIVITY_HAS_DLL */

#if defined (ACTIVITY_HAS_DLL) && (ACTIVITY_HAS_DLL == 1)
#  if defined (ACTIVITY_BUILD_DLL)
#    define activity_Export ACE_Proper_Export_Flag
#    define ACTIVITY_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define ACTIVITY_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* ACTIVITY_BUILD_DLL */
#    define activity_Export ACE_Proper_Import_Flag
#    define ACTIVITY_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define ACTIVITY_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* ACTIVITY_BUILD_DLL */
#else /* ACTIVITY_HAS_DLL == 1 */
#  define activity_Export
#  define ACTIVITY_SINGLETON_DECLARATION(T)
#  define ACTIVITY_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* ACTIVITY_HAS_DLL == 1 */

#endif /* ACTIVITY_EXPORT_H */

// End of auto generated file.
