// -*- C++ -*-

//=============================================================================
/**
 *  @file    ACEXML_Export.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef ACEXML_EXPORT_H
#define ACEXML_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (ACEXML_HAS_DLL)
#  define ACEXML_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && ACEXML_HAS_DLL */

#if defined (ACE_HAS_DLL)
#  if !defined (ACEXML_HAS_DLL)
#    define ACEXML_HAS_DLL 1
#  endif /* ! ACEXML_HAS_DLL */
#else
#  if !defined (ACEXML_HAS_DLL)
#    define ACEXML_HAS_DLL 0
#  endif /* ! ACEXML_HAS_DLL */
#endif

#if defined (ACEXML_HAS_DLL) && (ACEXML_HAS_DLL == 1)
#  if defined (ACEXML_BUILD_DLL)
#    define ACEXML_Export ACE_Proper_Export_Flag
#    define ACEXML_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define ACEXML_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* ACEXML_BUILD_DLL */
#    define ACEXML_Export ACE_Proper_Import_Flag
#    define ACEXML_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define ACEXML_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* ACEXML_BUILD_DLL */
#else /* ACEXML_HAS_DLL == 1 */
#  define ACEXML_Export
#  define ACEXML_SINGLETON_DECLARATION(T)
#  define ACEXML_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* ACEXML_HAS_DLL == 1 */

#endif /* ACEXML_EXPORT_H */

// End of auto generated file.
