// -*- C++ -*-
/**
 *  @file
 *
 *  @brief   C++ source file for classes that implement the
 *           eventtype(s) from web_common.idl.
 *
 *  See web_common_impl.h for class API.
 *
 *  $Id$
 *
 *  @author Edward Mulholland <emulholl@atl.lmco.com>
 */

#include "web_common_impl.h"
#include "Controller/Component/InterfaceC.h"
#include "tools/Config_Handlers/Common.h"
#include "tools/Config_Handlers/XML_File_Intf.h"
#include "tools/Config_Handlers/Package_Handlers/PCD_Handler.h"
#include "DAnCE/Plan_Launcher/Plan_Launcher_Impl.h"
#include "tao/ORB_Core.h"
#include "orbsvcs/CosNamingC.h"
#include "Controller/Component/ControllerC.h"

namespace OBV_CIAO
{
  namespace RACE
  {
    Web_Deployment_impl::Web_Deployment_impl (const char * plan_uri,
                                              const char * pcd_uri)
      : plan_launcher_i_ (0)
    {
      if (plan_uri == 0)
        plan_uri = "";
      this->plan_uri (plan_uri);

      // Deserialize plan from URI
      ::Deployment::DeploymentPlan_var plan_var;
      ::Deployment::PackageConfiguration_var pcd_var
          (new ::Deployment::PackageConfiguration);

      ACE_TRY_NEW_ENV
        {
          CIAO::Config_Handlers::XML_File_Intf intf (plan_uri);
          plan_var = intf.get_plan ();
        }
      ACE_CATCH (CIAO::Config_Handlers::Config_Error, ex)
        {
          ACE_ERROR ((LM_ERROR, "(%P|%t) Caught exception while parsing"
                      " deployment plan %s: \"%s\" -- %s\n",
                      plan_uri,
                      ex.name_.c_str (),
                      ex.error_.c_str ()));
        }
      ACE_CATCHANY
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Caught exception while parsing"
                      " deployment plan %s: %s\n",
                      plan_uri,
                      ACE_ANY_EXCEPTION._info().fast_rep()));
        }
      ACE_CATCHALL
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Caught exception while parsing"
                      " deployment plan %s.\n",
                      plan_uri));
        }
      ACE_ENDTRY;

      if (plan_var.ptr () == 0)
      {
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) XML_File_Intf::get_plan(\"%s\") returned null.\n",
                    plan_uri));
      }
      else
        this->plan (plan_var.in ());

      ACE_TRY_NEW_ENV
        {
          ::CIAO::Config_Handlers::Packaging::PCD_Handler::package_config
            (pcd_uri, pcd_var.inout ());
        }
      ACE_CATCH (CIAO::Config_Handlers::Config_Error, ex)
        {
          ACE_ERROR ((LM_ERROR, "(%P|%t) Caught exception while parsing"
                      " package handler %s: \"%s\" -- %s\n",
                      pcd_uri,
                      ex.name_.c_str (),
                      ex.error_.c_str ()));
        }
      ACE_CATCHANY
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Caught exception while parsing"
                      " package handler %s: %s\n",
                      pcd_uri,
                      ACE_ANY_EXCEPTION._info().fast_rep()));
        }
      ACE_CATCHALL
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Caught exception while parsing"
                      " package handler %s.\n",
                      pcd_uri));
        }
      ACE_ENDTRY;

      this->pcd (pcd_var.in ());
    }

    Web_Deployment_impl::Web_Deployment_impl (const Deployment::DeploymentPlan & plan,
                                              const Deployment::PackageConfiguration & pcd)
      : plan_launcher_i_ (0)
    {
      this->plan (plan);
      this->pcd (pcd);
    }

    Web_Deployment_impl::~Web_Deployment_impl ()
    {

    }

    ::Deployment::DomainApplicationManager_ptr
    Web_Deployment_impl::launch_plan (
         ACE_ENV_SINGLE_ARG_DECL_NOT_USED
      )
      ACE_THROW_SPEC ((
        ::CORBA::SystemException
      ))
    {
      const char * uuid = 0;

      ACE_TRY_NEW_ENV
        {
          if (this->plan_launcher_i_ == 0)
          {
            this->plan_launcher_i_ =
              new ::CIAO::Plan_Launcher::Plan_Launcher_i;
            this->plan_launcher_i_->init
              (0, // 0 here means to find EM in Naming Service
               TAO_ORB_Core_instance ()->orb ());
          }
          uuid = this->plan_launcher_i_->launch_plan (this->plan ());

	}
      ACE_CATCHANY
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Caught exception from "
                      "Plan_Launcher_i::launch_plan: %s\n",
                      ACE_ANY_EXCEPTION._info().fast_rep()));
        }
      ACE_CATCHALL
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Caught exception from "
                      "Plan_Launcher_i::launch_plan.\n"));
        }
      ACE_ENDTRY;

      ACE_TRY_NEW_ENV
        {

	  CORBA::Object_var tmp =
	    TAO_ORB_Core_instance ()->orb ()->
	    resolve_initial_references ("NameService"
					ACE_ENV_ARG_PARAMETER);
	  ACE_CHECK;

	  CosNaming::NamingContext_var pns =
	    CosNaming::NamingContext::_narrow (tmp.in ()
					       ACE_ENV_ARG_PARAMETER);
	  ACE_CHECK;

	  CosNaming::Name name (1);
	  name.length (1);
	  name[0].id =
	    CORBA::string_dup ("Controller");

	  CORBA::Object_var cont_tmp (pns->resolve (name));

	  ::CIAO::RACE::Controller_var cont_comp (::CIAO::RACE::Controller::_narrow (cont_tmp.in ()));


          ::CIAO::RACE::Descriptors_var controller =
	      cont_comp->provide_descriptors ();

	      //    ::CIAO::RACE::Descriptors::_narrow (this->controller ());
          if (CORBA::is_nil (controller.in ()))
          {
            ACE_ERROR ((LM_ERROR,
                        "(%P|%t) Controller is nil "
                        "in the Web_Deployment valuetype.\n"));
          }
          else {
            controller->register_string (this->pcd (), this->plan ());
            ACE_ERROR ((LM_ERROR,
                        "(%P|%t) controller->push_descriptors "
                        "succeeded in the Web_Deployment valuetype.\n"));
          }
        }
      ACE_CATCHANY
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Caught exception from "
                      "controller->push_descriptors: %s\n",
                      ACE_ANY_EXCEPTION._info().fast_rep()));
        }
      ACE_CATCHALL
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Caught exception from "
                      "controller->push_descriptors.\n"));
        }
      ACE_ENDTRY;

      if (uuid == 0)
      {
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) Plan_Launcher_i::launch_plan returned nil.\n"));
        return ::Deployment::DomainApplicationManager::_nil ();
      }
      else
      {
        ::Deployment::DomainApplicationManager_var dam_var
            (this->plan_launcher_i_->get_dam (uuid));
        this->dam (dam_var.in ());
        return this->dam ();
      }
    }

    ::CORBA::Boolean
    Web_Deployment_impl::teardown_plan (
         ACE_ENV_SINGLE_ARG_DECL_NOT_USED
      )
      ACE_THROW_SPEC ((
        ::CORBA::SystemException
      ))
    {
      if (this->plan_launcher_i_ == 0 ||
          CORBA::is_nil (this->dam ()))
      {
        ACE_DEBUG ((LM_ERROR,
                    "(%P|%t) Cannot teardown plan:\n"
                    "\t this->plan_launcher_i_ == %d\n"
                    "\t this->dam () == %d\n",
                    this->plan_launcher_i_,
                    this->dam ()));
        return false;
      }

      return
        this->plan_launcher_i_->teardown_plan (this->dam ());
    }

    Web_Deployment_impl::Web_Deployment_impl ()
      : plan_launcher_i_ (0)
    {
      // Do nothing
    }

    Web_Deployment_Factory::~Web_Deployment_Factory ()
    {
      // Do nothing
    }

    TAO_OBV_CREATE_RETURN_TYPE (Web_Deployment) Web_Deployment_Factory::create_for_unmarshal
    (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    {
      return new Web_Deployment_impl;
    }
  }
}
