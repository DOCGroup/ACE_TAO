
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef IDL_TO_PICML_BE_EXPORT_H
#define IDL_TO_PICML_BE_EXPORT_H

#include "ace/config-all.h"

#if defined (TAO_AS_STATIC_LIBS)
#  if !defined (IDL_TO_PICML_BE_HAS_DLL)
#    define IDL_TO_PICML_BE_HAS_DLL 0
#  endif /* ! IDL_TO_PICML_BE_HAS_DLL */
#else
#  if !defined (IDL_TO_PICML_BE_HAS_DLL)
#    define IDL_TO_PICML_BE_HAS_DLL 1
#  endif /* ! IDL_TO_PICML_BE_HAS_DLL */
#endif

#if defined (IDL_TO_PICML_BE_HAS_DLL) && (IDL_TO_PICML_BE_HAS_DLL == 1)
#  if defined (IDL_TO_PICML_BE_BUILD_DLL)
#    define IDL_TO_PICML_BE_Export ACE_Proper_Export_Flag
#    define IDL_TO_PICML_BE_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define IDL_TO_PICML_BE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* IDL_TO_PICML_BE_BUILD_DLL */
#    define IDL_TO_PICML_BE_Export ACE_Proper_Import_Flag
#    define IDL_TO_PICML_BE_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define IDL_TO_PICML_BE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* IDL_TO_PICML_BE_BUILD_DLL */
#else /* IDL_TO_PICML_BE_HAS_DLL == 1 */
#  define IDL_TO_PICML_BE_Export
#  define IDL_TO_PICML_BE_SINGLETON_DECLARATION(T)
#  define IDL_TO_PICML_BE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* IDL_TO_PICML_BE_HAS_DLL == 1 */

#endif /* IDL_TO_PICML_BE_EXPORT_H */

// End of auto generated file.
