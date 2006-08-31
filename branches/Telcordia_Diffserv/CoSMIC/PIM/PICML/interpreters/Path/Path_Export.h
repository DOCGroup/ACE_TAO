
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s Path
// ------------------------------
#ifndef PATH_EXPORT_H
#define PATH_EXPORT_H

#if !defined (PATH_HAS_DLL)
#  define PATH_HAS_DLL 1
#endif /* ! PATH_HAS_DLL */

#if defined (PATH_HAS_DLL) && (PATH_HAS_DLL == 1)
#  if defined (PATH_BUILD_DLL)
#    define Path_Export __declspec(dllexport)
#  else /* PATH_BUILD_DLL */
#    define Path_Export __declspec(dllimport)
#  endif /* PATH_BUILD_DLL */
#else /* PATH_HAS_DLL == 1 */
#  define Path_Export
#endif /* PATH_HAS_DLL == 1 */

#endif /* PATH_EXPORT_H */

// End of auto generated file.