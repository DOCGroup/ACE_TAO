#include "DP_Handler.h"
#include "ciao/Deployment_DataC.h"

#include "CCD_Handler.h"
#include "ADD_Handler.h"
#include "MDD_Handler.h"
#include "IDD_Handler.h"
#include "cdp.hpp"
#include "Singleton_IDREF_Map.h"

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
      , retval_ (false)
    {
      if (!this->resolve_plan ())
        throw;
    }

    DP_Handler::~DP_Handler (void)
      throw ()
    {
      (void) Singleton_IDREF_Map::instance ()->unbind_refs ();
    }

    ::Deployment::DeploymentPlan const *
    DP_Handler::plan (void) const
      throw (NoPlan)
    {
      if (this->retval_)
        return this->idl_dp_.get ();

      throw NoPlan ();
    }

    ::Deployment::DeploymentPlan *
    DP_Handler::plan (void)
      throw (NoPlan)
    {
      if (this->retval_)
        return this->idl_dp_.release ();

      throw NoPlan ();
    }

    bool
    DP_Handler::resolve_plan (void)
    {
      ::Deployment::DeploymentPlan *tmp =
          new Deployment::DeploymentPlan;

      this->idl_dp_.reset (tmp);

      this->retval_ =
        CCD_Handler::component_interface_descr (
          this->dp_.realizes (),
          this->idl_dp_->realizes);

      if (!this->retval_)
        return this->retval_;

      this->retval_ =
        ADD_Handler::artifact_deployment_descrs (
          this->dp_,
          this->idl_dp_->artifact);

      if (!this->retval_)
        return this->retval_;

      this->retval_ =
        MDD_Handler::mono_deployment_descriptions (
          this->dp_.implementation (),
          this->idl_dp_->implementation);

      if (!this->retval_)
        return this->retval_;

      this->retval_ =
        IDD_Handler::instance_deployment_descrs (
          this->dp_,
          this->idl_dp_->instance);

      if (!this->retval_)
        return this->retval_;

      return this->retval_;
    }

  }
}
