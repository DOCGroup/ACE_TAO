// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s IOAutomata
// ------------------------------
#ifndef IOAUTOMATA_EXPORT_H
#define IOAUTOMATA_EXPORT_H

#if !defined (IOAUTOMATA_HAS_DLL)
#  define IOAUTOMATA_HAS_DLL 1
#endif /* ! IOAUTOMATA_HAS_DLL */

#if defined (IOAUTOMATA_HAS_DLL) && (IOAUTOMATA_HAS_DLL == 1)
#  if defined (IOAUTOMATA_BUILD_DLL)
#    define IOAutomata_Export __declspec(dllexport)
#  else /* IOAUTOMATA_BUILD_DLL */
#    define IOAutomata_Export __declspec(dllimport)
#  endif /* IOAUTOMATA_BUILD_DLL */
#else /* IOAUTOMATA_HAS_DLL == 1 */
#  define IOAutomata_Export
#endif /* IOAUTOMATA_HAS_DLL == 1 */

#endif /* IOAUTOMATA_EXPORT_H */


// End of auto generated file.

