// -*- C++ -*-
// This file was copied and edited from the FlatPlan interpreter.

#ifndef FAULTTOLERANCE_EXPORT_H
#define FAULTTOLERANCE_EXPORT_H

#if !defined (FAULTTOLERANCE_HAS_DLL)
#  define FAULTTOLERANCE_HAS_DLL 1
#endif /* ! FAULTTOLERANCE_HAS_DLL */

#if defined (FAULTTOLERANCE_HAS_DLL) && (FAULTTOLERANCE_HAS_DLL == 1)
#  if defined (FAULTTOLERANCE_BUILD_DLL)
#    define FaultTolerance_Export __declspec(dllexport)
#  else /* FAULTTOLERANCE_BUILD_DLL */
#    define FaultTolerance_Export __declspec(dllimport)
#  endif /* FAULTTOLERANCE_BUILD_DLL */
#else /* FAULTTOLERANCE_HAS_DLL == 1 */
#  define FaultTolerance_Export
#endif /* FAULTTOLERANCE_HAS_DLL == 1 */

#endif /* FAULTTOLERANCE_EXPORT_H */

// End of auto generated file.