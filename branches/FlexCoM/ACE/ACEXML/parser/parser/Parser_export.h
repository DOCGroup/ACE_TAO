// -*- C++ -*-

//=============================================================================
/**
 *  @file    Parser_export.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef ACEXML_PARSER_EXPORT_H
#define ACEXML_PARSER_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (ACEXML_PARSER_HAS_DLL)
#  define ACEXML_PARSER_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && ACEXML_PARSER_HAS_DLL */

#if !defined (ACEXML_PARSER_HAS_DLL)
#  define ACEXML_PARSER_HAS_DLL 1
#endif /* ! ACEXML_PARSER_HAS_DLL */

#if defined (ACEXML_PARSER_HAS_DLL) && (ACEXML_PARSER_HAS_DLL == 1)
#  if defined (ACEXML_PARSER_BUILD_DLL)
#    define ACEXML_PARSER_Export ACE_Proper_Export_Flag
#    define ACEXML_PARSER_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define ACEXML_PARSER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* ACEXML_PARSER_BUILD_DLL */
#    define ACEXML_PARSER_Export ACE_Proper_Import_Flag
#    define ACEXML_PARSER_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define ACEXML_PARSER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* ACEXML_PARSER_BUILD_DLL */
#else /* ACEXML_PARSER_HAS_DLL == 1 */
#  define ACEXML_PARSER_Export
#  define ACEXML_PARSER_SINGLETON_DECLARATION(T)
#  define ACEXML_PARSER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* ACEXML_PARSER_HAS_DLL == 1 */

#endif /* ACEXML_PARSER_EXPORT_H */

// End of auto generated file.
