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

#ifndef CIAO_REPOSITORY_MANAGER_UPDATE_PLAN_H
#define CIAO_REPOSITORY_MANAGER_UPDATE_PLAN_H
#include /**/ "ace/pre.h"

#include "ace/Hash_Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace Deployment
{
  struct SubcomponentInstantiationDescription;
  struct DeploymentPlan;
  struct ComponentAssemblyDescription;
  struct SubcomponentInstantiationDescription;
  struct AssemblyConnectionDescription;
}

namespace CIAO
{
  class Null_Dom_Document { };
  // exception thrown when we have a null dom document.

  typedef ACE_Hash_Map_Manager<ACE_TString, int, ACE_Null_Mutex> REF_MAP;
  typedef ACE_Hash_Map_Manager<int, int, ACE_Null_Mutex> ART_REF_MAP;

  void
  traverse_assembly_instance (Deployment::
                              SubcomponentInstantiationDescription &instance,
                              Deployment::DeploymentPlan &plan,
                              int l,
                              REF_MAP &ref_map,
                              REF_MAP &primary_ref_map,
                              REF_MAP &plan_ref_map);

  void traverse_assembly (Deployment::ComponentAssemblyDescription &assembly,
                          Deployment::DeploymentPlan &plan,
                          REF_MAP &ref_map, REF_MAP &primary_ref_map,
                          REF_MAP &plan_ref_map);

  void traverse_interface (Deployment::SubcomponentInstantiationDescription
                           &instance,
                           Deployment::PlanSubcomponentPortEndpoint
                           &pspe);

  void traverse_assembly_connection (Deployment::ComponentAssemblyDescription
                                     &assembly,
                                     Deployment::AssemblyConnectionDescription
                                     &assembly_connection,
                                     Deployment::DeploymentPlan &plan);

  void traverse_package (Deployment::PackageConfiguration* &pc,
                         Deployment::DeploymentPlan &plan,
                         REF_MAP &ref_map, REF_MAP &primary_ref_map);

  void update_artifacts (Deployment::MonolithicImplementationDescription &mid,
                         Deployment::DeploymentPlan &plan,
                         Deployment::InstanceDeploymentDescription &instance,
                         REF_MAP &ref_map, REF_MAP &primary_ref_map,
                         ART_REF_MAP &art_ref_map,
                         REF_MAP &plan_ref_map,
                         Deployment::MonolithicDeploymentDescription &mdd);

  void update_artifact_location (Deployment::ImplementationArtifactDescription
                                 &pack_iad,
                                 Deployment::ArtifactDeploymentDescription
                                 &plan_artifact);

  void update_artifact_property (Deployment::ImplementationArtifactDescription
                                 &pack_iad,
                                 Deployment::ArtifactDeploymentDescription
                                 &plan_artifact);

  void update_impl_art_ref (Deployment::MonolithicDeploymentDescription
                            &mid, int art_len);

  void update_impl_config_property (Deployment::
                                    PackagedComponentImplementation &impl,
                                    Deployment::
                                    MonolithicDeploymentDescription &mid,
                                    Deployment::
                                    InstanceDeploymentDescription
                                    &instance);

  void update_common_artifact_and_art_ref (Deployment::
                                           ImplementationArtifactDescription &pack_iad,
                                           REF_MAP &primary_ref_map,
                                           REF_MAP &ref_map,
                                           ART_REF_MAP &art_ref_map,
                                           REF_MAP &plan_ref_map,
                                           Deployment::MonolithicDeploymentDescription
                                           &mid,
                                           Deployment::DeploymentPlan &plan,
                                           Deployment::
                                           InstanceDeploymentDescription
                                           &instance);
}

#include /**/ "ace/post.h"
#endif /* CIAO_REPOSITORY_MANAGER_UPDATE_PLAN_H*/
