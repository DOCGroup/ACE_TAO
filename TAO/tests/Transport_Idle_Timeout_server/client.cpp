#include "Transport_Idle_TimeoutC.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Time_Value.h"

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
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage: %s -k <ior>\n",
                             argv[0]),
                            -1);
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

      Transport_Idle_Timeout_Test::Test_var test =
        Transport_Idle_Timeout_Test::Test::_narrow (object.in ());

      if (CORBA::is_nil (test.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "nil Test reference\n"), 1);

      // The first request opens the transport. With a 2 second idle
      // timeout, the server-side transport must remain usable because
      // this request is followed by another request before 2 seconds
      // have elapsed since the first request.
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client: first ping\n"));
      test->ping ();

      ACE_OS::sleep (ACE_Time_Value (1, 500000));

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client: second ping\n"));
      test->ping ();

      ACE_OS::sleep (ACE_Time_Value (1, 500000));

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client: third ping\n"));
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
