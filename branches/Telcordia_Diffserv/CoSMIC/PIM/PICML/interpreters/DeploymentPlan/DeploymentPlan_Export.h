// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s Package
// ------------------------------
#ifndef DEPLOYMENTPLAN_EXPORT_H
#define DEPLOYMENTPLAN_EXPORT_H

#if !defined (DEPLOYMENTPLAN_HAS_DLL)
#  define DEPLOYMENTPLAN_HAS_DLL 1
#endif /* ! DEPLOYMENTPLAN_HAS_DLL */

#if defined (DEPLOYMENTPLAN_HAS_DLL) && (DEPLOYMENTPLAN_HAS_DLL == 1)
#  if defined (DEPLOYMENTPLAN_BUILD_DLL)
#    define DeploymentPlan_Export __declspec(dllexport)
#  else /* DEPLOYMENTPLAN_BUILD_DLL */
#    define DeploymentPlan_Export __declspec(dllimport)
#  endif /* DEPLOYMENTPLAN_BUILD_DLL */
#else /* DEPLOYMENTPLAN_HAS_DLL == 1 */
#  define DeploymentPlan_Export
#endif /* DEPLOYMENTPLAN_HAS_DLL == 1 */

#endif /* DEPLOYMENTPLAN_EXPORT_H */

// End of auto generated file.