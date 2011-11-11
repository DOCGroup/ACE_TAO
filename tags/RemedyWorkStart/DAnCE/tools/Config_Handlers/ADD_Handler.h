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

#include "Config_Handlers_Export.h"
#include "tao/Basic_Types.h"
#include "IDREF_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace Deployment
{
  struct ArtifactDeploymentDescription;
  class ArtifactDeploymentDescriptions;
}


namespace DAnCE
{

  namespace Config_Handlers
    {
      class deploymentPlan;
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
          static void artifact_deployment_descrs (const deploymentPlan &src,
                                                  ::Deployment::ArtifactDeploymentDescriptions &dest);

          static ArtifactDeploymentDescription
            artifact_deployment_descr (const Deployment::ArtifactDeploymentDescription &src);

          ///  The IDREF Table associated with this class
          static IDREF_Base<CORBA::ULong> IDREF;

        private:
          static void artifact_deployment_descr (
                                                 const ArtifactDeploymentDescription& desc,
                                                 ::Deployment::ArtifactDeploymentDescription &dest,
                                                 CORBA::ULong l = 0);
        };
    }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_ADD_Handler_H */
