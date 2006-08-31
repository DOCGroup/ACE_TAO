
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s CIDL
// ------------------------------
#ifndef CIDL_EXPORT_H
#define CIDL_EXPORT_H

#if !defined (CIDL_HAS_DLL)
#  define CIDL_HAS_DLL 1
#endif /* ! BGML_HAS_DLL */

#if defined (CIDL_HAS_DLL) && (CIDL_HAS_DLL == 1)
#  if defined (CIDL_BUILD_DLL)
#    define CIDL_Export __declspec(dllexport)
#  else /* CIDL_BUILD_DLL */
#    define CIDL_Export __declspec(dllimport)
#  endif /* CIDL_BUILD_DLL */
#else /* CIDL_HAS_DLL == 1 */
#  define CIDL_Export
#endif /* CIDL_HAS_DLL == 1 */

#endif /* CIDL_EXPORT_H */

// End of auto generated file.
