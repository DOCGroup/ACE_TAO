/* -*- C++ -*- $Id$ */
// generate_export_file.pl,v 1.8 2001/03/04 09:16:29 nanbor Exp
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef TAO_RTEVENTLOG_EXPORT_H
#define TAO_RTEVENTLOG_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (TAO_RTEVENTLOG_HAS_DLL)
#  define TAO_RTEVENTLOG_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && TAO_RTEVENTLOG_HAS_DLL */

#if !defined (TAO_RTEVENTLOG_HAS_DLL)
#  define TAO_RTEVENTLOG_HAS_DLL 1
#endif /* ! TAO_RTEVENTLOG_HAS_DLL */

#if defined (TAO_RTEVENTLOG_HAS_DLL) && (TAO_RTEVENTLOG_HAS_DLL == 1)
#  if defined (TAO_RTEVENTLOG_BUILD_DLL)
#    define TAO_RTEventLog_Export ACE_Proper_Export_Flag
#    define TAO_RTEVENTLOG_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TAO_RTEVENTLOG_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TAO_RTEVENTLOG_BUILD_DLL */
#    define TAO_RTEventLog_Export ACE_Proper_Import_Flag
#    define TAO_RTEVENTLOG_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TAO_RTEVENTLOG_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TAO_RTEVENTLOG_BUILD_DLL */
#else /* TAO_RTEVENTLOG_HAS_DLL == 1 */
#  define TAO_RTEventLog_Export
#  define TAO_RTEVENTLOG_SINGLETON_DECLARATION(T)
#  define TAO_RTEVENTLOG_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TAO_RTEVENTLOG_HAS_DLL == 1 */

#endif /* TAO_RTEVENTLOG_EXPORT_H */

// End of auto generated file.
