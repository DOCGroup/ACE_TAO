// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// ------------------------------

#ifndef OCML_COMMON_EXPORT_H
#define OCML_COMMON_EXPORT_H

#if !defined (OCML_COMMON_HAS_DLL)
#  define OCML_COMMON_HAS_DLL 1
#endif /* ! OCML_COMMON_HAS_DLL */

#if defined (OCML_COMMON_HAS_DLL) && (OCML_COMMON_HAS_DLL == 1)
#  if defined (OCML_COMMON_BUILD_DLL)
#    define OCML_Common_Export __declspec(dllexport)
#  else /* OCML_COMMON_BUILD_DLL */
#    define OCML_Common_Export __declspec(dllimport)
#  endif /* OCML_COMMON_BUILD_DLL */
#else /* OCML_COMMON_HAS_DLL == 1 */
#  define OCML_Common_Export
#endif /* OCML_COMMON_HAS_DLL == 1 */

#endif /* OCML_COMMON_EXPORT_H */

// End of auto generated file.
