// $Id$
#include "Plan_Launcher_Impl.h"
#include "orbsvcs/CosNamingC.h"
#include "Config_Handlers/XML_File_Intf.h"

#include <iostream>
#include <string>

#define IPDSFIELD_DSCP_DEFAULT  0x00
#define IPDSFIELD_DSCP_CS1      0x08
#define IPDSFIELD_DSCP_CS2      0x10
#define IPDSFIELD_DSCP_CS3      0x18
#define IPDSFIELD_DSCP_CS4      0x20
#define IPDSFIELD_DSCP_CS5      0x28
#define IPDSFIELD_DSCP_CS6      0x30
#define IPDSFIELD_DSCP_CS7      0x38
#define IPDSFIELD_DSCP_AF11     0x0A
#define IPDSFIELD_DSCP_AF12     0x0C
#define IPDSFIELD_DSCP_AF13     0x0E
#define IPDSFIELD_DSCP_AF21     0x12
#define IPDSFIELD_DSCP_AF22     0x14
#define IPDSFIELD_DSCP_AF23     0x16
#define IPDSFIELD_DSCP_AF31     0x1A
#define IPDSFIELD_DSCP_AF32     0x1C
#define IPDSFIELD_DSCP_AF33     0x1E
#define IPDSFIELD_DSCP_AF41     0x22
#define IPDSFIELD_DSCP_AF42     0x24
#define IPDSFIELD_DSCP_AF43     0x26
#define IPDSFIELD_ECT_MASK      0x02
#define IPDSFIELD_CE_MASK       0x01
#define IPDSFIELD_DSCP_EF       0x2E

