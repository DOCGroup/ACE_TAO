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

namespace ciao
{
  namespace config_handlers
  {
    dp_handler::dp_handler (deploymentplan &dp)
      :   xsc_dp_ (0)
      , idl_dp_ (0)
      , retval_ (true)
    {
      if (!this->resolve_plan (dp))
        throw;
    }

    dp_handler::dp_handler (const ::deployment::deploymentplan &plan)
      : xsc_dp_ (new deploymentplan),
        idl_dp_ (0),
        retval_ (0)
    {
      if (!this->build_xsc (plan))
        throw;
    }

    dp_handler::~dp_handler (void)
      throw ()
    {
    }

    deploymentplan const *
    dp_handler::xsc (void) const
    {
      if (this->retval_ && this->xsc_dp_.get () != 0)
        return this->xsc_dp_.get ();

      throw noplan ();
    }

    deploymentplan *
    dp_handler::xsc (void)
    {
      if (this->retval_ && this->xsc_dp_.get () != 0)
        return this->xsc_dp_.release ();

      throw noplan ();
    }

    ::deployment::deploymentplan const *
    dp_handler::plan (void) const
    {
      if (this->retval_ && this->idl_dp_.get () != 0)
        return this->idl_dp_.get ();

      throw noplan ();
    }

    ::deployment::deploymentplan *
    dp_handler::plan (void)
    {
      if (this->retval_ && this->idl_dp_.get () != 0)
        return this->idl_dp_.release ();

      throw noplan ();
    }

    bool
    dp_handler::resolve_plan (deploymentplan &xsc_dp)
    {
      dance_trace ("dp_handler::resolve_plan");

      ::deployment::deploymentplan *tmp =
          new deployment::deploymentplan;

      this->idl_dp_.reset (tmp);

      // read in the label, if present, since minoccurs = 0
      if (xsc_dp.label_p ())
        {
          this->idl_dp_->label =
            corba::string_dup (ace_text_always_char (xsc_dp.label ().c_str ()));
        }

      // read in the uuid, if present
      if (xsc_dp.uuid_p ())
        {
          this->idl_dp_->uuid =
            corba::string_dup (ace_text_always_char  (xsc_dp.uuid ().c_str ()));
        }

      // similar thing for dependson
      for (deploymentplan::dependson_const_iterator dstart = xsc_dp.begin_dependson ();
            dstart != xsc_dp.end_dependson ();
            ++dstart)
        {
          corba::ulong len = this->idl_dp_->dependson.length ();
          this->idl_dp_->dependson.length (len + 1);
          id_handler::get_implementationdependency (*(*dstart),
                                                    this->idl_dp_->dependson [len]);

        }

      // ... an the property stuff
      for (deploymentplan::infoproperty_const_iterator pstart = xsc_dp.begin_infoproperty ();
            pstart != xsc_dp.end_infoproperty ();
            ++pstart)
        {
          corba::ulong len =
            this->idl_dp_->infoproperty.length ();

          this->idl_dp_->infoproperty.length (len + 1);

          property_handler::handle_property (*(*pstart),
                                              this->idl_dp_->infoproperty [len]);
        }

      // read in the realizes, if present
      if (xsc_dp.realizes_p ())
        {
          ccd_handler::component_interface_descr (
                                                  xsc_dp.realizes (),
                                                  this->idl_dp_->realizes);
        }

      add_handler::artifact_deployment_descrs (xsc_dp,
                                                this->idl_dp_->artifact);

      mdd_handler::mono_deployment_descriptions (xsc_dp,
                                                  this->idl_dp_->implementation);

      idd_handler::instance_deployment_descrs (xsc_dp,
                                                this->idl_dp_->instance);

      this->idl_dp_->connection.length (xsc_dp.count_connection ());
      std::for_each (xsc_dp.begin_connection (),
                      xsc_dp.end_connection (),
                      pcd_functor (this->idl_dp_->connection));

      this->idl_dp_->localityconstraint.length (xsc_dp.count_localityconstraint ());

      std::for_each (xsc_dp.begin_localityconstraint (),
                      xsc_dp.end_localityconstraint (),
                      pl_functor (this->idl_dp_->localityconstraint));

      //pcd_handler::get_planconnectiondescription (xsc_dp, this->idl_dp_->connection);

      return true;
    }

    bool
    dp_handler::build_xsc (const ::deployment::deploymentplan &plan)
    {
      dance_trace ("dp_handler::build_xsc");

      // initialize the uuid generator.
      ace_utils::uuid_generator::instance ()->init ();

      // clear idref tables
      idd_handler::idref.unbind_refs ();
      mdd_handler::idref.unbind_refs ();
      add_handler::idref.unbind_refs ();


      size_t len; //used for checking the length of struct data members

      // read in the label, if present, since minoccurs = 0
      if (plan.label != 0)
        {
          xmlschema::string< ace_tchar > i(ace_text_char_to_tchar (plan.label));
          this->xsc_dp_->label(i);
        }

      // read in the uuid, if present
      if (plan.uuid != 0)
        {
          xmlschema::string< ace_tchar > j(ace_text_char_to_tchar(plan.uuid));
          this->xsc_dp_->uuid(j);
        }

      // similar thing for dependson
      len = plan.dependson.length();
      for (size_t j = 0;
            j < len;
            ++j)
        {
          //this->xsc_dp_->add_dependson(id_handler::impl_dependency(plan.dependson[j]));
        }

      // ... and the property stuff
      len = plan.infoproperty.length();
      for (size_t q = 0;
            q < len;
            q++)
        {
          if (ace_os::strcmp (plan.infoproperty[q].name.in (),
                              "ciaoserverresources") == 0)
            {
              dance_debug (1, (lm_error,
                          "(%p|%t) dp_handler: dumping of serverresources not currently supported."));
              continue;
            }

          //this->xsc_dp_->add_infoproperty (property_handler::get_property (plan.infoproperty[q]));
        }


      // we are assuming there is a realizes for the moment
      // @@ we may want to change this at a later date by creating a sequence of
      // @@ componentinterfacedescriptions in the deploymentplan in ../dance/deployment/deployment_data.idl
      // @@ so we can check for length
      this->xsc_dp_->realizes(ccd_handler::component_interface_descr(plan.realizes));
      if (!this->xsc_dp_->realizes_p())
        {
          dance_debug (1, (lm_error,
                      "(%p|%t) dp_handler: "
                      "error parsing component interface descriptor."));
          return false;
        }

      //take care of the artifact(s) if they exist
      len = plan.artifact.length();
      for(size_t k = 0;
          k < len;
          k++)
        {
          //this->xsc_dp_->add_artifact (add_handler::artifact_deployment_descr (plan.artifact[k]));
        }

      //take care of the implementation(s) if they exist
      len = plan.implementation.length();
      for(size_t l = 0;
          l < len;
          l++)
        {
          //this->xsc_dp_->add_implementation (mdd_handler::mono_deployment_description (plan.implementation[l]));
        }

      //ditto for the instance(s)
      len = plan.instance.length();
      for(size_t m = 0;
          m < len;
          m++)
        {
          //this->xsc_dp_->add_instance (idd_handler::instance_deployment_descr (plan.instance[m]));
        }

      //finally, take care of the connection planning
      len = plan.connection.length();
      for(size_t n = 0; n < len; n++)
        {
          //this->xsc_dp_->add_connection (pcd_handler::get_planconnectiondescription (plan.connection[n]));
        }

      retval_ = true;
      return true;
    }
  }
}
