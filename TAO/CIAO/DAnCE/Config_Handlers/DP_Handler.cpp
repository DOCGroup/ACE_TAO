#include "DP_Handler.h"
#include "ciao/Deployment_DataC.h"
#include "ace/Auto_Ptr.h"
#include "CCD_Handler.h"
#include "cdp.hpp"

ACE_RCSID (Config_Handlers,
           DP_Handler,
           "$Id$")

namespace CIAO
{
  namespace Config_Handlers
  {
    DP_Handler::DP_Handler (DeploymentPlan &dp)
      : idl_dp_ (0)
      , dp_ (dp)
    {
      if (!this->resolve_plan ())
        throw;
    }

    DP_Handler::~DP_Handler (void)
    {
      delete this->idl_dp_;
    }

    bool
    DP_Handler::resolve_plan (void)
    {
      ::Deployment::DeploymentPlan *tmp =
          new Deployment::DeploymentPlan;

      auto_ptr< ::Deployment::DeploymentPlan>
        auto_idl_dp (tmp);

      bool retval =
        CCD_Handler::component_interface_descr ((*auto_idl_dp).realizes,
                                                this->dp_.realizes ());

      if (!retval)
        return retval;


      this->idl_dp_ =
        auto_idl_dp.release ();

      return true;
    }

  }
}
