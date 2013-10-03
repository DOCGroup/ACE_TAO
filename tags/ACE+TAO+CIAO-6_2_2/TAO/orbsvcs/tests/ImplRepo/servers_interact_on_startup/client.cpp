// $Id$

#include "TestC.h"
#include <iostream>
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

int client_num = 0;
ACE_TCHAR *server_ior = 0;
bool expect_transient = false;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("ek:n:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'e':
        expect_transient = true;
        break;

      case 'k':
        server_ior = get_opts.opt_arg ();
        break;

        case 'n':
          client_num = ACE_OS::atoi (get_opts.opt_arg ());
          break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-e ")
                           ACE_TEXT ("-k <server-ior> ")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

      if (parse_args (argc, argv) != 0)
        return 1;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t|%T) Client %d sending request to %C\n"),
                  client_num, server_ior));
      try
        {
          CORBA::Object_var obj =
            orb->string_to_object (server_ior);
          ACE_ASSERT (!CORBA::is_nil (obj.in ()));
          Test_var test = Test::_narrow (obj.in());
          ACE_ASSERT (!CORBA::is_nil (test.in()));
          CORBA::Short n = test->get_server_num ();
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t|%T) Client %d received reply from server %d\n"),
                      client_num, n));
        }
      catch (CORBA::TRANSIENT &)
        {
          if (expect_transient)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t|%T) Client %d got expected transient exception\n"),
                          client_num));
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t|%T) Client %d caught unexpected transent\n"),
                          client_num));
              return -1;
            }
        }
      return 0;

    }
  catch(const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("client:"));
    }

  return -1;
}
