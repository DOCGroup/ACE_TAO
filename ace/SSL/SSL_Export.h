// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by
// ${ACE_ROOT}/GenExportH.BAT
// ------------------------------
#ifndef ACE_SSL_EXPORT_H
#define ACE_SSL_EXPORT_H

#include "ace/OS.h"

#if !defined (ACE_SSL_HAS_DLL)
#define ACE_SSL_HAS_DLL 1
#endif /* ! ACE_SSL_HAS_DLL */

#if defined (ACE_SSL_HAS_DLL)
#  if (ACE_SSL_HAS_DLL == 1)
#    if defined (ACE_SSL_BUILD_DLL)
#      define ACE_SSL_Export ACE_Proper_Export_Flag
#      define ACE_SSL_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    else
#      define ACE_SSL_Export ACE_Proper_Import_Flag
#      define ACE_SSL_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    endif /* ACE_SSL_BUILD_DLL */
#  else
#    define ACE_SSL_Export
#    define ACE_SSL_SINGLETON_DECLARATION(T)
#  endif   /* ! ACE_SSL_HAS_DLL == 1 */
#else
#  define ACE_SSL_Export
#  define ACE_SSL_SINGLETON_DECLARATION(T)
#endif     /* ACE_SSL_HAS_DLL */

#endif     /* ACE_SSL_EXPORT_H */

// End of auto generated file.
