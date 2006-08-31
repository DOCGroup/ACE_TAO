// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// ------------------------------

#ifndef OCML_ENGINE_EXPORT_H
#define OCML_ENGINE_EXPORT_H

#if !defined (OCML_ENGINE_HAS_DLL)
#  define OCML_ENGINE_HAS_DLL 1
#endif /* ! OCML_ENGINE_HAS_DLL */

#if defined (OCML_ENGINE_HAS_DLL) && (OCML_ENGINE_HAS_DLL == 1)
#  if defined (OCML_ENGINE_BUILD_DLL)
#    define OCML_Engine_Export __declspec(dllexport)
#  else /* OCML_ENGINE_BUILD_DLL */
#    define OCML_Engine_Export __declspec(dllimport)
#  endif /* OCML_ENGINE_BUILD_DLL */
#else /* OCML_ENGINE_HAS_DLL == 1 */
#  define OCML_Engine_Export
#endif /* OCML_ENGINE_HAS_DLL == 1 */

#endif /* OCML_ENGINE_EXPORT_H */

// End of auto generated file.
