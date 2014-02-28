// $Id$

#include "TestC.h"
#include <iostream>
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

int request_delay_secs = 0;
bool server_abort = false;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("d:a"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        request_delay_secs = ACE_OS::atoi(get_opts.opt_arg ());
        break;

      case 'a':
        server_abort = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-d <request delay in seconds> "
                           "-a [abort server] "
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

    CORBA::Object_var obj = orb->resolve_initial_references("Test");
    ACE_ASSERT (!CORBA::is_nil(obj.in()));
    Test_var test = Test::_narrow( obj.in() );
    ACE_ASSERT (!CORBA::is_nil(test.in()));

    if (server_abort)
      {
        test->abort (request_delay_secs);
      }
    else
      {
        CORBA::Short n = test->get_server_num (request_delay_secs);
        ACE_DEBUG ((LM_DEBUG,
                    "Client received reply from server %d\n",
                    n));
      }

    return 0;

  }
  catch(const CORBA::Exception& ex) {
    ex._tao_print_exception ("client:");
  }

  return -1;
}
