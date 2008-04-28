// -*- C++ -*-

//=============================================================================
/**
 *  @file    MonitorControl_export.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef MONITORCONTROL_EXPORT_H
#define MONITORCONTROL_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS)
# if !defined (MONITORCONTROL_HAS_DLL)
#   define MONITORCONTROL_HAS_DLL 0
# endif /* ! MONITORCONTROL_HAS_DLL */
#else
# if !defined (MONITORCONTROL_HAS_DLL)
#   define MONITORCONTROL_HAS_DLL 1
# endif /* ! MONITORCONTROL_HAS_DLL */
#endif /* ACE_AS_STATIC_LIB */

#if defined (MONITORCONTROL_HAS_DLL) && (MONITORCONTROL_HAS_DLL == 1)
#  if defined (MONITORCONTROL_BUILD_DLL)
#    define MONITORCONTROL_Export ACE_Proper_Export_Flag
#    define MONITORCONTROL_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define MONITORCONTROL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* MONITORCONTROL_BUILD_DLL */
#    define MONITORCONTROL_Export ACE_Proper_Import_Flag
#    define MONITORCONTROL_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define MONITORCONTROL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* MONITORCONTROL_BUILD_DLL */
#else /* MONITORCONTROL_HAS_DLL == 1 */
#  define MONITORCONTROL_Export
#  define MONITORCONTROL_SINGLETON_DECLARATION(T)
#  define MONITORCONTROL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* MONITORCONTROL_HAS_DLL == 1 */

#endif /* MONITORCONTROL_EXPORT_H */

// End of auto generated file.
