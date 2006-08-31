// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s Domain
// ------------------------------
#ifndef DOMAIN_EXPORT_H
#define DOMAIN_EXPORT_H

#if !defined (DOMAIN_HAS_DLL)
#  define DOMAIN_HAS_DLL 1
#endif /* ! DOMAIN_HAS_DLL */

#if defined (DOMAIN_HAS_DLL) && (DOMAIN_HAS_DLL == 1)
#  if defined (DOMAIN_BUILD_DLL)
#    define Domain_Export __declspec(dllexport)
#  else /* DOMAIN_BUILD_DLL */
#    define Domain_Export __declspec(dllimport)
#  endif /* DOMAIN_BUILD_DLL */
#else /* DOMAIN_HAS_DLL == 1 */
#  define Domain_Export
#endif /* DOMAIN_HAS_DLL == 1 */

#endif /* DOMAIN_EXPORT_H */

// End of auto generated file.
