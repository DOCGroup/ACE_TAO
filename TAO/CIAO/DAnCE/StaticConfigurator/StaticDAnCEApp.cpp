// $Id$

#include "Container_Base.h"
#include "NodeApplication_Impl.h"
#include "DeploymentC.h"
#include "CIAO/DAnCE/NodeApplicationManager/ImplementationInfo.h"
//#include "CIAO/DAnCE/RepositoryManager/RepositoryManager_Impl.h"
//#include "CIAO/DAnCE/RepositoryManager/Update_Plan.h"
#include "CIAO/DAnCE/DomainApplicationManager/DomainApplicationManager_Impl.h"
#include "CIAO/DAnCE/NodeManager/NodeDaemon_Impl.h"
//#include "Config_Handlers/DnC_Dump.h"
//#include "orbsvcs/orbsvcs/Naming/Transient_Naming_Context.h"
#include "ace/OS_NS_stdio.h"
#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "tao/IORTable/IORTable.h"
#include "tao/CDR.h"
#include "plan.h"


const char * exec_ior = "file://exec_mgr.ior";
const char * node_daemon_ior = 0;
const char * init_file = "deployment.dat";

static void
usage (const ACE_TCHAR* program)
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("Usage: %s -p <URI> -d <URI> -k <EX_IOR> \
                         -t <NODE_MANAGER_IOR> \n")
              ACE_TEXT (" <URI>: URI identifying the package\n"),
              program));
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // top level package URL
  char* package_url = 0;

  // deployment plan URL
  char* plan_url = 0;

  // Initialize the ORB so that CORBA::Any will work
  //
  CORBA::ORB_var orb =
    CORBA::ORB_init (argc,
                     argv,
                     "");
  try
    {
      /*
      Deployment::DeploymentPlan plan;

      TAO_InputCDR in_cdr(plan_cdr_buffer, sizeof(plan_cdr_buffer)-1 );

      in_cdr >> plan;
      */
      //ACE_DEBUG ((LM_DEBUG, "Dumping deployment plan===start\n"));
      //Deployment::DnC_Dump::dump (plan);
      //ACE_DEBUG ((LM_DEBUG, "Dumping deployment plan===end\n"));

      // Pass the parsed plan to the Execution Manager to start the
      // Deployment Process.

      // Get reference to Root POA.
      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA"
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (obj.in ()
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Activate POA manager
      PortableServer::POAManager_var mgr
        = poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      mgr->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int homes_table_size =
        sizeof (homes_table)/sizeof(HomeAttributes);

      CIAO::HOMECREATOR_FUNCPTR_MAP home_creator_fptr_map;
      CIAO::HOMESERVANTCREATOR_FUNCPTR_MAP homesvnt_creator_fptr_map;
      CIAO::Static_Config_EntryPoints_Maps static_config_entrypoints_maps;
      static_config_entrypoints_maps.home_creator_funcptr_map_ = &home_creator_fptr_map;
      static_config_entrypoints_maps.home_servant_creator_funcptr_map_ = &homesvnt_creator_fptr_map;

      int i=0;
      for (i=0; i<homes_table_size; ++i)
        {
          home_creator_fptr_map.bind (homes_table[i].executor_entrypt_,
                                      homes_table[i].executor_fptr_);

          homesvnt_creator_fptr_map.bind (homes_table[i].servant_entrypt_,
                                          homes_table[i].servant_fptr_);
        }      

      /*
      CIAO::Static_DomainApplicationManager_Impl* dapp_mgr =
        new CIAO::Static_DomainApplicationManager_Impl
                        (orb.in (),
                         poa.in (),
                         Deployment::TargetManager::_nil (),
                         plan,
                         init_file,
                         &maps);

      dapp_mgr->init (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (Deployment::DomainApplicationManager::_nil());

      dapp_mgr->set_uuid (plan.UUID.in ());

      // Create a dummy set of properties and start the
      // Launching of applications
      Deployment::Properties_var properties;
      ACE_NEW_RETURN (properties,
                      Deployment::Properties,
                      1);
      ACE_DEBUG ((LM_DEBUG, "Executor: start Launch application....."));
      // Start the Application immediately
      int start = 1;
      dapp_mgr->startLaunch (properties.in (), start);
      ACE_DEBUG ((LM_DEBUG, "[success]\n"));

      ACE_DEBUG ((LM_DEBUG, "Executor: finish Launch application....."));
      // Call finish Launch to complete the connections
      dapp_mgr->finishLaunch (start);
      ACE_DEBUG ((LM_DEBUG, "[success]\n"));

      ACE_DEBUG ((LM_DEBUG, "Executor: Application Deployed successfully \n"));
      */

      CIAO::Static_NodeDaemon_Impl *static_node_daemon_impl;
      // Create and install the CIAO Daemon servant
      
      ACE_DEBUG ((LM_DEBUG, "creating static_node_manager\n"));
      
      static_node_daemon_impl =
        new CIAO::Static_NodeDaemon_Impl("NodeDaemon",
                                         orb.in (),
                                         poa.in (),
                                         "", //exe location
                                         0, //spawn delay
                                         &static_config_entrypoints_maps);    

      static_node_daemon_impl->init ();

      CORBA::Object_var table_object =
        orb->resolve_initial_references ("IORTable"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (adapter.in ()))
          ACE_ERROR_RETURN ((LM_ERROR, "Nil IORTable\n"), -1);

      CIAO::NodeDaemon_var daemon =
        static_node_daemon_impl->_this ();

      CORBA::String_var str =
        orb->object_to_string (daemon.in ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      adapter->bind ("NodeManager",
                     str.in ()
                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      
      // Run the main event loop for the ORB.
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      
      /*
      ACE_DEBUG ((LM_DEBUG, "Executor: destroy the application....."));
      dapp_mgr->destroyApplication ();
      ACE_DEBUG ((LM_DEBUG, "[success]\n"));
      */

      poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  catch (CORBA::Exception& ex)
    {
      ACE_PRINT_EXCEPTION (ex, "Caught CORBA Exception: ");
      return -1;
    }

  return 0;
}
