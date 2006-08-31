// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s RTConfig
// ------------------------------
#ifndef RTConfig_EXPORT_H
#define RTConfig_EXPORT_H

#if !defined (RTConfig_HAS_DLL)
#  define RTConfig_HAS_DLL 1
#endif /* ! RTConfig_HAS_DLL */

#if defined (RTConfig_HAS_DLL) && (RTConfig_HAS_DLL == 1)
#  if defined (RTConfig_BUILD_DLL)
#    define RTConfig_Export __declspec(dllexport)
#  else /* RTConfig_BUILD_DLL */
#    define RTConfig_Export __declspec(dllimport)
#  endif /* RTConfig_BUILD_DLL */
#else /* RTConfig_HAS_DLL == 1 */
#  define RTConfig_Export
#endif /* RTConfig_HAS_DLL == 1 */

#endif /* RTConfig_EXPORT_H */

// End of auto generated file.