static int const dscp[] =
{
  IPDSFIELD_DSCP_DEFAULT ,
  IPDSFIELD_DSCP_CS1     ,
  IPDSFIELD_DSCP_CS2     ,
  IPDSFIELD_DSCP_CS3     ,
  IPDSFIELD_DSCP_CS4     ,
  IPDSFIELD_DSCP_CS5     ,
  IPDSFIELD_DSCP_CS6     ,
  IPDSFIELD_DSCP_CS7     ,
  IPDSFIELD_DSCP_AF11    ,
  IPDSFIELD_DSCP_AF12    ,
  IPDSFIELD_DSCP_AF13    ,
  IPDSFIELD_DSCP_AF21    ,
  IPDSFIELD_DSCP_AF22    ,
  IPDSFIELD_DSCP_AF23    ,
  IPDSFIELD_DSCP_AF31    ,
  IPDSFIELD_DSCP_AF32    ,
  IPDSFIELD_DSCP_AF33    ,
  IPDSFIELD_DSCP_AF41    ,
  IPDSFIELD_DSCP_AF42    ,
  IPDSFIELD_DSCP_AF43    ,
  IPDSFIELD_DSCP_EF
};

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

      // *******************************************************

      // update the plan with the server resources here

      Deployment::DeploymentPlan temp_plan = plan.in ();

      Deployment::DiffservInfos dscp_infos;
      Deployment::DiffservInfo dscp_info;

      int request_array_slot = 3;
      int reply_array_slot = 2;

      dscp_info.server_instance_name =
        CORBA::string_dup ("Hello-Sender-idd");
      dscp_info.client_instance_name =
        CORBA::string_dup ("Hello-Receiver-idd");
      dscp_info.client_receptacle_name =
        CORBA::string_dup ("read_message");
      dscp_info.request_dscp = dscp[request_array_slot];
      dscp_info.reply_dscp = dscp[reply_array_slot];

      CORBA::ULong dscp_infos_len = dscp_infos.length ();
      CORBA::ULong new_dscp_infos_len;
      new_dscp_infos_len = dscp_infos_len + 1;
      dscp_infos.length (new_dscp_infos_len);
      dscp_infos[dscp_infos_len] = dscp_info;

      CORBA::ULong len = dscp_infos.length ();
      CIAO::DAnCE::ServerResource server_resource;
      server_resource.Id =
        CORBA::string_dup ("test_server_resource_id");

      for (CORBA::ULong i = 0; i < len; ++i)
        {
          const char *server_instance_name =
             dscp_infos[i].server_instance_name.in ();
          const char *client_instance_name =
             dscp_infos[i].client_instance_name.in ();
          const char *client_receptacle_name =
             dscp_infos[i].client_receptacle_name.in ();
          CORBA::Long request_dscp = dscp_infos[i].request_dscp;
          CORBA::Long reply_dscp = dscp_infos[i].reply_dscp;

          CORBA::ULong instance_len = temp_plan.instance.length ();
          for (CORBA::ULong j = 0; j < instance_len; ++j)
            {
              const char *instance_name =
                temp_plan.instance[j].name.in ();
              this->instance_map_.bind (instance_name, j);
            }

          CORBA::ULong policy_set_len;
          CORBA::ULong new_policy_set_len;
          CORBA::ULong policy_def_len;
          CORBA::ULong new_policy_def_len;
          CORBA::ULong dep_res_len;
          CORBA::ULong new_dep_res_len;
          CORBA::ULong pro_len;
          CORBA::ULong new_pro_len;
          ACE_CString policy_set_id;
          ACE_CString temp_policy_set_id;

          int server_instance_iter;
          if (this->instance_map_.find
                 (server_instance_name, server_instance_iter) == 0)
            {
              policy_set_len =
                server_resource.orb_config.policy_set.length ();
              new_policy_set_len = policy_set_len + 1;
              server_resource.orb_config.policy_set.length (
                 new_policy_set_len);
              policy_set_id = "test_policy_set_id";
              policy_set_id += "_";
              policy_set_id += server_instance_name;
              server_resource.orb_config.policy_set[policy_set_len].Id =
                CORBA::string_dup (policy_set_id.c_str ());

              policy_def_len = server_resource.orb_config.
                                 policy_set[policy_set_len].
                                   policies.length ();
              new_policy_def_len = policy_def_len + 1;
              server_resource.orb_config.policy_set[policy_set_len].
                policies.length (new_policy_def_len);

              ::CIAO::DAnCE::NWPriorityModelPolicyDef npmd;
              npmd.nw_priority_model = 
                ::CIAO::DAnCE::CLIENT_PROPAGATED_NWPRIORITY;
              npmd.request_dscp = 0;
              npmd.reply_dscp = 0;

              server_resource.orb_config.policy_set[policy_set_len].
                policies[policy_def_len].NWPriorityModelDef (npmd);

              dep_res_len = 
                temp_plan.instance[server_instance_iter].
                  deployedResource.length ();
              new_dep_res_len = dep_res_len + 1;
              temp_plan.instance[server_instance_iter].
                 deployedResource.length (new_dep_res_len);
              temp_plan.instance[server_instance_iter].
                deployedResource[dep_res_len].resourceUsage =
                   Deployment::InstanceUsesResource;
              temp_plan.instance[server_instance_iter].
                deployedResource[dep_res_len].requirementName =
                  CORBA::string_dup ("CIAO:PolicySet");
              temp_plan.instance[server_instance_iter].
                deployedResource[dep_res_len].resourceName =
                  CORBA::string_dup ("test_server_resource_id");
              pro_len = temp_plan.instance[server_instance_iter].
                deployedResource[dep_res_len].property.length ();
              new_pro_len = pro_len + 1;
              temp_plan.instance[server_instance_iter].
                deployedResource[dep_res_len].property.length (
                  new_pro_len);
              temp_plan.instance[server_instance_iter].
                deployedResource[dep_res_len].property[pro_len].
                   name = CORBA::string_dup ("CIAO:InstancePolicy");
              temp_plan.instance[server_instance_iter].
                deployedResource[dep_res_len].property[pro_len].
                   value <<= policy_set_id.c_str ();
            }

          int client_instance_iter;
          if (this->instance_map_.find
                 (client_instance_name, client_instance_iter) == 0)
            {
              policy_set_len =
                server_resource.orb_config.policy_set.length ();
              new_policy_set_len = policy_set_len + 1;
              server_resource.orb_config.policy_set.length (
                 new_policy_set_len);

              policy_set_id = "test_policy_set_id";
              policy_set_id += "_";
              policy_set_id += client_instance_name;
              temp_policy_set_id = "test_policy_set_id";
              temp_policy_set_id += "_";
              temp_policy_set_id += server_instance_name;

              server_resource.orb_config.policy_set[policy_set_len].Id =
                CORBA::string_dup (policy_set_id.c_str ());

              policy_def_len = server_resource.orb_config.
                                 policy_set[policy_set_len].
                                   policies.length ();
              new_policy_def_len = policy_def_len + 1;
              server_resource.orb_config.policy_set[policy_set_len].
                policies.length (new_policy_def_len);

              ::CIAO::DAnCE::CNWPriorityModelPolicyDef cnpmd;
              cnpmd.request_dscp = request_dscp;
              cnpmd.reply_dscp = reply_dscp;

              server_resource.orb_config.policy_set[policy_set_len].
                policies[policy_def_len].CNWPriorityModelDef (cnpmd);

              dep_res_len = 
                temp_plan.instance[client_instance_iter].
                  deployedResource.length ();
              new_dep_res_len = dep_res_len + 1;
              temp_plan.instance[client_instance_iter].
                 deployedResource.length (new_dep_res_len);
              temp_plan.instance[client_instance_iter].
                deployedResource[dep_res_len].resourceUsage =
                   Deployment::InstanceUsesResource;
              temp_plan.instance[client_instance_iter].
                deployedResource[dep_res_len].requirementName =
                  CORBA::string_dup ("CIAO:PolicySet");
              temp_plan.instance[client_instance_iter].
                deployedResource[dep_res_len].resourceName =
                  CORBA::string_dup ("test_server_resource_id");

              pro_len = temp_plan.instance[client_instance_iter].
                deployedResource[dep_res_len].property.length ();
              new_pro_len = pro_len + 1;
              temp_plan.instance[client_instance_iter].
                deployedResource[dep_res_len].property.length (
                  new_pro_len);
              temp_plan.instance[client_instance_iter].
                deployedResource[dep_res_len].property[pro_len].
                   name = CORBA::string_dup ("CIAO:ReceptaclePolicy");
              temp_plan.instance[client_instance_iter].
                deployedResource[dep_res_len].property[pro_len].
                   value <<= policy_set_id.c_str ();

              pro_len = temp_plan.instance[client_instance_iter].
                deployedResource[dep_res_len].property.length ();
              new_pro_len = pro_len + 1;
              temp_plan.instance[client_instance_iter].
                deployedResource[dep_res_len].property.length (
                  new_pro_len);
              temp_plan.instance[client_instance_iter].
                deployedResource[dep_res_len].property[pro_len].
                   name = client_receptacle_name;
              temp_plan.instance[client_instance_iter].
                deployedResource[dep_res_len].property[pro_len].
                   value <<= policy_set_id.c_str ();
            }
        }

      CORBA::ULong new_info_prop_len;
      CORBA::ULong info_prop_len = temp_plan.infoProperty.length ();
      new_info_prop_len = info_prop_len + 1;
      temp_plan.infoProperty.length (new_info_prop_len);
      temp_plan.infoProperty[0].name =
         CORBA::string_dup ("CIAOServerResources");
      temp_plan.infoProperty[0].value <<= server_resource;

      // ******************************************************
      
      // return this->launch_plan (plan.in ());
      return this->launch_plan (temp_plan);
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
