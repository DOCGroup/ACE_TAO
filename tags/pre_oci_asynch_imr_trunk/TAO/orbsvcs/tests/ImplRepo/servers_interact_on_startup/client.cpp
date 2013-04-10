// $Id$

#include "TestC.h"
#include <iostream>
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

int client_num = 0;
ACE_TCHAR *server_ior = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:n:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        server_ior = get_opts.opt_arg ();
        break;

        case 'n':
          client_num = ACE_OS::atoi (get_opts.opt_arg ());
          break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <server-ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    if (parse_args (argc, argv) != 0)
      return 1;

    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t|%T) Client %d sending request to %s\n",
                client_num, server_ior));
    CORBA::Object_var obj =
      orb->string_to_object (server_ior);
    ACE_ASSERT (!CORBA::is_nil (obj.in ()));
    Test_var test = Test::_narrow (obj.in());
    ACE_ASSERT (!CORBA::is_nil (test.in()));
    CORBA::Short n = test->get_server_num ();
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t|%T) Client %d received reply from server %d\n",
                client_num, n));

    return 0;

  }
  catch(const CORBA::Exception& ex) {
    ex._tao_print_exception ("client:");
  }

  return -1;
}
