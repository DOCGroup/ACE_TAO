// $Id$
#include "Plan_Launcher_Impl.h"
#include "orbsvcs/CosNamingC.h"
#include "Config_Handlers/XML_File_Intf.h"

#include <iostream>
#include <string>

namespace CIAO
{
  namespace Plan_Launcher
  {

    static CORBA::Object_ptr
    fetch_reference_naming (CORBA::ORB_ptr orb,
                            bool use_repoman = 0, 
                            const char *repoman_name = 0
                            ACE_ENV_ARG_DECL)
    {
      CORBA::Object_var tmp =
        orb->resolve_initial_references ("NameService"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CosNaming::NamingContext_var pns =
        CosNaming::NamingContext::_narrow (tmp.in ()
                                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CosNaming::Name name (1);
      name.length (1);
      
      if (!use_repoman)
        {
          name[0].id = CORBA::string_dup ("ExecutionManager");
        }
      else
        {
          name[0].id = CORBA::string_dup (repoman_name);
        }

      return pns->resolve (name
                           ACE_ENV_ARG_PARAMETER);
    }

    Plan_Launcher_i::Plan_Launcher_i ()
      : em_ (), rm_ ()
    {
    }

    bool
    Plan_Launcher_i::init (const char *em_ior,
                           CORBA::ORB_ptr orb,
                           bool use_repoman,
                           bool rm_use_naming,
                           const char *rm_ior
                           ACE_ENV_ARG_DECL)
    {
      CORBA::Object_var obj;

      // EM
      if (em_ior == 0)
        {
          obj = fetch_reference_naming (orb ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
      else
        {
          obj = orb->string_to_object (em_ior
                                       ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      this->em_ = ::CIAO::ExecutionManagerDaemon::_narrow (obj.in ()
                                                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (CORBA::is_nil (this->em_.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) CIAO_PlanLauncher: nil Execution"
                      " Manager reference, narrow failed\n"));
          return false;
        }

      if (CIAO::debug_level () > 9)
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) CIAO_PlanLauncher: Obtained Execution"
                    " Manager ref \n"));

      // RM
      if (use_repoman)
        {
          if (rm_use_naming)
            {
              obj = fetch_reference_naming (orb, use_repoman, rm_ior ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
          else
            {
              obj = orb->string_to_object (rm_ior
                                          ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }

          this->rm_ = Deployment::RepositoryManager::_narrow (obj.in ()
                                                              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          if (CORBA::is_nil (this->rm_.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          "(%P|%t) CIAO_PlanLauncher: nil Repository"
                          " Manager reference, narrow failed\n"));
              return false;
            }

          if (CIAO::debug_level () > 9)
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) CIAO_PlanLauncher: Obtained Repository"
                        " Manager ref \n"));
        }

      return true;
    }


    const char *
    Plan_Launcher_i::launch_plan (const char *deployment_plan_uri,
                                  const char *package_uri,
                                  bool use_package_name,
                                  bool use_repoman 
                                  ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((Plan_Launcher_i::Deployment_Failure))
    {
      ACE_DEBUG ((LM_DEBUG, "Parsing plan...\n"));

      CIAO::Config_Handlers::XML_File_Intf intf (deployment_plan_uri);

      ::Deployment::DeploymentPlan_var plan =
          intf.get_plan ();

      if (use_repoman)
      {
        ::Deployment::PackageConfiguration_var pc;

        ACE_TCHAR package[PACKAGE_NAME_LEN];

        size_t length = ACE_OS::strlen (package_uri);
        
        size_t pos1 = 0;
        size_t pos2 = ACE_OS::strcspn (package_uri + pos1, "+");

        while (pos1 < length)
        {
          ACE_OS::strsncpy (package, package_uri + pos1, pos2 + 1);
          
          if (use_package_name)
          {
            pc = rm_->findPackageByName (package);
            
            PCVisitor pcv (plan, *pc);
            pcv.Visit ();
          }
          else
          {
            CORBA::StringSeq_var seq = rm_->findNamesByType (package);

            for (size_t i = 0; i < seq->length (); ++i)
            {
              pc = rm_->findPackageByName (seq[i]);
            
              PCVisitor pcv (plan, *pc);
              pcv.Visit ();
            }
          }

          pos1 += pos2 + 1;
          pos2 = ACE_OS::strcspn (package_uri + pos1, "+");
        }
      }

      ACE_DEBUG ((LM_DEBUG, "Parsing complete....\n"));
      
      return this->launch_plan (plan.in ());
    }

    const char *
    Plan_Launcher_i::launch_plan (const ::Deployment::DeploymentPlan &plan ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((Plan_Launcher_i::Deployment_Failure))
    {
      ACE_TRY
        {
	  if (CORBA::is_nil (this->em_.in ()))
	    {
	      ACE_ERROR ((LM_ERROR,
			  ACE_TEXT ("CIAO::Plan_Launcher_i: ")
			  ACE_TEXT ("launch_plan called witn an uninitialized EM.\n")));
	      return 0;
	    }

	  ACE_ERROR ((LM_ERROR,
		      ACE_TEXT ("CIAO::Plan_Launcher_i: " )
		      ACE_TEXT ("about to call this->em_->preparePlan\n")));
	  ::Deployment::DomainApplicationManager_var dam (this->em_->preparePlan (plan, 1));
	  ACE_ERROR ((LM_ERROR,
		      ACE_TEXT ("CIAO::Plan_Launcher_i: " )
		      ACE_TEXT ("after to call this->em_->preparePlan\n")));

	  if (CORBA::is_nil (dam.in ()))
	    {
	      ACE_DEBUG ((LM_DEBUG,
			  "(%P|%t) CIAO_PlanLauncher:preparePlan call failed: "
			  "nil DomainApplicationManager reference\n"));
	      return 0;
	    }

	  if (CIAO::debug_level () > 9)
	    ACE_DEBUG ((LM_DEBUG,
			"CIAO_PlanLauncher: Obtained DAM ref \n"));

	  ::Deployment::Properties_var properties;
	  ACE_NEW_RETURN (properties,
			  Deployment::Properties,
			  0);

	  if (CIAO::debug_level ())
	    ACE_DEBUG ((LM_DEBUG,
			"CIAO_PlanLauncher: start Launch application...\n"));

	  // Dont not start the Application immediately since it vialtes
	  // the semantics of component activation sequence
	  int start = 0;

          dam->startLaunch (properties.in (), 0);

          if (CIAO::debug_level ())
            ACE_DEBUG ((LM_DEBUG, "[success]\n"));

          // Call finish Launch to complete the connections
          if (CIAO::debug_level ())
            ACE_DEBUG ((LM_DEBUG,
                        "CIAO_PlanLauncher: finish Launch application...\n"));
          dam->finishLaunch (start, false); // is_ReDAC by default is <false>

          if (CIAO::debug_level ())
            ACE_DEBUG ((LM_DEBUG, "[success]\n"));

          // Call start to activate components
          if (CIAO::debug_level ())
            ACE_DEBUG ((LM_DEBUG,
                        "CIAO_PlanLauncher: start activating components...\n"));
          dam->start ();

          if (CIAO::debug_level ())
            ACE_DEBUG ((LM_DEBUG, "[success]\n"));

          if (CIAO::debug_level ())
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("CIAO_PlanLauncher: ")
                        ACE_TEXT ("Application Deployed successfully\n")));

          map_.bind_dam_reference (plan.UUID.in (),
                                   Deployment::DomainApplicationManager::_duplicate (dam.in ()));
        }
      ACE_CATCH (Deployment::ResourceNotAvailable, ex)
        {
          ACE_ERROR ((LM_ERROR,
                      "EXCEPTION: ResourceNotAvaiable exception caught: %s,\n"
                      "Type: %s\n"
                      "Property: %s\n"
                      "Element: %s\n"
                      "Resource: %s\n",
                      ex.name.in (),
                      ex.resourceType.in (),
                      ex.propertyName.in (),
                      ex.elementName.in (),
                      ex.resourceName.in ()));
          ACE_THROW (Deployment_Failure (""));
        }
      ACE_CATCH (Deployment::StartError, ex)
        {
          ACE_ERROR ((LM_ERROR,
                      "EXCEPTION: StartError exception caught: %s, %s\n",
                      ex.name.in (),
                      ex.reason.in ()));
          ACE_THROW (Deployment_Failure (""));
        }
      ACE_CATCH (Deployment::InvalidProperty, ex)
        {
          ACE_ERROR ((LM_ERROR,
                      "EXCEPTION: InvalidProperty exception caught: %s, %s\n",
                      ex.name.in (),
                      ex.reason.in ()));
          ACE_THROW (Deployment_Failure (""));
        }
      ACE_CATCH (Deployment::InvalidConnection, ex)
        {
          ACE_ERROR ((LM_ERROR,
                      "EXCEPTION: InvalidConnection exception caught: %s, %s\n",
                      ex.name.in (),
                      ex.reason.in ()));
          ACE_THROW (Deployment_Failure  (""));
        }
      ACE_CATCHANY
        {
          ACE_ERROR ((LM_ERROR,
                      "CORBA EXCEPTION: %s\n",
                      ACE_ANY_EXCEPTION._info().fast_rep()));
          ACE_THROW (Deployment_Failure  (""));
        }
      ACE_CATCHALL
        {
          ACE_ERROR ((LM_ERROR,
                      "EXCEPTION: non-CORBA exception\n"));
          ACE_THROW (Deployment_Failure  (""));
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (0);

      std::string * retv = new std::string (plan.UUID.in ());

      return (*retv).c_str ();
    }

    ::Deployment::DomainApplicationManager_ptr
    Plan_Launcher_i::get_dam (const char *uuid ACE_ENV_ARG_DECL)
    {
      if (!this->map_.is_plan_available (uuid))
        {
          // Note: we could do an exhaustive (and expensive!) search of the DAMS
          // on our EM at this point, if we wanted.  In most cases, though, this
          // would not produce a different result.
          return Deployment::DomainApplicationManager::_nil ();
        }
      return this->map_.fetch_dam_reference (uuid);
    }

    bool
    Plan_Launcher_i::teardown_plan (const char *uuid)
    {
      // Since people could always run another instance of the Plan_Launcher
      // executable to tear down a plan, so we could NOT rely on the local
      // DAM_Map to fetch DAM obj reference. Instead, we make a remote call
      // on ExecutionManager to fetch it.
      ACE_TRY
        {
          ::Deployment::DomainApplicationManager_var dapp_mgr =
              this->em_->getManager (uuid);

          if (!::CORBA::is_nil (dapp_mgr))
          {
            dapp_mgr->destroyApplication ();
            if (CIAO::debug_level ())
              ACE_DEBUG ((LM_DEBUG, "[success]\n"));

            // Note that we should ask the DAM to tell EM whether the DAM should
            // be destroyed
            this->destroy_dam_by_plan (uuid);
          }
        }
      ACE_CATCHANY
        {
          ACE_DEBUG ((LM_ERROR, "Unable to find DomainApplicationManager "
                      "for plan with uuid: %s\n", uuid));
          return false;
        }
      ACE_ENDTRY;

      return true;
    }

    bool
    Plan_Launcher_i::teardown_plan (::Deployment::DomainApplicationManager_ptr dam
                                    ACE_ENV_ARG_DECL)
    {
      if (CIAO::debug_level ())
        ACE_DEBUG ((LM_DEBUG,
                    "CIAO_PlanLauncher: destroy the application.....\n"));

      dam->destroyApplication ();

      if (CIAO::debug_level ())
        ACE_DEBUG ((LM_DEBUG, "[success]\n"));

      this->destroy_dam (dam);

      return true;
    }

    void
    Plan_Launcher_i::destroy_dam (::Deployment::DomainApplicationManager_ptr dam
                                  ACE_ENV_ARG_DECL)
    {
      if (CIAO::debug_level ())
        ACE_DEBUG ((LM_DEBUG,
                    "CIAO_PlanLauncher: destroy the manager.....\n"));

      this->em_->destroyManager (dam);

      if (CIAO::debug_level ())
        ACE_DEBUG ((LM_DEBUG, "[success]\n"));
    }

    void
    Plan_Launcher_i::destroy_dam_by_plan (const char* plan_uuid
                                          ACE_ENV_ARG_DECL)
    {
      if (CIAO::debug_level ())
        ACE_DEBUG ((LM_DEBUG,
                    "CIAO_PlanLauncher: destroy the manager.....\n"));

      this->em_->destroyManagerByPlan (plan_uuid);

      if (CIAO::debug_level ())
        ACE_DEBUG ((LM_DEBUG, "[success]\n"));
    }

    const char *
    Plan_Launcher_i::re_launch_plan (const char *plan_uri ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((Plan_Launcher_i::Deployment_Failure))
    {
      CIAO::Config_Handlers::XML_File_Intf intf (plan_uri);

      ::Deployment::DeploymentPlan_var plan =
          intf.get_plan ();

      return this->re_launch_plan (plan.in ());
    }

    const char *
    Plan_Launcher_i::re_launch_plan (const ::Deployment::DeploymentPlan &plan ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((Plan_Launcher_i::Deployment_Failure))
    {

      if (CORBA::is_nil (this->em_.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("CIAO::Plan_Launcher_i: ")
                      ACE_TEXT ("re_launch_plan called witn an uninitialized EM.\n")));
          return 0;
        }

      this->em_->perform_redeployment (plan);

      if (CIAO::debug_level ())
        ACE_DEBUG ((LM_DEBUG,
                    "CIAO_PlanLauncher: new plan redeployed ...\n"));

      std::string * retv = new std::string (plan.UUID.in ());

      return (*retv).c_str ();
    }
  }
}
