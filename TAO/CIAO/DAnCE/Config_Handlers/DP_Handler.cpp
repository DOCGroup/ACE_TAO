#include "DP_Handler.h"
#include "ciao/Deployment_DataC.h"

#include "CCD_Handler.h"
#include "ADD_Handler.h"
#include "MDD_Handler.h"
#include "IDD_Handler.h"
#include "ID_Handler.h"
#include "cdp.hpp"

#include "DP_PCD_Handler.h"

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
    }

    ::Deployment::DeploymentPlan const *
    DP_Handler::plan (void) const
      throw (DP_Handler::NoPlan)
    {
      if (this->retval_)
        return this->idl_dp_.get ();

      throw NoPlan ();
    }

    ::Deployment::DeploymentPlan *
    DP_Handler::plan (void)
      throw (DP_Handler::NoPlan)
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

      // Read in the label, if present, since minoccurs = 0
      if (this->dp_.label_p ())
	{
	  this->idl_dp_->label =
	    CORBA::string_dup (this->dp_.label ().c_str ());
	}

      // Read in the UUID, if present
      if (this->dp_.UUID_p ())
	{
	  this->idl_dp_->UUID =
	    CORBA::string_dup (this->dp_.UUID ().c_str ());
	}


      // Similar thing for dependsOn
      for (DeploymentPlan::dependsOn_const_iterator dstart = this->dp_.begin_dependsOn ();
	   dstart != this->dp_.end_dependsOn ();
	   ++dstart)
	{
	  CORBA::ULong len =
	    this->idl_dp_->dependsOn.length ();

	  this->idl_dp_->dependsOn.length (len + 1);

	  ID_Handler::get_ImplementationDependency (
             this->idl_dp_->dependsOn [len],
             *dstart);

	}

      /* @@ Not needed at this time...

      // ... An the property stuff
      for (DeploymentPlan::infoProperty_const_iterator pstart = this->dp_.begin_infoProperty ();
	   pstart != this->dp_.end_infoProperty ();
	   ++pstart)
	{
	  CORBA::ULong len =
	    this-idl_dp_->infoProperty.length ();

	  this->idl_dp_->infoProperty.length (len + 1);

	  Property_Handler::get_property (
	    *pstart,
	    this->idl_dp_->infoProperty [len]);
	}
      */

      this->retval_ =
        CCD_Handler::component_interface_descr (
          this->dp_.realizes (),
          this->idl_dp_->realizes);

      if (!this->retval_)
        {
          ACE_DEBUG ((LM_ERROR,
                      "(%P|%t) DP_Handler: "
                      "Error parting Component Interface Descriptor."));
          return false;
        }


      this->retval_ =
        ADD_Handler::artifact_deployment_descrs (
          this->dp_,
          this->idl_dp_->artifact);

      if (!this->retval_)
        {
          ACE_DEBUG ((LM_ERROR,
                      "(%P|%t) DP_Handler: "
                      "Error parting Artifact Deployment Descriptior."));
          return false;
        }

      this->retval_ =
        MDD_Handler::mono_deployment_descriptions (
          this->dp_,
          this->idl_dp_->implementation);

      if (!this->retval_)
        {
          ACE_DEBUG ((LM_ERROR,
                      "(%P|%t) DP_Handler: "
                      "Error parting Monolithic Deployment Decriptions."));
          return false;
        }

      this->retval_ =
        IDD_Handler::instance_deployment_descrs (
          this->dp_,
          this->idl_dp_->instance);

      if (!this->retval_)
        {
          ACE_DEBUG ((LM_ERROR,
                      "(%P|%t) DP_Handler: "
                      "Error parting Instance Deployment Decriptions."));
          return false;
        }

      DP_PCD_Handler::plan_connection_descrs (this->dp_, this->idl_dp_->connection);

      return this->retval_;
    }

  }
}
