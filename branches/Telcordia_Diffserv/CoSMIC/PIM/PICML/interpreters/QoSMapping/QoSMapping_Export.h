// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s Package
// ------------------------------
#ifndef QOSMAPPING_EXPORT_H
#define QOSMAPPING_EXPORT_H

#if !defined (QOSMAPPING_HAS_DLL)
#  define QOSMAPPING_HAS_DLL 1
#endif /* ! QOSMAPPING_HAS_DLL */

#if defined (QOSMAPPING_HAS_DLL) && (QOSMAPPING_HAS_DLL == 1)
#  if defined (QOSMAPPING_BUILD_DLL)
#    define QoSMapping_Export __declspec(dllexport)
#  else /* QOSMAPPING_BUILD_DLL */
#    define QoSMapping_Export __declspec(dllimport)
#  endif /* QOSMAPPING_BUILD_DLL */
#else /* QOSMAPPING_HAS_DLL == 1 */
#  define QoSMapping_Export
#endif /* QOSMAPPING_HAS_DLL == 1 */

#endif /* QOSMAPPING_EXPORT_H */

// End of auto generated file.
