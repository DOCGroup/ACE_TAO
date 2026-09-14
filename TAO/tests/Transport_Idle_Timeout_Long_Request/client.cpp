#include "Transport_Idle_Timeout_Long_RequestC.h"

#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://server.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'k':
          ior = get_opts.opt_arg ();
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage: %s -k <ior>\n", argv[0]), -1);
        }
    }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object = orb->string_to_object (ior);
      Transport_Idle_Timeout_Long_Request_Test::Test_var test =
        Transport_Idle_Timeout_Long_Request_Test::Test::_narrow (object.in ());

      if (CORBA::is_nil (test.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "nil Test reference\n"), 1);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client: long_request\n"));
      test->long_request ();

      // long_request() only returns if the server could send its reply after
      // two seconds, despite a one second transport idle timeout.
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client: ping\n"));
      test->ping ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client: shutdown\n"));
      test->shutdown ();
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }
  return 0;
}
