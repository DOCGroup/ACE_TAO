
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s BGML
// ------------------------------
#ifndef MPC_EXPORT_H
#define MPC_EXPORT_H

#if !defined (MPC_HAS_DLL)
#  define MPC_HAS_DLL 1
#endif /* ! BGML_HAS_DLL */

#if defined (MPC_HAS_DLL) && (MPC_HAS_DLL == 1)
#  if defined (MPC_BUILD_DLL)
#    define MPC_Export __declspec(dllexport)
#  else /* MPC_BUILD_DLL */
#    define MPC_Export __declspec(dllimport)
#  endif /* MPC_BUILD_DLL */
#else /* MPC_HAS_DLL == 1 */
#  define MPC_Export
#endif /* MPC_HAS_DLL == 1 */

#endif /* MPC_EXPORT_H */

// End of auto generated file.
