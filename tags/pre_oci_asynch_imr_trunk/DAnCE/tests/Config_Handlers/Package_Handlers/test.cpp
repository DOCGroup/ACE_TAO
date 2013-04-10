// $Id$

#include <iostream>

#include "Deployment.hpp"
#include "PCD_Handler.h"
#include "Deployment/Deployment_Packaging_DataC.h"
#include "SID_Handler.h"
#include "ace/Get_Opt.h"
#include "Utils/XML_Helper.h"
#include "tao/ORB.h"
#include "Utils/Exceptions.h"

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
  try
    {
      if (parse_args (argc, argv) != 0)
        return 1;

      // Initialize an ORB so Any will work
      CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv);
      ACE_UNUSED_ARG (orb);


      if (xercesc::DOMDocument *doc = XML_HELPER->create_dom (input_file))
        {

          ::Deployment::PackageConfiguration idl_pc;
          {
            //            //Read in the XSC type structure from the DOMDocument
            //PackageConfiguration pc = packageConfiguration (doc);
            //std::cerr << "XML to XSC conversion succeeded. Converting to IDL...\n";

            //Convert the XSC to an IDL datatype
            Packaging::PCD_Handler::package_config (input_file, idl_pc);
            std::cout << "Instance document import succeeded.  Dumping contents to file\n";
          }

          // Clear SID_Handler's IDREF Table
          Packaging::SID_Handler::IDREF.unbind_refs ();

          std::cerr << "Performing IDL->XSC transformation...\n";

          PackageConfiguration out_pc (Packaging::PCD_Handler::package_config (idl_pc));

          //Create a new DOMDocument for writing the XSC into XML
          xercesc::DOMDocument* the_xsc (XML_HELPER->create_dom("Deployment:packageConfiguration",
                                                                "http://www.omg.org/Deployment"));

          //Serialize the XSC into a DOMDocument
          packageConfiguration(out_pc, the_xsc);


          //Write it to test.xml
          XML_HELPER->write_DOM(the_xsc, "test.xml");

          //Cleanliness is next to Godliness
          delete doc;
        }

      std::cout << "Test completed!\n";
    }
  catch (CIAO::Config_Handlers::Plan_Error &excep)
    {
      std::cerr << "Plan Error exception caught: " << excep.reason_ << std::endl;
    }
  catch (CIAO::Config_Handlers::Config_Error &excep)
    {
      std::cerr << "Config Error " << excep.name_ << ": " << excep.error_ << std::endl;
    }


  return 0;
}


