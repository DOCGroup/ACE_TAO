
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl MIF_DT_Creator
// ------------------------------
#ifndef MIF_DT_CREATOR_EXPORT_H
#define MIF_DT_CREATOR_EXPORT_H

#include "ace/config-all.h"

#if !defined (MIF_DT_CREATOR_HAS_DLL)
#  define MIF_DT_CREATOR_HAS_DLL 1
#endif /* ! MIF_DT_CREATOR_HAS_DLL */

#if defined (MIF_DT_CREATOR_HAS_DLL) && (MIF_DT_CREATOR_HAS_DLL == 1)
#  if defined (MIF_DT_CREATOR_BUILD_DLL)
#    define MIF_DT_Creator_Export ACE_Proper_Export_Flag
#    define MIF_DT_CREATOR_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define MIF_DT_CREATOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* MIF_DT_CREATOR_BUILD_DLL */
#    define MIF_DT_Creator_Export ACE_Proper_Import_Flag
#    define MIF_DT_CREATOR_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define MIF_DT_CREATOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* MIF_DT_CREATOR_BUILD_DLL */
#else /* MIF_DT_CREATOR_HAS_DLL == 1 */
#  define MIF_DT_Creator_Export
#  define MIF_DT_CREATOR_SINGLETON_DECLARATION(T)
#  define MIF_DT_CREATOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* MIF_DT_CREATOR_HAS_DLL == 1 */

// Set MIF_DT_CREATOR_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (MIF_DT_CREATOR_NTRACE)
#  if (ACE_NTRACE == 1)
#    define MIF_DT_CREATOR_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define MIF_DT_CREATOR_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !MIF_DT_CREATOR_NTRACE */

#if (MIF_DT_CREATOR_NTRACE == 1)
#  define MIF_DT_CREATOR_TRACE(X)
#else /* (MIF_DT_CREATOR_NTRACE == 1) */
#  define MIF_DT_CREATOR_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (MIF_DT_CREATOR_NTRACE == 1) */

#endif /* MIF_DT_CREATOR_EXPORT_H */

// End of auto generated file.
