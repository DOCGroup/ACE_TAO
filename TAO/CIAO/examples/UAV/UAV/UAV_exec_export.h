
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl UAV_EXEC
// ------------------------------
#ifndef UAV_EXEC_EXPORT_H
#define UAV_EXEC_EXPORT_H

#include "ace/config-all.h"

#if !defined (UAV_EXEC_HAS_DLL)
#  define UAV_EXEC_HAS_DLL 1
#endif /* ! UAV_EXEC_HAS_DLL */

#if defined (UAV_EXEC_HAS_DLL) && (UAV_EXEC_HAS_DLL == 1)
#  if defined (UAV_EXEC_BUILD_DLL)
#    define UAV_EXEC_Export ACE_Proper_Export_Flag
#    define UAV_EXEC_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define UAV_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* UAV_EXEC_BUILD_DLL */
#    define UAV_EXEC_Export ACE_Proper_Import_Flag
#    define UAV_EXEC_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define UAV_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* UAV_EXEC_BUILD_DLL */
#else /* UAV_EXEC_HAS_DLL == 1 */
#  define UAV_EXEC_Export
#  define UAV_EXEC_SINGLETON_DECLARATION(T)
#  define UAV_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* UAV_EXEC_HAS_DLL == 1 */

// Set UAV_EXEC_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (UAV_EXEC_NTRACE)
#  if (ACE_NTRACE == 1)
#    define UAV_EXEC_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define UAV_EXEC_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !UAV_EXEC_NTRACE */

#if (UAV_EXEC_NTRACE == 1)
#  define UAV_EXEC_TRACE(X)
#else /* (UAV_EXEC_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define UAV_EXEC_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (UAV_EXEC_NTRACE == 1) */

#endif /* UAV_EXEC_EXPORT_H */

// End of auto generated file.
