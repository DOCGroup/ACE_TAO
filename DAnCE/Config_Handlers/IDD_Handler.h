//==============================================================
/**
 *  @file  IDD_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_IDD_HANDLER_H
#define CIAO_CONFIG_HANDLERS_IDD_HANDLER_H
#include /**/ "ace/pre.h"

#include "Common.h"
#include "tao/Basic_Types.h"
#include "Config_Handlers_Export.h"
#include "IDREF_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct InstanceDeploymentDescription;
  class InstanceDeploymentDescriptions;
  struct InstanceResourceDeploymentDescription;
}

namespace DAnCE
{
  namespace Config_Handlers
    {
      class deploymentPlan;
      class InstanceDeploymentDescription;
      class InstanceResourceDeploymentDescription;

      /*
       * @class IDD_Handler
       *
       * @brief Handler class for <InstanceDeploymentDescription> types.
       *
       * This class defines handler methods to map values from
       * XSC InstanceDeploymentDescriptionn objects, parsed from
       * the descriptor files, to the corresponding CORBA IDL type.
       *
       */

      class Config_Handlers_Export IDD_Handler
        {
        public:
          static void instance_deployment_descrs (const deploymentPlan &src,
                                                  ::Deployment::InstanceDeploymentDescriptions& dest);

          static InstanceDeploymentDescription instance_deployment_descr (const Deployment::InstanceDeploymentDescription &src);

          static IDREF_Base<CORBA::ULong> IDREF;

        private:
          static void instance_deployment_descr (const InstanceDeploymentDescription &src,
                                                 ::Deployment::InstanceDeploymentDescription &dest,
                                                 CORBA::ULong pos);

        };
    }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_IDD_HANDLER_H*/
