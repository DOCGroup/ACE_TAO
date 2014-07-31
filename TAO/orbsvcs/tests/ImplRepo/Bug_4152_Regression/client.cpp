// $Id$

#include "TestC.h"
#include <iostream>
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"


bool killit = false;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        killit = true;
        break;
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
    Test_var test;

    try
      {
        test = Test::_narrow( obj.in() );
        if (killit)
          {
            test->terminate ();
          }
        else
          {
            CORBA::Short n = test->get_server_num ();
            ACE_DEBUG ((LM_DEBUG,
                        "Client received reply from server %d on first attempt\n",
                        n));
          }
      }
    catch (const CORBA::Exception &ex)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "Client caught: %s on first attempt, retrying\n",
                    ex._name ()));
        try
          {
            if (CORBA::is_nil (test.in()))
              {
                test = Test::_narrow( obj.in() );
              }
            CORBA::Short n = test->get_server_num ();
            ACE_DEBUG ((LM_DEBUG,
                        "Client received reply from server %d on second attempt\n",
                        n));
          }
        catch (const CORBA::Exception &ex)
          {
            ex._tao_print_exception ("Client second attempt: ");
          }
      }
    return 0;

  }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client:");
    }

  return -1;
}
