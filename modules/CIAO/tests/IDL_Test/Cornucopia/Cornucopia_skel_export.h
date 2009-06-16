
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl CORNUCOPIA_SKEL
// ------------------------------
#ifndef CORNUCOPIA_SKEL_EXPORT_H
#define CORNUCOPIA_SKEL_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (CORNUCOPIA_SKEL_HAS_DLL)
#  define CORNUCOPIA_SKEL_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && CORNUCOPIA_SKEL_HAS_DLL */

#if !defined (CORNUCOPIA_SKEL_HAS_DLL)
#  define CORNUCOPIA_SKEL_HAS_DLL 1
#endif /* ! CORNUCOPIA_SKEL_HAS_DLL */

#if defined (CORNUCOPIA_SKEL_HAS_DLL) && (CORNUCOPIA_SKEL_HAS_DLL == 1)
#  if defined (CORNUCOPIA_SKEL_BUILD_DLL)
#    define CORNUCOPIA_SKEL_Export ACE_Proper_Export_Flag
#    define CORNUCOPIA_SKEL_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define CORNUCOPIA_SKEL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* CORNUCOPIA_SKEL_BUILD_DLL */
#    define CORNUCOPIA_SKEL_Export ACE_Proper_Import_Flag
#    define CORNUCOPIA_SKEL_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define CORNUCOPIA_SKEL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* CORNUCOPIA_SKEL_BUILD_DLL */
#else /* CORNUCOPIA_SKEL_HAS_DLL == 1 */
#  define CORNUCOPIA_SKEL_Export
#  define CORNUCOPIA_SKEL_SINGLETON_DECLARATION(T)
#  define CORNUCOPIA_SKEL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* CORNUCOPIA_SKEL_HAS_DLL == 1 */

// Set CORNUCOPIA_SKEL_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (CORNUCOPIA_SKEL_NTRACE)
#  if (ACE_NTRACE == 1)
#    define CORNUCOPIA_SKEL_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define CORNUCOPIA_SKEL_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !CORNUCOPIA_SKEL_NTRACE */

#if (CORNUCOPIA_SKEL_NTRACE == 1)
#  define CORNUCOPIA_SKEL_TRACE(X)
#else /* (CORNUCOPIA_SKEL_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define CORNUCOPIA_SKEL_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (CORNUCOPIA_SKEL_NTRACE == 1) */

#endif /* CORNUCOPIA_SKEL_EXPORT_H */

// End of auto generated file.
