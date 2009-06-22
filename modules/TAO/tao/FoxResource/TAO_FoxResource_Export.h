
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s TAO_FoxResource
// ------------------------------
#ifndef TAO_FOXRESOURCE_EXPORT_H
#define TAO_FOXRESOURCE_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (TAO_FOXRESOURCE_HAS_DLL)
#  define TAO_FOXRESOURCE_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && TAO_FOXRESOURCE_HAS_DLL */

#if !defined (TAO_FOXRESOURCE_HAS_DLL)
#  define TAO_FOXRESOURCE_HAS_DLL 1
#endif /* ! TAO_FOXRESOURCE_HAS_DLL */

#if defined (TAO_FOXRESOURCE_HAS_DLL) && (TAO_FOXRESOURCE_HAS_DLL == 1)
#  if defined (TAO_FOXRESOURCE_BUILD_DLL)
#    define TAO_FoxResource_Export ACE_Proper_Export_Flag
#    define TAO_FOXRESOURCE_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TAO_FOXRESOURCE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TAO_FOXRESOURCE_BUILD_DLL */
#    define TAO_FoxResource_Export ACE_Proper_Import_Flag
#    define TAO_FOXRESOURCE_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TAO_FOXRESOURCE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TAO_FOXRESOURCE_BUILD_DLL */
#else /* TAO_FOXRESOURCE_HAS_DLL == 1 */
#  define TAO_FoxResource_Export
#  define TAO_FOXRESOURCE_SINGLETON_DECLARATION(T)
#  define TAO_FOXRESOURCE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TAO_FOXRESOURCE_HAS_DLL == 1 */

// Set TAO_FOXRESOURCE_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (TAO_FOXRESOURCE_NTRACE)
#  if (ACE_NTRACE == 1)
#    define TAO_FOXRESOURCE_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define TAO_FOXRESOURCE_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !TAO_FOXRESOURCE_NTRACE */

#if (TAO_FOXRESOURCE_NTRACE == 1)
#  define TAO_FOXRESOURCE_TRACE(X)
#else /* (TAO_FOXRESOURCE_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define TAO_FOXRESOURCE_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (TAO_FOXRESOURCE_NTRACE == 1) */

#endif /* TAO_FOXRESOURCE_EXPORT_H */

// End of auto generated file.
