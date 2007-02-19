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

//added for the HTTP support
#include "ace/Message_Block.h"      //for ACE_Message_Block

#include "DAnCE/Deployment/Deployment_ContainerC.h"
#include "DAnCE/Deployment/DeploymentC.h"
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
            const Deployment::ComponentPlans & shared_components);

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
    Deployment::ComponentPlans shared_components_;

    /**-------------------------------------------------------------------
     * HTTP capability
     * @author Stoyan Paunov
     *
     * Purpose: Adding the HTTP access code which will resove
     * any references to HTTP URLs
     */
  protected:

    // @Stoyan - Please follow ACE coding guideines when naming your
    //member variables.  In particular, please use lower case and
    //follow the name with a trailing underscore.
    //directory in which to download the libraries obtained via HTTP
    ACE_CString HTTP_DOWNLOAD_PATH;

    /// This function checks if the HTTP_DOWNLOAD_PATH is
    /// in the library load path
    void
    update_loader_path (void);

    /// This function resolves any http location references
    /// in the name of the implementation artifacts
    /// It returns true on success and false on failure
    bool
    resolve_http_reference (const char* location,
                            ACE_CString &arti_name,
                            ACE_CString &name);

    /// function to retvieve a file via HTTP
    /// stores the file in the passed preallocated ACE_Message_Block
    /// returns 1 on success
    ///         0 on error
    bool
    retrieve_via_HTTP (const char* URL,
                       ACE_Message_Block &mb);

    /// function that writes out a file to a specified location on the hand disk
    /// returns 1 on success
    ///         0 on already exists and replace == false
    ///         0 on error

    bool
    write_to_disk (const char* full_path,
                   ACE_Message_Block& mb,
                   bool replace = true);

    /// TODO: Fuction to remove the downloaded library to ensure
    ///       consisterncy in future runs

  };
}

#include /**/ "ace/post.h"

#endif /*CIAO_CONTAINERS_INFO_MAP_H*/
