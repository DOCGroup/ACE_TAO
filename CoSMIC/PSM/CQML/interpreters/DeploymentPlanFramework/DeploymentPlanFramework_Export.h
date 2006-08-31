// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s Package
// ------------------------------
#ifndef DEPLOYMENTPLANFRAMEWORK_EXPORT_H
#define DEPLOYMENTPLANFRAMEWORK_EXPORT_H

#if !defined (DEPLOYMENTPLANFRAMEWORK_HAS_DLL)
#  define DEPLOYMENTPLANFRAMEWORK_HAS_DLL 1
#endif /* ! DEPLOYMENTPLANFRAMEWORK_HAS_DLL */

#if defined (DEPLOYMENTPLANFRAMEWORK_HAS_DLL) && (DEPLOYMENTPLANFRAMEWORK_HAS_DLL == 1)
#  if defined (DEPLOYMENTPLANFRAMEWORK_BUILD_DLL)
#    define DeploymentPlanFramework_Export __declspec(dllexport)
#  else /* DEPLOYMENTPLANFRAMEWORK_BUILD_DLL */
#    define DeploymentPlanFramework_Export __declspec(dllimport)
#  endif /* DEPLOYMENTPLANFRAMEWORK_BUILD_DLL */
#else /* DEPLOYMENTPLANFRAMEWORK_HAS_DLL == 1 */
#  define DeploymentPlanFramework_Export
#endif /* DEPLOYMENTPLANFRAMEWORK_HAS_DLL == 1 */

#endif /* DEPLOYMENTPLANFRAMEWORK_EXPORT_H */

// End of auto generated file.
