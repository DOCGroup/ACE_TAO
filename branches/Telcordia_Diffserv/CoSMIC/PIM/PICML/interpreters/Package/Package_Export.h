// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s Package
// ------------------------------
#ifndef PACKAGE_EXPORT_H
#define PACKAGE_EXPORT_H

#if !defined (PACKAGE_HAS_DLL)
#  define PACKAGE_HAS_DLL 1
#endif /* ! PACKAGE_HAS_DLL */

#if defined (PACKAGE_HAS_DLL) && (PACKAGE_HAS_DLL == 1)
#  if defined (PACKAGE_BUILD_DLL)
#    define Package_Export __declspec(dllexport)
#  else /* PACKAGE_BUILD_DLL */
#    define Package_Export __declspec(dllimport)
#  endif /* PACKAGE_BUILD_DLL */
#else /* PACKAGE_HAS_DLL == 1 */
#  define Package_Export
#endif /* PACKAGE_HAS_DLL == 1 */

#endif /* PACKAGE_EXPORT_H */

// End of auto generated file.
