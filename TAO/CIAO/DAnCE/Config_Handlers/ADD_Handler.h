
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
#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */



namespace Deployment
{
  class ArtifactDeploymentDescription;
}


namespace CIAO
{

  namespace Config_Handlers
  {

   class ArtifactDeploymentDescription;


   /*
    * @class ADD_Handler
    *
    * @brief Handler class for <ArtifactDeploymentDescription> types.
    *
    * This class defines handler methods to map values from
    * XSC ArtifactDeploymentDescription objects, parsed from the descriptor files, to the
    * corresponding CORBA IDL Any type.
    *
    */
    
    class Config_Handlers_Export ADD_Handler {
     
      public:

        ADD_Handler (void);
        virtual ~ADD_Handler (void);

        static void artifact_deployment_descr (
             const ArtifactDeploymentDescription& desc,
             Deployment::ArtifactDeploymentDescription& toconfig);

    };
  }
}

#include /**/ "ace/post.h" 
#endif /* CIAO_CONFIG_HANDLERS_ADD_Handler_H */

