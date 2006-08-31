// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// ------------------------------

#ifndef CONFIGURATOR_UI_EXPORT_H
#define CONFIGURATOR_UI_EXPORT_H

#if !defined (CONFIGURATOR_UI_HAS_DLL)
#  define CONFIGURATOR_UI_HAS_DLL 1
#endif /* ! CONFIGURATOR_UI_HAS_DLL */

#if defined (CONFIGURATOR_UI_HAS_DLL) && (CONFIGURATOR_UI_HAS_DLL == 1)
#  if defined (CONFIGURATOR_UI_BUILD_DLL)
#    define CONFIGURATOR_UI_Export __declspec(dllexport)
#  else /* CONFIGURATOR_UI_BUILD_DLL */
#    define CONFIGURATOR_UI_Export __declspec(dllimport)
#  endif /* CONFIGURATOR_UI_BUILD_DLL */
#else /* CONFIGURATOR_UI_HAS_DLL == 1 */
#  define CONFIGURATOR_UI_Export
#endif /* CONFIGURATOR_UI_HAS_DLL == 1 */

#endif /* CONFIGURATOR_UI_EXPORT_H */

// End of auto generated file.
