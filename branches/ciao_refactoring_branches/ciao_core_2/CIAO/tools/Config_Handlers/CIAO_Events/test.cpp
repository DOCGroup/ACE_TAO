// $Id$

#include <iostream>

#include "CIAOEvents.hpp"
#include "CIAOEvents_Handler.h"
#include "DAnCE/Deployment/Deployment_EventsC.h"

#include "ace/Get_Opt.h"
#include "tao/ORB.h"

static const char *input_file = "test.ced";


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

  CIAOEvents_Handler event_handler (input_file);
  // Convert XSC to idl datatype

  std::cout << "Instance document import succeeded.  Dumping contents to file\n";

  std::cout << "Test completed!";

  return 0;

}
