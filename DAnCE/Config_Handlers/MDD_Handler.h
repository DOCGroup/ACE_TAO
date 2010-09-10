//==============================================================
/**
 *  @file  MDD_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_MDD_Handler_H
#define CIAO_CONFIG_HANDLERS_MDD_Handler_H
#include /**/ "ace/pre.h"
#include "tao/Basic_Types.h"
#include "Config_Handlers_Export.h"
#include "IDREF_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace Deployment
{
  struct MonolithicDeploymentDescription;
  class MonolithicDeploymentDescriptions;
}


namespace DAnCE
{

  namespace Config_Handlers
    {
      class MonolithicDeploymentDescription;
      class deploymentPlan;

      /*
       * @class MDD_Handler
       *
       * @brief Handler class for <MonolithicDeploymentDescription> types.
       *
       * This class defines handler methods to map values from XSC
       * MonolithicDeploymentDescription objects, parsed from the
       * descriptor files, to the corresponding CORBA IDL types.
       *
       */

      class Config_Handlers_Export MDD_Handler
        {
        public:
          static void mono_deployment_descriptions (const deploymentPlan &src,
                                                    Deployment::MonolithicDeploymentDescriptions &dest);

          static MonolithicDeploymentDescription mono_deployment_description(const Deployment::MonolithicDeploymentDescription &src);

          static IDREF_Base<CORBA::ULong> IDREF;

        private:
          static void mono_deployment_description (const MonolithicDeploymentDescription& desc,
                                                   Deployment::MonolithicDeploymentDescription& toconfig,
                                                   CORBA::ULong pos);
        };
    }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_MDD_Handler_H */
