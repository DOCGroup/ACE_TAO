
//Author: Stoyan Paunov
//$Id$


#include "ace/OS_main.h"

#include "ciao/RepositoryManagerC.h"
#include "PCVisitor.h"
#include "PCVisitorBase.h"
#include "Config_Handlers/DnC_Dump.h"

#include "ciao/DeploymentC.h"
#include "ciao/Deployment_BaseC.h"
#include "ciao/Deployment_DataC.h"
#include "ciao/Packaging_DataC.h"

#include "Node_T.h"                  //Node
#include "ace/OS_Memory.h"           //for ACE_NEW* macros
#include "ace/Containers_T.h"        //for ACE_Double_Linked_List


#include "Config_Handlers/DP_Handler.h"
#include "Config_Handlers/Utils/XML_Helper.h"
#include "Config_Handlers/Deployment.hpp"
//using namespace CIAO::Config_Handlers;

#include <iostream>
using namespace std;


//Definition of a Node parametrized by
//Deployment::Node
namespace {
 typedef ::Node<Deployment::Node> NNode;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  char* name = 0;

  if (argc < 2)
  {
    cout << "Please provide name of package you want to deploy!\n";
  }
  else
    name = argv[1];


  ACE_TRY_NEW_ENV
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv,
                      ""ACE_ENV_ARG_PARAMETER);

      ACE_TRY_CHECK;

      //form the path to the RepoMan IOR file
      ACE_CString RepoMan_ior = ("file://");
      RepoMan_ior += ACE_OS::getenv ("CIAO_ROOT");
      RepoMan_ior += "/DAnCE/RepositoryManager/RepositoryManagerDeamon.ior";

      //get an object reference to the RepositoryManager
      CORBA::Object_var obj =
         orb->string_to_object (RepoMan_ior.c_str ()
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Deployment::RepositoryManager_var rm =
      Deployment::RepositoryManager::_narrow (obj.in ()
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (rm.in ()))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to acquire RepositoryManagerDaemon's objref\n"),
                           -1);
      }

      Deployment::PackageConfiguration_var pc;
      try
      {
        pc = rm->findPackageByName (name);
        cout << "The package was found!" << endl;
      }
      catch (CORBA::Exception &)
      {
        cout << "Unable to retrieve package!\n" << endl;
        return -1;
      }
      ACE_TRY_CHECK;

      Deployment::DeploymentPlan plan;

      PCVisitor pcv (plan, *pc);
      pcv.Visit ();
      ACE_TRY_CHECK;

      cout << "--> Printing populated DeploymentPlan\n";
      ::Deployment::DnC_Dump::dump (plan);
      cout << "End of populated DeploymentPlan dump\n---------------------------------------\n";
      ACE_TRY_CHECK;

/*-- NOTE: Currently reverse handler segfaults

      //Create an XML_Helper for all the file work
      XML_Helper the_helper;

      //Convert it back to an XSC structure with a new DP_Handler
      DP_Handler reverse_handler(plan);

      //Create a new DOMDocument for writing the XSC into XML
      xercesc::DOMDocument* the_xsc (the_helper.create_dom(0));

      //Serialize the XSC into a DOMDocument
      deploymentPlan(*reverse_handler.xsc(), the_xsc);


      //Write it to test.xml
      the_helper.write_DOM(the_xsc, "generatedDP.xml");
*/

      // Finally destroy the ORB
      orb->destroy ();
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unknown exception \n");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
