
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl DDS4CCM_Logger
// ------------------------------
#ifndef DDS4CCM_LOGGER_EXPORT_H
#define DDS4CCM_LOGGER_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (DDS4CCM_LOGGER_HAS_DLL)
#  define DDS4CCM_LOGGER_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && DDS4CCM_LOGGER_HAS_DLL */

#if !defined (DDS4CCM_LOGGER_HAS_DLL)
#  define DDS4CCM_LOGGER_HAS_DLL 1
#endif /* ! DDS4CCM_LOGGER_HAS_DLL */

#if defined (DDS4CCM_LOGGER_HAS_DLL) && (DDS4CCM_LOGGER_HAS_DLL == 1)
#  if defined (DDS4CCM_LOGGER_BUILD_DLL)
#    define DDS4CCM_Logger_Export ACE_Proper_Export_Flag
#    define DDS4CCM_LOGGER_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define DDS4CCM_LOGGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* DDS4CCM_LOGGER_BUILD_DLL */
#    define DDS4CCM_Logger_Export ACE_Proper_Import_Flag
#    define DDS4CCM_LOGGER_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define DDS4CCM_LOGGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* DDS4CCM_LOGGER_BUILD_DLL */
#else /* DDS4CCM_LOGGER_HAS_DLL == 1 */
#  define DDS4CCM_Logger_Export
#  define DDS4CCM_LOGGER_SINGLETON_DECLARATION(T)
#  define DDS4CCM_LOGGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* DDS4CCM_LOGGER_HAS_DLL == 1 */

// Set DDS4CCM_LOGGER_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (DDS4CCM_LOGGER_NTRACE)
#  if (ACE_NTRACE == 1)
#    define DDS4CCM_LOGGER_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define DDS4CCM_LOGGER_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !DDS4CCM_LOGGER_NTRACE */

#if (DDS4CCM_LOGGER_NTRACE == 1)
#  define DDS4CCM_LOGGER_TRACE(X)
#else /* (DDS4CCM_LOGGER_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define DDS4CCM_LOGGER_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (DDS4CCM_LOGGER_NTRACE == 1) */

#endif /* DDS4CCM_LOGGER_EXPORT_H */

// End of auto generated file.
