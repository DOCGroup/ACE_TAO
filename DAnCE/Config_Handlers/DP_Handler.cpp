// $Id$

#include "DP_Handler.h"
#include "ace/UUID.h"
#include "DAnCE/Deployment/Deployment_DataC.h"

#include "CCD_Handler.h"
#include "ADD_Handler.h"
#include "MDD_Handler.h"
#include "IDD_Handler.h"
#include "ID_Handler.h"
#include "PL_Handler.h"
#include "Property_Handler.h"
#include "cdp.hpp"

#include "PCD_Handler.h"

namespace DAnCE
{
  namespace Config_Handlers
  {
    DP_Handler::DP_Handler (deploymentPlan &dp)
      :   xsc_dp_ (0)
      , idl_dp_ (0)
      , retval_ (true)
    {
      if (!this->resolve_plan (dp))
        throw;
    }

    DP_Handler::DP_Handler (const ::Deployment::DeploymentPlan &plan)
      : xsc_dp_ (new deploymentPlan),
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

    deploymentPlan const *
    DP_Handler::xsc (void) const
    {
      if (this->retval_ && this->xsc_dp_.get () != 0)
        return this->xsc_dp_.get ();

      throw NoPlan ();
    }

    deploymentPlan *
    DP_Handler::xsc (void)
    {
      if (this->retval_ && this->xsc_dp_.get () != 0)
        return this->xsc_dp_.release ();

      throw NoPlan ();
    }

    ::Deployment::DeploymentPlan const *
    DP_Handler::plan (void) const
    {
      if (this->retval_ && this->idl_dp_.get () != 0)
        return this->idl_dp_.get ();

      throw NoPlan ();
    }

    ::Deployment::DeploymentPlan *
    DP_Handler::plan (void)
    {
      if (this->retval_ && this->idl_dp_.get () != 0)
        return this->idl_dp_.release ();

      throw NoPlan ();
    }

    bool
    DP_Handler::resolve_plan (deploymentPlan &xsc_dp)
    {
      DANCE_TRACE ("DP_Handler::resolve_plan");

      ::Deployment::DeploymentPlan *tmp =
          new Deployment::DeploymentPlan;

      this->idl_dp_.reset (tmp);

      // Read in the label, if present, since minoccurs = 0
      if (xsc_dp.label_p ())
        {
          this->idl_dp_->label =
            CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (xsc_dp.label ().c_str ()));
        }

      // Read in the UUID, if present
      if (xsc_dp.UUID_p ())
        {
          this->idl_dp_->UUID =
            CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR  (xsc_dp.UUID ().c_str ()));
        }

      // Similar thing for dependsOn
      for (deploymentPlan::dependsOn_const_iterator dstart = xsc_dp.begin_dependsOn ();
            dstart != xsc_dp.end_dependsOn ();
            ++dstart)
        {
          CORBA::ULong len = this->idl_dp_->dependsOn.length ();
          this->idl_dp_->dependsOn.length (len + 1);
          ID_Handler::get_ImplementationDependency (*(*dstart),
                                                    this->idl_dp_->dependsOn [len]);

        }

      // ... An the property stuff
      for (deploymentPlan::infoProperty_const_iterator pstart = xsc_dp.begin_infoProperty ();
            pstart != xsc_dp.end_infoProperty ();
            ++pstart)
        {
          CORBA::ULong len =
            this->idl_dp_->infoProperty.length ();

          this->idl_dp_->infoProperty.length (len + 1);

          Property_Handler::handle_property (*(*pstart),
                                              this->idl_dp_->infoProperty [len]);
        }

      // Read in the realizes, if present
      if (xsc_dp.realizes_p ())
        {
          CCD_Handler::component_interface_descr (
                                                  xsc_dp.realizes (),
                                                  this->idl_dp_->realizes);
        }

      ADD_Handler::artifact_deployment_descrs (xsc_dp,
                                                this->idl_dp_->artifact);

      MDD_Handler::mono_deployment_descriptions (xsc_dp,
                                                  this->idl_dp_->implementation);

      IDD_Handler::instance_deployment_descrs (xsc_dp,
                                                this->idl_dp_->instance);

