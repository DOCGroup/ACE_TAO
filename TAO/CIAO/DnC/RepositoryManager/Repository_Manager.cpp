// $Id$

#include "RepositoryManager_Impl.h"
#include "Update_Plan.h"
#include "ExecutionManager/ExecutionManagerC.h"
#include "NodeManager/NodeDaemonC.h"
#include "ace/OS_NS_stdio.h"
#include "ace/streams.h"

const char * exec_ior = "file://exec_mgr.ior";
const char * node_daemon_ior = 0;

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
  XercesDOMParser::ValSchemes val_schema = XercesDOMParser::Val_Auto;
  
  char* package_url = 0;

  // deployment plan URL
  char* plan_url = 0;

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("p:d:k:t:"));
  int c;

  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
          case 'p':
            package_url = get_opt.opt_arg ();
            break;
          case 'd':
            plan_url = get_opt.opt_arg ();
            break;
          case 'k':
            exec_ior = get_opt.opt_arg ();
            break;
          case 't':
            node_daemon_ior = get_opt.opt_arg ();
            break;
          default:
            usage(argv[0]);
            return -1;
        }
    }

  if (package_url == 0) {
    usage(argv[0]);
    return -1;
  }

  if (plan_url == 0) {
    usage(argv[0]);
    return -1;
  }

  // Initialize the ORB so that CORBA::Any will work
  //
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "");

  try
    {
      xercesc::XMLPlatformUtils::Initialize();

      // get a reference to the parser.
      XercesDOMParser *plan_parser = new XercesDOMParser;

      // free up DOMBuilder. DOMBuilder also deletes the DOMDocument memory.
      auto_ptr<XercesDOMParser> cleanup_parser (plan_parser);

      plan_parser->setValidationScheme (val_schema);
      plan_parser->setDoNamespaces (true);
      plan_parser->setDoSchema (true);
      plan_parser->setValidationSchemaFullChecking (true);
      plan_parser->setCreateEntityReferenceNodes (false);
      plan_parser->setIncludeIgnorableWhitespace (false);
      plan_parser->parse (plan_url);

      // use the parser to parse the deployment plan URL and create
      // a DOM document.
      DOMDocument* plan_doc = plan_parser->getDocument ();
      if (plan_doc == NULL)
        {
          ACE_DEBUG ((LM_DEBUG, "Null DOM Document obtained, \
                      May be the URL is wrong!!\n"));
          throw Null_Dom_Document ();
        }

      // free up DOMBuilder. DOMBuilder also deletes the DOMDocument memory.
      //auto_ptr<DOMBuilder> cleanup_parser (parser);

      // call the Deployment Plan handler to parse the XML descriptor.
      CIAO::Config_Handler::Plan_Handler plan_handler (plan_doc,
                                                  DOMNodeFilter::SHOW_ELEMENT |
                                                  DOMNodeFilter::SHOW_TEXT);
      Deployment::DeploymentPlan plan;
      plan_handler.process_plan (plan);

      // call the PackageConfiguration handler to parse the XML descriptor.
      Deployment::PackageConfiguration* pc;
      CIAO::RepositoryManager_Impl rep_impl;
      rep_impl.installPackage ("PC", package_url);
      pc = rep_impl.findPackageByName ("PC");

      REF_MAP ref_map;
      REF_MAP primary_ref_map;

      // traverse the PackageConfiguration IDL data structure and
      // update the deployment plan IDL data structure.
      traverse_package (pc, plan, ref_map, primary_ref_map);

      //Deployment::DnC_Dump::dump (plan);

      // Pass the parsed plan to the Execution Manager to start the
      // Deployment Process.

      CORBA::Object_var obj = orb->string_to_object (exec_ior
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CIAO::ExecutionManagerDaemon_var exec_mgr =
        CIAO::ExecutionManagerDaemon::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (exec_mgr.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Executor: nil Execution Manager reference, \
                       narrow failed\n"));
          return 1;

        }

      CIAO::NodeDaemon_var node_mgr;
      if (node_daemon_ior != 0)
      {
        CORBA::Object_var node_mgr_obj = orb->string_to_object
                                                 (node_daemon_ior
                                                  ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        node_mgr =
          CIAO::NodeDaemon::_narrow (node_mgr_obj.in ()
                                     ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        if (CORBA::is_nil (node_mgr.in ()))
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Executor: nil Node Manager reference, \
                        narrow failed\n"));
            return 1;
          }
      }

      ACE_DEBUG ((LM_DEBUG, "Executor: Obtained Execution Manager ref \n"));
      Deployment::DomainApplicationManager_var dapp_mgr =
        exec_mgr->preparePlan (plan, 1);

      if (CORBA::is_nil (dapp_mgr.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Executor:preparePlan call failed:nil \
                       DomainApplicationManager reference\n"));
          return 1;
        }
      ACE_DEBUG ((LM_DEBUG,
                  "Executor: Obtained DomainApplication Manager ref \n"));

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
      ACE_DEBUG ((LM_DEBUG, "Press <Enter> to tear down application \n"));

      char dummy [256];
      std::cin.getline (dummy, 256);
      // Tear down the assembly

      ACE_DEBUG ((LM_DEBUG, "Executor: destroy the application....."));
      dapp_mgr->destroyApplication ();
      ACE_DEBUG ((LM_DEBUG, "[success]\n"));

      ACE_DEBUG ((LM_DEBUG, "Executor: destroy the manager....."));
      exec_mgr->destroyManager (dapp_mgr.in ());
      ACE_DEBUG ((LM_DEBUG, "[success]\n"));

      if (node_daemon_ior != 0)
        {
          ACE_DEBUG ((LM_DEBUG, "shutting down node manager \n"));
          exec_mgr->shutdown (); // shut down execution manager.
          ACE_DEBUG ((LM_DEBUG, "[success]\n"));

          ACE_DEBUG ((LM_DEBUG, "shutting down node manager \n"));
          node_mgr->shutdown (); // shut down the node manager.
          ACE_DEBUG ((LM_DEBUG, "[success]\n"));
        }

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  catch (CORBA::Exception& ex)
    {
      ACE_PRINT_EXCEPTION (ex, "Caught CORBA Exception: ");
      while (true); // @@ (OO) What purpose does this server?
      return -1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Caught unknown exception\n"));
      while (true); // @@ (OO) What purpose does this server?
      return -1;
    }

  return 0;
}
