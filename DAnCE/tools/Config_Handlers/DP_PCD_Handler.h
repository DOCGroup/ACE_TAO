//==============================================================
/**
 *  @file  DP_PCD_Handler.h
 *
 *  $Id$
 *
 *  Handler for Deployment::PlanConnectionHandler
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_DP_PCD_HANDLER_H
#define CIAO_CONFIG_HANDLERS_DP_PCD_HANDLER_H
#include /**/ "ace/pre.h"

#include "Config_Handlers_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct PlanConnectionDescription;
  class PlanConnectionDescriptions;
}

namespace DAnCE
{

  namespace Config_Handlers
  {
    class DeploymentPlan;
    class PlanConnectionDescription;

   /*
    * @class DP_PCD_Handler
    *
    * @brief Handler class for <PlanConnectionDescription> types.
    *
    * This class defines handler methods to map values from
    * XSC PlanConnectionDescription objects, parsed from
    * the descriptor files, to the corresponding CORBA IDL type.
    *
    */

    class Config_Handlers_Export DP_PCD_Handler
    {
    public:
      static void plan_connection_descrs (
          const DeploymentPlan &src,
          Deployment::PlanConnectionDescriptions& toconfig);

      static PlanConnectionDescription plan_connection_descr(
      const Deployment::PlanConnectionDescription &src);

    private:
      static void plan_connection_descr (
          const PlanConnectionDescription& desc,
          Deployment::PlanConnectionDescription& toconfig);
    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_PCD_HANDLER_H*/