      this->idl_dp_->connection.length (xsc_dp.count_connection ());
      std::for_each (xsc_dp.begin_connection (),
                      xsc_dp.end_connection (),
                      PCD_Functor (this->idl_dp_->connection));

      this->idl_dp_->localityConstraint.length (xsc_dp.count_localityConstraint ());

      std::for_each (xsc_dp.begin_localityConstraint (),
                      xsc_dp.end_localityConstraint (),
                      PL_Functor (this->idl_dp_->localityConstraint));

      //PCD_Handler::get_PlanConnectionDescription (xsc_dp, this->idl_dp_->connection);

      return true;
    }

    bool
    DP_Handler::build_xsc (const ::Deployment::DeploymentPlan &plan)
    {
      DANCE_TRACE ("DP_Handler::build_xsc");

      // Initialize the UUID generator.
      ACE_Utils::UUID_GENERATOR::instance ()->init ();

      // Clear IDREF tables
      IDD_Handler::IDREF.unbind_refs ();
      MDD_Handler::IDREF.unbind_refs ();
      ADD_Handler::IDREF.unbind_refs ();


      size_t len; //Used for checking the length of struct data members

      // Read in the label, if present, since minoccurs = 0
      if (plan.label != 0)
        {
          XMLSchema::string< ACE_TCHAR > i(ACE_TEXT_CHAR_TO_TCHAR (plan.label));
          this->xsc_dp_->label(i);
        }

      // Read in the UUID, if present
      if (plan.UUID != 0)
        {
          XMLSchema::string< ACE_TCHAR > j(ACE_TEXT_CHAR_TO_TCHAR(plan.UUID));
          this->xsc_dp_->UUID(j);
        }

      // Similar thing for dependsOn
      len = plan.dependsOn.length();
      for (size_t j = 0;
            j < len;
            ++j)
        {
          //this->xsc_dp_->add_dependsOn(ID_Handler::impl_dependency(plan.dependsOn[j]));
        }

      // ... And the property stuff
      len = plan.infoProperty.length();
      for (size_t q = 0;
            q < len;
            q++)
        {
          if (ACE_OS::strcmp (plan.infoProperty[q].name.in (),
                              "CIAOServerResources") == 0)
            {
              DANCE_DEBUG (1, (LM_ERROR,
                          "(%P|%t) DP_Handler: Dumping of ServerResources not currently supported."));
              continue;
            }

          //this->xsc_dp_->add_infoProperty (Property_Handler::get_property (plan.infoProperty[q]));
        }


      // We are assuming there is a realizes for the moment
      // @@ We may want to change this at a later date by creating a sequence of
      // @@ ComponentInterfaceDescriptions in the DeploymentPlan in ../DAnCE/Deployment/Deployment_Data.idl
      // @@ so we can check for length
      this->xsc_dp_->realizes(CCD_Handler::component_interface_descr(plan.realizes));
      if (!this->xsc_dp_->realizes_p())
        {
          DANCE_DEBUG (1, (LM_ERROR,
                      "(%P|%t) DP_Handler: "
                      "Error parsing Component Interface Descriptor."));
          return false;
        }

      //Take care of the artifact(s) if they exist
      len = plan.artifact.length();
      for(size_t k = 0;
          k < len;
          k++)
        {
          //this->xsc_dp_->add_artifact (ADD_Handler::artifact_deployment_descr (plan.artifact[k]));
        }

      //Take care of the implementation(s) if they exist
      len = plan.implementation.length();
      for(size_t l = 0;
          l < len;
          l++)
        {
          //this->xsc_dp_->add_implementation (MDD_Handler::mono_deployment_description (plan.implementation[l]));
        }

      //Ditto for the instance(s)
      len = plan.instance.length();
      for(size_t m = 0;
          m < len;
          m++)
        {
          //this->xsc_dp_->add_instance (IDD_Handler::instance_deployment_descr (plan.instance[m]));
        }

      //Finally, take care of the Connection Planning
      len = plan.connection.length();
      for(size_t n = 0; n < len; n++)
        {
          //this->xsc_dp_->add_connection (PCD_Handler::get_PlanConnectionDescription (plan.connection[n]));
        }

      retval_ = true;
      return true;
    }
  }
}
