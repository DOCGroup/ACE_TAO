// $Id$
#include <iostream>

#include "Deployment.hpp"
#include "DP_Handler.h"
#include "ciao/Deployment_DataC.h"
#include "ace/Get_Opt.h"
#include "XML_Helper.h"
#include "DnC_Dump.h"
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
  // Indicates sucessful parsing of the command-line
  return 0;  
}

using namespace CIAO::Config_Handlers;


int main (int argc, char *argv[])
{
  
  if (parse_args (argc, argv) != 0)
    return 1;

  // Initialize an ORB so Any will work
  CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv, "");
  
  XML_Helper the_helper;
  
  if (xercesc::DOMDocument *doc = the_helper.create_dom (input_file))
    {
      DeploymentPlan dp = deploymentPlan (doc);
      
      
      DP_Handler dp_handler (dp);
      
      std::cout << "Instance document import succeeded.  Dumping contents....\n";
      
      Deployment::DnC_Dump::dump (*dp_handler.plan ());
      
    }
  
  std::cout << "Test completed!\n";

  return 0;
}

    
