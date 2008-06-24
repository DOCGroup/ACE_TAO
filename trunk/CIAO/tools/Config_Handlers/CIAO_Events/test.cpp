// $Id$

#include <iostream>

#include "CIAOEvents.hpp"
#include "CIAOEvents_Handler.h"
#include "DAnCE/Deployment/Deployment_EventsC.h"

#include "ace/Get_Opt.h"
#include "tao/ORB.h"

static const ACE_TCHAR *input_file = ACE_TEXT ("test.ced");

static int
parse_args (int argc, ACE_TCHAR *argv[])
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

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  if (parse_args (argc, argv) != 0)
    return 1;

  CIAOEvents_Handler event_handler (ACE_TEXT_ALWAYS_CHAR (input_file));
  // Convert XSC to idl datatype

  std::cout << "Instance document import succeeded.  Dumping contents to file\n";

  std::cout << "Test completed!";

  return 0;

}
