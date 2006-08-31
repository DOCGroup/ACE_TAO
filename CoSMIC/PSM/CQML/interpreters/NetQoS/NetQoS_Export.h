// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s NetQoS
// ------------------------------
#ifndef NETQOS_EXPORT_H
#define NETQOS_EXPORT_H

#if !defined (NETQOS_HAS_DLL)
#  define NETQOS_HAS_DLL 1
#endif /* ! NETQOS_HAS_DLL */

#if defined (NETQOS_HAS_DLL) && (NETQOS_HAS_DLL == 1)
#  if defined (NETQOS_BUILD_DLL)
#    define NetQoS_Export __declspec(dllexport)
#  else /* NETQOS_BUILD_DLL */
#    define NetQoS_Export __declspec(dllimport)
#  endif /* NETQOS_BUILD_DLL */
#else /* NETQOS_HAS_DLL == 1 */
#  define NetQoS_Export
#endif /* NETQOS_HAS_DLL == 1 */

#endif /* NETQOS_EXPORT_H */


// End of auto generated file.

