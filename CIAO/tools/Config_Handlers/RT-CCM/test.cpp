// $Id$

#include <iostream>

#include "CIAOServerResources.hpp"
#include "SRD_Handler.h"
#include "ciao/ServerResourcesC.h"

#include "ace/Get_Opt.h"
#include "XML_Helper.h"
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
  CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv);

  //Create an XML_Helper for all the file work
  XML_Helper the_helper;

  if (xercesc::DOMDocument *doc = the_helper.create_dom (input_file))
  {
    //Read in the XSC type structure from the DOMDocument
    ServerResourcesDef srd = ServerResources (doc);

    // Convert XSC to idl datatype
    SRD_Handler srd_handler (&srd);

    std::cout << "Instance document import succeeded.  Dumping contents to file\n";

    CIAO::DAnCE::ServerResource *sr (srd_handler.srd_idl ());

    SRD_Handler reverse_handler (sr);

    xercesc::DOMDocument *the_xsc (the_helper.create_dom ("CIAO:ServerResources",
                                                          "http://www.dre.vanderbilt.edu/ServerResources"));

    std::cout << "NS:"
              << std::string (xercesc::XMLString::transcode (the_xsc->getDocumentElement ()->getNamespaceURI ())) << std::endl
              << std::string (xercesc::XMLString::transcode (the_xsc->getDocumentElement ()->getPrefix ())) << std::endl
              << std::string (xercesc::XMLString::transcode (the_xsc->getDocumentElement ()->getLocalName ())) << std::endl
              << std::endl;

    ServerResources (*reverse_handler.srd_xsc (), the_xsc);

    // write out the result
    the_helper.write_DOM (the_xsc, "output.srd");
  }

  std::cout << "Test completed!";

  return 0;

}
