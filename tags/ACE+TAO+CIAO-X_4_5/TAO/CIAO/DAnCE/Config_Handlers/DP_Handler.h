//==============================================================
/**
 *  @file  DP_Handler.h
 *
 *  $Id$
 *
 *  @author Bala Natarajan <bala@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_DP_HANDLER_H
#define CIAO_CONFIG_HANDLERS_DP_HANDLER_H
#include /**/ "ace/pre.h"

#include "Config_Handlers_Export.h"
#include "ace/Auto_Ptr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace Deployment
{
  struct DeploymentPlan ;
}

namespace CIAO
{
  namespace Config_Handlers
  {
    class DeploymentPlan;

   /*
    * @class DP_Handler
    *
    * @brief Handler for Deployment::DeploymentPlan
    */
    class Config_Handlers_Export DP_Handler
    {
    public:
      class NoPlan {};

      DP_Handler (DeploymentPlan &dp);

      ~DP_Handler (void)
        throw ();

      ::Deployment::DeploymentPlan const *plan (void) const
        throw (NoPlan);

      ::Deployment::DeploymentPlan *plan (void)
        throw (NoPlan);

    private:

      bool resolve_plan (void);

    private:

      auto_ptr< ::Deployment::DeploymentPlan> idl_dp_;

      DeploymentPlan &dp_;

      bool retval_;
    };
  }
}

#include /**/ "ace/post.h"
#endif /*CIAO_CONFIG_HANDLERS_DP_HANDLER_H*/
