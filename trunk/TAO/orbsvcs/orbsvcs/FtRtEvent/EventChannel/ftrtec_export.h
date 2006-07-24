
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl TAO_FTRTEC
// ------------------------------
#ifndef TAO_FTRTEC_EXPORT_H
#define TAO_FTRTEC_EXPORT_H

#include "ace/config-all.h"

#if defined (TAO_AS_STATIC_LIBS)
#  if !defined (TAO_FTRTEC_HAS_DLL)
#    define TAO_FTRTEC_HAS_DLL 0
#  endif /* ! TAO_FTRTEC_HAS_DLL */
#else
#  if !defined (TAO_FTRTEC_HAS_DLL)
#    define TAO_FTRTEC_HAS_DLL 1
#  endif /* ! TAO_FTRTEC_HAS_DLL */
#endif

#if defined (TAO_FTRTEC_HAS_DLL) && (TAO_FTRTEC_HAS_DLL == 1)
#  if defined (TAO_FTRTEC_BUILD_DLL)
#    define TAO_FTRTEC_Export ACE_Proper_Export_Flag
#    define TAO_FTRTEC_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TAO_FTRTEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TAO_FTRTEC_BUILD_DLL */
#    define TAO_FTRTEC_Export ACE_Proper_Import_Flag
#    define TAO_FTRTEC_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TAO_FTRTEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TAO_FTRTEC_BUILD_DLL */
#else /* TAO_FTRTEC_HAS_DLL == 1 */
#  define TAO_FTRTEC_Export
#  define TAO_FTRTEC_SINGLETON_DECLARATION(T)
#  define TAO_FTRTEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TAO_FTRTEC_HAS_DLL == 1 */

// Set TAO_FTRTEC_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (TAO_FTRTEC_NTRACE)
#  if (ACE_NTRACE == 1)
#    define TAO_FTRTEC_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define TAO_FTRTEC_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !TAO_FTRTEC_NTRACE */

#if (TAO_FTRTEC_NTRACE == 1)
#  define TAO_FTRTEC_TRACE(X)
#else /* (TAO_FTRTEC_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define TAO_FTRTEC_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (TAO_FTRTEC_NTRACE == 1) */

#endif /* TAO_FTRTEC_EXPORT_H */

// End of auto generated file.
