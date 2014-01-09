// $Id$

#include "TestC.h"
#include <iostream>
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "tao/Stub.h"
#include "tao/Messaging/Messaging.h"

char pause_poa = ' ';
char resume_poa = ' ';

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT(""));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
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
    if (CORBA::is_nil(obj.in()))
      {
        ACE_DEBUG ((LM_DEBUG, "Client could not RIR Test\n"));
        return 1;
      }
    Test_var test = Test::_narrow( obj.in() );
    if (CORBA::is_nil(test.in()))
      {
        ACE_DEBUG ((LM_DEBUG, "Client could not narrow Test\n"));
        return 1;
      }

    test->_stubobj()->reset_profiles();

    test->foo ();

    CORBA::Any a;
    a <<= Messaging::SYNC_WITH_SERVER;

    CORBA::PolicyList policy_list (1);
    policy_list.length (1);
    policy_list[0] =
      orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE, a);

    obj = test->_set_policy_overrides (policy_list, CORBA::ADD_OVERRIDE);
    test = Test::_narrow ( obj.in());

    test->_stubobj()->reset_profiles();

    test->foo ();

    CORBA::Short n = test->get_call_count();
    ACE_DEBUG ((LM_DEBUG,
                "Got call count of %d\n",
                n));

    return n != 1;

  }
  catch(const CORBA::Exception& ex) {
    ex._tao_print_exception ("client:");
  }

  return -1;
}
