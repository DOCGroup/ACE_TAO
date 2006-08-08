// $Id: test.cpp 72251 2006-04-21 16:29:20Z wotte $

#include <iostream>

#include "NetQoS/NetQoSRequirements.hpp"
#include "NetQoS/NetQoS_Handler.h"
#include "ciao/NetQoSC.h"

#include "ace/Get_Opt.h"
#include "Utils/XML_Helper.h"
#include "tao/ORB.h"

static const char *input_file = "NetQoS-example.xml";

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
  try {
    if (parse_args (argc, argv) != 0)
      return 1;
  
    // Initialize an ORB so Any will work
    CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv, "");

    //Create an XML_Helper for all the file work
    XML_Helper the_helper;
  
    if (xercesc::DOMDocument *doc = the_helper.create_dom (input_file))
    {
      //Read in the XSC type structure from the DOMDocument
      NetQoSRequirementsDef net_qos = NetQoSRequirements (doc);

      // Convert XSC to idl datatype
      NetQoS_Handler net_qos_handler (new NetQoSRequirementsDef (net_qos));
/*
      std::cout << "Instance document import succeeded.  Dumping contents to file\n";
  
      CIAO::DAnCE::NetQoSRequirement *sr (net_qos_handler.netqos_idl ());
  
      NetQoS_Handler reverse_handler (sr);
  
      xercesc::DOMDocument *the_xsc (the_helper.create_dom ("CIAO:NetQoSRequirements",
                                                            "http://www.dre.vanderbilt.edu/NetQoSRequirements"));
  
      std::cout << "NS:"
                << std::string (xercesc::XMLString::transcode (the_xsc->getDocumentElement ()->getNamespaceURI ())) << std::endl
                << std::string (xercesc::XMLString::transcode (the_xsc->getDocumentElement ()->getPrefix ())) << std::endl
                << std::string (xercesc::XMLString::transcode (the_xsc->getDocumentElement ()->getLocalName ())) << std::endl
                << std::endl;
  
      //NetQoSRequirements (*reverse_handler.netqos_xsc (), the_xsc);
  
      // write out the result
      the_helper.write_DOM (the_xsc, "output.netqos");
*/
    }
  }
  catch (CIAO::Config_Handlers::NetQoS_Handler::NoNetQoS)
  {
    std::cerr << "Not a valid NetQoS file." << std::endl;
  }

  std::cout << "Test completed!";

  return 0;

}
