//==============================================================
/**
 *  @file  ADD_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_ADD_Handler_H
#define CIAO_CONFIG_HANDLERS_ADD_Handler_H
#include /**/ "ace/pre.h"

#include "Config_Handlers/Config_Handlers_Export.h"
#include "tao/Basic_Types.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "ace/Singleton.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace Deployment
{
  struct ArtifactDeploymentDescription;
  class ArtifactDeploymentDescriptions;
}


namespace CIAO
{

  namespace Config_Handlers
  {
    class DeploymentPlan;
    class ArtifactDeploymentDescription;

    /*
    * @class ADD_Handler
    *
    * @brief Handler class for <ArtifactDeploymentDescription> types.
    *
    * This class defines handler methods to map values from XSC
    * ArtifactDeploymentDescription objects, parsed from the
    *       // @@ Jules, why Any type?
    * descriptor files, to the  corresponding CORBA IDL Any type.
    */
    class Config_Handlers_Export ADD_Handler
    {
    public:
      static bool artifact_deployment_descrs (
          const DeploymentPlan &src,
          ::Deployment::ArtifactDeploymentDescriptions &dest);

      static ArtifactDeploymentDescription
      artifact_deployment_descr (
        const Deployment::ArtifactDeploymentDescription &src);

      
      static bool
      bind_ref (ACE_CString& id, size_t index);
  
      static bool
      find_ref (const ACE_CString& id, size_t val);
      
      static bool
      find_ref (const size_t id, ACE_CString& val);
  
      static bool
      unbind_refs (void);

      typedef ACE_Hash_Map_Manager<ACE_CString,
                                   size_t,
                                   ACE_Null_Mutex> IDREF_MAP;

          
      typedef ACE_Hash_Map_Manager<size_t,
                                   ACE_CString,
                                   ACE_Null_Mutex> POS_MAP;

    private:
      static bool artifact_deployment_descr (
          const ArtifactDeploymentDescription& desc,
          ::Deployment::ArtifactDeploymentDescription &dest,
          CORBA::ULong l = 0);

      /// The map used to store and look up the indexes of elements
      /// referenced by their IDREF.
      static IDREF_MAP idref_map_;
      /// The map used to store and look up the IDREFS of elements
      /// referenced by their index.
      static POS_MAP pos_map_;
    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_ADD_Handler_H */
