
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef TAO_MESSAGING_EXPORT_H
#define TAO_MESSAGING_EXPORT_H

#include "ace/config-all.h"

#if !defined (TAO_MESSAGING_HAS_DLL)
#  define TAO_MESSAGING_HAS_DLL 1
#endif /* ! TAO_MESSAGING_HAS_DLL */

#if defined (TAO_MESSAGING_HAS_DLL) && (TAO_MESSAGING_HAS_DLL == 1)
#  if defined (TAO_MESSAGING_BUILD_DLL)
#    define TAO_Messaging_Export ACE_Proper_Export_Flag
#    define TAO_MESSAGING_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TAO_MESSAGING_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TAO_MESSAGING_BUILD_DLL */
#    define TAO_Messaging_Export ACE_Proper_Import_Flag
#    define TAO_MESSAGING_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TAO_MESSAGING_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TAO_MESSAGING_BUILD_DLL */
#else /* TAO_MESSAGING_HAS_DLL == 1 */
#  define TAO_Messaging_Export
#  define TAO_MESSAGING_SINGLETON_DECLARATION(T)
#  define TAO_MESSAGING_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TAO_MESSAGING_HAS_DLL == 1 */

#endif /* TAO_MESSAGING_EXPORT_H */

// End of auto generated file.
