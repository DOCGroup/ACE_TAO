/*=======================================================================
 *
 * @file Update_Plan.h
 *
 * $Id$
 *
 * @brief This file contains declarations for helper functions
 * used in updating the deployment plan data structure from
 * PackageConfiguration data structure.
 *
 * @author Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 *
 *======================================================================*/

#ifndef UPDATE_PLAN_H
#define UPDATE_PLAN_H

#include "DeploymentC.h"
#include "ace/Hash_Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Null_Dom_Document { };
// exception thrown when we have a null dom document.

typedef ACE_Hash_Map_Manager<ACE_TString, int, ACE_Null_Mutex> REF_MAP;
typedef ACE_Hash_Map_Manager<int, int, ACE_Null_Mutex> ART_REF_MAP;

void traverse_instance (Deployment::SubcomponentInstantiationDescription
                        &instance,
                        Deployment::DeploymentPlan &plan, int l,
                        REF_MAP &ref_map, REF_MAP &primary_ref_map);

void traverse_interface (Deployment::SubcomponentInstantiationDescription
                         &instance,
                         Deployment::PlanSubcomponentPortEndpoint
                         &pspe);

void traverse_connection (Deployment::ComponentAssemblyDescription
                          &assembly,
                          Deployment::AssemblyConnectionDescription
                          &assembly_connection,
                          Deployment::DeploymentPlan &plan);

void traverse_package (Deployment::PackageConfiguration* &pc,
                       Deployment::DeploymentPlan &plan,
                       REF_MAP &ref_map, REF_MAP &primary_ref_map);

#include /**/ "ace/post.h"
#endif /* UPDATE_PLAN_H */
