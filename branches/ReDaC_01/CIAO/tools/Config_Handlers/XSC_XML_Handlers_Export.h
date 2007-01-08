
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl XSC_XML_Handlers
// ------------------------------
#ifndef XSC_XML_HANDLERS_EXPORT_H
#define XSC_XML_HANDLERS_EXPORT_H

#include "ace/config-all.h"

#if !defined (XSC_XML_HANDLERS_HAS_DLL)
#  define XSC_XML_HANDLERS_HAS_DLL 1
#endif /* ! XSC_XML_HANDLERS_HAS_DLL */

#if defined (XSC_XML_HANDLERS_HAS_DLL) && (XSC_XML_HANDLERS_HAS_DLL == 1)
#  if defined (XSC_XML_HANDLERS_BUILD_DLL)
#    define XSC_XML_Handlers_Export ACE_Proper_Export_Flag
#    define XSC_XML_HANDLERS_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define XSC_XML_HANDLERS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* XSC_XML_HANDLERS_BUILD_DLL */
#    define XSC_XML_Handlers_Export ACE_Proper_Import_Flag
#    define XSC_XML_HANDLERS_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define XSC_XML_HANDLERS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* XSC_XML_HANDLERS_BUILD_DLL */
#else /* XSC_XML_HANDLERS_HAS_DLL == 1 */
#  define XSC_XML_Handlers_Export
#  define XSC_XML_HANDLERS_SINGLETON_DECLARATION(T)
#  define XSC_XML_HANDLERS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* XSC_XML_HANDLERS_HAS_DLL == 1 */

// Set XSC_XML_HANDLERS_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (XSC_XML_HANDLERS_NTRACE)
#  if (ACE_NTRACE == 1)
#    define XSC_XML_HANDLERS_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define XSC_XML_HANDLERS_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !XSC_XML_HANDLERS_NTRACE */

#if (XSC_XML_HANDLERS_NTRACE == 1)
#  define XSC_XML_HANDLERS_TRACE(X)
#else /* (XSC_XML_HANDLERS_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define XSC_XML_HANDLERS_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (XSC_XML_HANDLERS_NTRACE == 1) */

#endif /* XSC_XML_HANDLERS_EXPORT_H */

// End of auto generated file.
