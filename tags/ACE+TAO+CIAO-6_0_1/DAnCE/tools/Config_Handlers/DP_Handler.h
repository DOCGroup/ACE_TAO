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
#include <memory>
using std::auto_ptr;


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace Deployment
{
  struct DeploymentPlan ;
}

namespace DAnCE
{
  namespace Config_Handlers
    {
      class deploymentPlan;

      /*
       * @class DP_Handler
       *
       * @brief Handler for Deployment::DeploymentPlan
       */
      class Config_Handlers_Export DP_Handler
        {
        public:
          class NoPlan {};

          /// Use this constructor if you want to use
          /// the XSC->IDL mapping
          DP_Handler (deploymentPlan &dp);

          /// Use this constructor if you want to use
          /// the IDL->XSC mapping
          DP_Handler (const ::Deployment::DeploymentPlan &plan);

          ~DP_Handler (void)
            throw ();

          /// Generates the IDL->XSC mapping.  returns
          /// null if the method fails or if the class was
          /// constructed with the wrong source.
          deploymentPlan const *xsc (void) const;

          deploymentPlan *xsc (void);

          /// Generates the XSC->IDL mapping.  returns null
          /// if the method fails or the class was constructed
          ::Deployment::DeploymentPlan const *plan (void) const;

          ::Deployment::DeploymentPlan *plan (void);

        private:

          bool resolve_plan (deploymentPlan &dp);

          bool build_xsc (const ::Deployment::DeploymentPlan &plan);

        private:
          auto_ptr< deploymentPlan > xsc_dp_;

          auto_ptr< ::Deployment::DeploymentPlan> idl_dp_;

          bool retval_;
        };
    }
}

#include /**/ "ace/post.h"
#endif /*CIAO_CONFIG_HANDLERS_DP_HANDLER_H*/
