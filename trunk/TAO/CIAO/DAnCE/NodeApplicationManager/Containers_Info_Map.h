/**
 * 
 * @file Containers_Info_Map.h
 * @author Gan Deng <dengg@dre.vanderbilt.edu>
 *
 * Map of ContainerImplementationInfo sequences.
 *
 * $Id$
 */

#ifndef CIAO_CONTAINERS_INFO_MAP_H
#define CIAO_CONTAINERS_INFO_MAP_H

#include /**/ "ace/pre.h"

#include "ace/Null_Mutex.h"
#include "ace/Hash_Map_Manager.h"
#include "ciao/DeploymentC.h"
#include "ace/SString.h"
#include "CIAO_NAM_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  /**
   * @class Container_Infos_Map
   * @brief Implementation of a map of ContainerImplementationInfo sequences.
   *
   * This is a helper class to populate the NodeImplementationInfo struct from
   * a deployment plan..
   */
  class CIAO_NAM_Export Containers_Info_Map
  {
  public:

    Containers_Info_Map (const Deployment::DeploymentPlan & plan,
                         CORBA::StringSeq shared_components);

    Deployment::ContainerImplementationInfos *
      containers_info (void);

  private:
    void initialize_map (void);
    bool build_map (void);

    bool insert_instance_into_map (
      const Deployment::InstanceDeploymentDescription & instance);

    bool insert_instance_into_container (
      const Deployment::InstanceDeploymentDescription & instance,
      Deployment::ComponentImplementationInfos & impl_infos);

    /// Helper function to check wheather a component instance
    /// is in the "shared components list".
    bool is_shared_component (ACE_CString & name);

    //Deployment::ContainerImplementationInfos_var containers_info_;

    typedef 
    ACE_Hash_Map_Manager_Ex <ACE_CString,
                            Deployment::ContainerImplementationInfo *,
                            ACE_Hash<ACE_CString>,
                            ACE_Equal_To<ACE_CString>,
                            ACE_Null_Mutex> MAP;

    typedef MAP::iterator Iterator;
    MAP map_;

    const Deployment::DeploymentPlan & plan_;

    /// shared components list, passed in from NodeImplementationInfoHandler
    /// class.
    CORBA::StringSeq shared_components_;
  };
}

#include /**/ "ace/post.h"

#endif /*CIAO_CONTAINERS_INFO_MAP_H*/
