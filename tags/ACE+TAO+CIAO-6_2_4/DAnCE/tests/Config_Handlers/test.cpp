// $Id$

#include <iostream>

#include "Deployment.hpp"
#include "DP_Handler.h"
#include "Deployment/Deployment_DataC.h"
#include "ace/Get_Opt.h"
#include "Utils/XML_Typedefs.h"
#include "tao/ORB.h"

static const char *input_file = "BasicSP.cdp";

static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "i:");

  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        input_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-i <input file> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command-line
  return 0;
}

// Check to see if SRD was imported.
void check_srd (const Deployment::DeploymentPlan &);

using namespace CIAO::Config_Handlers;


int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  if (parse_args (argc, argv) != 0)
    return 1;

  // Initialize an ORB so Any will work
  CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv);
  ACE_UNUSED_ARG (orb);

  //Create an XML_Helper for all the file work

  if (xercesc::DOMDocument *doc =
      XML_Helper::XML_HELPER.create_dom (input_file))
    {
      //Read in the XSC type structure from the DOMDocument
      DeploymentPlan dp = deploymentPlan (doc);

      //Convert the XSC to an IDL datatype

      DP_Handler dp_handler (dp);

      std::cout << "Instance document import succeeded.  Dumping contents to file\n";

      //Retrieve the newly created IDL structure
      Deployment::DeploymentPlan *idl = dp_handler.plan();

      // Check for server resources, if present....
      check_srd (*idl);

      //Convert it back to an XSC structure with a new DP_Handler
      DP_Handler reverse_handler(*idl);

      //Create a new DOMDocument for writing the XSC into XML
      xercesc::DOMDocument* the_xsc
        (XML_Helper::XML_HELPER.create_dom(0));

      //Serialize the XSC into a DOMDocument
      deploymentPlan(*reverse_handler.xsc(), the_xsc);


      //Write it to test.xml
      XML_Helper::XML_HELPER.write_DOM(the_xsc, "test.xml");

      //Cleanliness is next to Godliness
      delete doc;
    }

  std::cout << "Test completed!\n";

  return 0;
}


void check_srd (const Deployment::DeploymentPlan &dp)
{
  for (CORBA::ULong i = 0;
       i < dp.infoProperty.length ();
       ++i)
    {
      if (ACE_OS::strcmp (dp.infoProperty[i].name.in (),
                          "CIAOServerResources") == 0)
        {
          CIAO::DAnCE::ServerResource *test;

          if (dp.infoProperty[i].value >>= test)
            std::cerr << "ServerResources found and successfully extracted." << std::endl;
          else
            std::cerr << "ERROR: ServerResource extraction failed!" << std::endl;
        }
    }

}

