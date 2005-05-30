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
      :   xsc_dp_ (0)
        , idl_dp_ (0)
        , retval_ (false)
    {
      if (!this->resolve_plan (dp))
        throw;
    }
    
    DP_Handler::DP_Handler (const ::Deployment::DeploymentPlan &plan)
      : xsc_dp_ (0),
        idl_dp_ (0),
        retval_ (0)
    {
      if (!this->build_xsc (plan))
        throw;
    }
    
    DP_Handler::~DP_Handler (void)
      throw ()
    {
    }
    
    DeploymentPlan const *
    DP_Handler::xsc (void) const
      throw (DP_Handler::NoPlan)
    {
      if (this->retval_ && this->xsc_dp_.get () != 0)
        return this->xsc_dp_.get ();
      
      throw NoPlan ();
    }
    
    DeploymentPlan *
    DP_Handler::xsc (void)
      throw (DP_Handler::NoPlan)
    {
      if (this->retval_ && this->xsc_dp_.get () != 0)
        return this->xsc_dp_.release ();
      
      throw NoPlan ();
    }
    
    ::Deployment::DeploymentPlan const *
    DP_Handler::plan (void) const
      throw (DP_Handler::NoPlan)
    {
      if (this->retval_ && this->idl_dp_.get () != 0)
        return this->idl_dp_.get ();

      throw NoPlan ();
    }

    ::Deployment::DeploymentPlan *
    DP_Handler::plan (void)
      throw (DP_Handler::NoPlan)
    {
      if (this->retval_ && this->idl_dp_.get () != 0)
        return this->idl_dp_.release ();

      throw NoPlan ();
    }

    bool
    DP_Handler::resolve_plan (DeploymentPlan &xsc_dp)
    {
      ::Deployment::DeploymentPlan *tmp =
          new Deployment::DeploymentPlan;

      this->idl_dp_.reset (tmp);

      // Read in the label, if present, since minoccurs = 0
      if (xsc_dp.label_p ())
	      {
	        this->idl_dp_->label =
	          CORBA::string_dup (xsc_dp.label ().c_str ());
	      }

      // Read in the UUID, if present
      if (xsc_dp.UUID_p ())
	      {
	        this->idl_dp_->UUID =
	          CORBA::string_dup (xsc_dp.UUID ().c_str ());
	      }


      // Similar thing for dependsOn
      for (DeploymentPlan::dependsOn_const_iterator dstart = xsc_dp.begin_dependsOn ();
	         dstart != xsc_dp.end_dependsOn ();
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
      for (DeploymentPlan::infoProperty_const_iterator pstart = xsc_dp.begin_infoProperty ();
	   pstart != xsc_dp.end_infoProperty ();
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

      // Read in the realizes, if present
      if (xsc_dp.realizes_p ())
      {
        this->retval_ =
          CCD_Handler::component_interface_descr (
            xsc_dp.realizes (),
            this->idl_dp_->realizes);

        if (!this->retval_)
          {
            ACE_DEBUG ((LM_ERROR,
                        "(%P|%t) DP_Handler: "
                        "Error parsing Component Interface Descriptor."));
            return false;
          }
      }


      this->retval_ =
        ADD_Handler::artifact_deployment_descrs (
          xsc_dp,
          this->idl_dp_->artifact);

      if (!this->retval_)
        {
          ACE_DEBUG ((LM_ERROR,
                      "(%P|%t) DP_Handler: "
                      "Error parsing Artifact Deployment Descriptior."));
          return false;
        }

      this->retval_ =
        MDD_Handler::mono_deployment_descriptions (
          xsc_dp,
          this->idl_dp_->implementation);

      if (!this->retval_)
        {
          ACE_DEBUG ((LM_ERROR,
                      "(%P|%t) DP_Handler: "
                      "Error parsing Monolithic Deployment Decriptions."));
          return false;
        }

      this->retval_ =
        IDD_Handler::instance_deployment_descrs (
          xsc_dp,
          this->idl_dp_->instance);

      if (!this->retval_)
        {
          ACE_DEBUG ((LM_ERROR,
                      "(%P|%t) DP_Handler: "
                      "Error parsing Instance Deployment Decriptions."));
          return false;
        }

      DP_PCD_Handler::plan_connection_descrs (xsc_dp, this->idl_dp_->connection);

      return this->retval_;
    }
    
    bool
    DP_Handler::build_xsc (const ::Deployment::DeploymentPlan &plan)
    {
      // @@Lucas: Fill in the implementation here. 
    }
    

  }
}
