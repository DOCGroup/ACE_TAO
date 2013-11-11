//==============================================================
/**
*  @file  IRDD_Handler.h
*
*  $Id$
*
*  @author Jules White <jules@dre.vanderbilt.edu>
*/
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_IRDD_Handler_H
#define CIAO_CONFIG_HANDLERS_IRDD_Handler_H
#include /**/ "ace/pre.h"

#include "Config_Handlers_Export.h"
#include "Common.h"
#include "ace/config-lite.h"
#include "Utils/Functors.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct InstanceResourceDeploymentDescription;
  class InstanceResourceDeploymentDescriptions;
}

namespace DAnCE
{
  namespace Config_Handlers
    {
      class InstanceResourceDeploymentDescription;

      /*
       * @class IRDD_Handler
       *
       * @brief Handler class for <InstanceResourceDeploymentDescription> types.
       *
       * This class defines handler methods to map values from
       * XSC InstanceResourceDeploymentDescription objects, parsed from the descriptor files, to the
       * corresponding CORBA IDL Any type.
       *
       */
      class Config_Handlers_Export IRDD_Handler {
      public:
    IRDD_Handler (void);
    virtual ~IRDD_Handler (void);

    static void handle_irdd (const InstanceResourceDeploymentDescription& desc,
                            Deployment::InstanceResourceDeploymentDescription& toconfig)
      /*throw (Config_Error)*/;

    static InstanceResourceDeploymentDescription
      instance_resource_deployment_descr (const Deployment::InstanceResourceDeploymentDescription& src)
      /*throw (Config_Error)*/;
      };

      typedef Sequence_Handler < InstanceResourceDeploymentDescription,
                 ::Deployment::InstanceResourceDeploymentDescriptions,
                 ::Deployment::InstanceResourceDeploymentDescription,
                 IRDD_Handler::handle_irdd > IRDD_Functor;
    }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_IRDD_Handler_H */

