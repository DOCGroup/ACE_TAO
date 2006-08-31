// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s BGML
// ------------------------------
#ifndef BGML_EXPORT_H
#define BGML_EXPORT_H

#if !defined (BGML_HAS_DLL)
#  define BGML_HAS_DLL 1
#endif /* ! BGML_HAS_DLL */

#if defined (BGML_HAS_DLL) && (BGML_HAS_DLL == 1)
#  if defined (BGML_BUILD_DLL)
#    define BGML_Export __declspec(dllexport)
#  else /* BGML_BUILD_DLL */
#    define BGML_Export __declspec(dllimport)
#  endif /* BGML_BUILD_DLL */
#else /* BGML_HAS_DLL == 1 */
#  define BGML_Export
#endif /* BGML_HAS_DLL == 1 */

#endif /* BGML_EXPORT_H */

// End of auto generated file.
