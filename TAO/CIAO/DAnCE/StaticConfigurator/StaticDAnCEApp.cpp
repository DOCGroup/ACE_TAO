// $Id$

#include "NodeApplication_Impl.h"
#include "DeploymentC.h"
#include "CIAO/DAnCE/NodeApplicationManager/ImplementationInfo.h"
#include "CIAO/DAnCE/RepositoryManager/RepositoryManager_Impl.h"
#include "CIAO/DAnCE/RepositoryManager/Update_Plan.h"
#include "CIAO/DAnCE/DomainApplicationManager/DomainApplicationManager_Impl.h"
#include "Config_Handlers/DnC_Dump.h"
#include "orbsvcs/orbsvcs/Naming/Transient_Naming_Context.h"
#include "ace/OS_NS_stdio.h"
#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "tao/IORTable/IORTable.h"
#include "tao/CDR.h"
#include "CIAO/DAnCE/Config_Handlers/XercesString.h"
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/parsers/AbstractDOMParser.hpp>
#include "CIAO/DAnCE/Config_Handlers/Config_Handler_export.h"
#include "CIAO/DAnCE/Config_Handlers/Domain_Handler.h"
#include "CIAO/DAnCE/Config_Handlers/PC_Handler.h"
#include "CIAO/DAnCE/Config_Handlers/Plan_Handler.h"
#include "CIAO/DAnCE/Config_Handlers/CompImplDesc_Handler.h"
#include "CIAO/DAnCE/Config_Handlers/DnC_Dump.h"
#include "CIAO/DAnCE/Config_Handlers/Utils.h"
#include "CIAO/DAnCE/Config_Handlers/Config_Error_Handler.h"
#include "plan.h"

using Config_Handler::XStr;
using xercesc::XMLUni;
using xercesc::XMLString;
using xercesc::XMLException;
using xercesc::DOMException;
using xercesc::DOMBuilder;
using xercesc::DOMImplementationRegistry;
using xercesc::DOMImplementationLS;
using xercesc::DOMImplementation;
using xercesc::DOMAttr;
using xercesc::DOMNamedNodeMap;
using xercesc::DOMLocator;
using xercesc::DOMError;
using xercesc::DOMNodeList;
using xercesc::DOMDocument;
using xercesc::DOMDocumentTraversal;
using xercesc::DOMNodeIterator;
using xercesc::DOMNode;
using xercesc::DOMNodeFilter;
using xercesc::XercesDOMParser;
using xercesc::AbstractDOMParser;
using namespace std;
using namespace CIAO;

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
  /*
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("p:d:k:t:i:"));
  int c;

  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
          case 'p':
            package_url = get_opt.opt_arg ();
            break;
          case 'i':
            init_file = get_opt.opt_arg ();
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

  try
    {
      xercesc::XMLPlatformUtils::Initialize();
    }

  catch (const XMLException& xml_e)
    {
      char* message = XMLString::transcode (xml_e.getMessage());
      ACE_Auto_Basic_Array_Ptr<char> cleanup_message (message);
      ACE_DEBUG ((LM_DEBUG, "Error during initialization : %s\n", message));
      return 1;
    }
  */
  try
    {
      Deployment::DeploymentPlan plan;

      TAO_InputCDR in_cdr(plan_cdr_buffer, sizeof(plan_cdr_buffer)-1 );

      in_cdr >> plan;

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
      CIAO::Static_Config_EntryPoints_Maps maps;
      maps.home_creator_funcptr_map_ = &home_creator_fptr_map;
      maps.home_servant_creator_funcptr_map_ = &homesvnt_creator_fptr_map;

      int i=0;
      for (i=0; i<homes_table_size; ++i)
        {
          home_creator_fptr_map.bind (homes_table[i].executor_entrypt_,
                                      homes_table[i].executor_fptr_);

          homesvnt_creator_fptr_map.bind (homes_table[i].servant_entrypt_,
                                          homes_table[i].servant_fptr_);
        }      

      Static_DomainApplicationManager_Impl* dapp_mgr =
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

      // Run the main event loop for the ORB.
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Executor: destroy the application....."));
      dapp_mgr->destroyApplication ();
      ACE_DEBUG ((LM_DEBUG, "[success]\n"));

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
  catch (const DOMException& e)
    {
      const unsigned int maxChars = 2047;
      XMLCh errText[maxChars + 1];

      ACE_ERROR ((LM_ERROR, "\nException occured while parsing %s: \
                  \n",plan_url));
      ACE_ERROR ((LM_ERROR, "DOMException code: %d\n ", e.code));
      if (DOMImplementation::loadDOMExceptionMsg (e.code, errText, maxChars))
        {
          char* message = XMLString::transcode (errText);
          ACE_Auto_Basic_Array_Ptr<char> cleanup_message (message);
          ACE_ERROR ((LM_ERROR, "Message is: %s\n", message));
        }
      //ACE_PRINT_EXCEPTION ("Caught DOM Exception: ");
      ACE_ERROR ((LM_ERROR, "Caught DOM exception\n"));
      return -1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Caught unknown exception\n"));
      return -1;
    }

  return 0;
}
