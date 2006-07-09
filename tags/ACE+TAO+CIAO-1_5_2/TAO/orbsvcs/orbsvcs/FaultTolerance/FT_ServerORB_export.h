
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl TAO_FT_ServerORB
// ------------------------------
#ifndef TAO_FT_SERVERORB_EXPORT_H
#define TAO_FT_SERVERORB_EXPORT_H

#include "ace/config-all.h"

#if defined (TAO_AS_STATIC_LIBS)
#  if !defined (TAO_FT_SERVERORB_HAS_DLL)
#    define TAO_FT_SERVERORB_HAS_DLL 0
#  endif /* ! TAO_FT_SERVERORB_HAS_DLL */
#else
#  if !defined (TAO_FT_SERVERORB_HAS_DLL)
#    define TAO_FT_SERVERORB_HAS_DLL 1
#  endif /* ! TAO_FT_SERVERORB_HAS_DLL */
#endif

#if defined (TAO_FT_SERVERORB_HAS_DLL) && (TAO_FT_SERVERORB_HAS_DLL == 1)
#  if defined (TAO_FT_SERVERORB_BUILD_DLL)
#    define TAO_FT_ServerORB_Export ACE_Proper_Export_Flag
#    define TAO_FT_SERVERORB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TAO_FT_SERVERORB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TAO_FT_SERVERORB_BUILD_DLL */
#    define TAO_FT_ServerORB_Export ACE_Proper_Import_Flag
#    define TAO_FT_SERVERORB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TAO_FT_SERVERORB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TAO_FT_SERVERORB_BUILD_DLL */
#else /* TAO_FT_SERVERORB_HAS_DLL == 1 */
#  define TAO_FT_ServerORB_Export
#  define TAO_FT_SERVERORB_SINGLETON_DECLARATION(T)
#  define TAO_FT_SERVERORB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TAO_FT_SERVERORB_HAS_DLL == 1 */

// Set TAO_FT_SERVERORB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (TAO_FT_SERVERORB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define TAO_FT_SERVERORB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define TAO_FT_SERVERORB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !TAO_FT_SERVERORB_NTRACE */

#if (TAO_FT_SERVERORB_NTRACE == 1)
#  define TAO_FT_SERVERORB_TRACE(X)
#else /* (TAO_FT_SERVERORB_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define TAO_FT_SERVERORB_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (TAO_FT_SERVERORB_NTRACE == 1) */

#endif /* TAO_FT_SERVERORB_EXPORT_H */

// End of auto generated file.
