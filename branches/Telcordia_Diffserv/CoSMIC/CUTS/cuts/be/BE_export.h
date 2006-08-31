
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl CUTS_BE
// ------------------------------
#ifndef CUTS_BE_EXPORT_H
#define CUTS_BE_EXPORT_H

#include "cuts/config.h"

#if defined (PICML_HAS_DLL) && (PICML_HAS_DLL == 1)
#  if defined (CUTS_BE_BUILD_DLL)
#    define CUTS_BE_Export __declspec (dllexport)
#  else /* CUTS_UDM_CIAO_BUILD_DLL */
#    define CUTS_BE_Export __declspec (dllimport)
#  endif /* CUTS_UDM_CIAO_BUILD_DLL */
#else /* CUTS_UDM_CIAO_HAS_DLL == 1 */
#  define CUTS_BE_Export
#endif /* CUTS_UDM_CIAO_HAS_DLL == 1 */

#endif /* CUTS_BE_EXPORT_H */

// End of auto generated file.
