#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_sys_time.h"

static const char *ior = nullptr;
static int timeout_sec = 3;

namespace
{
  void
  wait_with_reactor (CORBA::ORB_ptr orb, int seconds)
  {
    ACE_Time_Value const deadline =
      ACE_OS::gettimeofday () + ACE_Time_Value (seconds);

    while (ACE_OS::gettimeofday () < deadline)
      {
        ACE_Time_Value tv (0, 50000);
        orb->perform_work (tv);
      }
  }
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:t:"));
  int c;
  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'k':
          ior = get_opts.opt_arg ();
          break;
        case 't':
          timeout_sec = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Usage: client -k <ior> [-t <sec>]\n")),
                            -1);
        }
    }

  if (ior == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("client: -k <IOR> is required\n")),
                      -1);
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
      Test::OnewayIdle_var test = Test::OnewayIdle::_narrow (object.in ());
      if (CORBA::is_nil (test.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Narrow to Test::OnewayIdle failed\n")),
                          1);

      int const wait_sec = timeout_sec + 2;
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("Sending oneway ping; server waits %d seconds ")
                  ACE_TEXT ("with idle timeout %d seconds\n"),
                  wait_sec,
                  timeout_sec));

      test->ping (wait_sec);

      // Give the server enough time to finish the oneway operation before
      // fetching the value it recorded while handling that operation.
      wait_with_reactor (orb.in (), wait_sec + 1);

      CORBA::Long const observed = test->observed_cache_size ();
      if (observed != 1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("[FAIL] server cache size after oneway wait = %d; ")
                      ACE_TEXT ("expected 1 (transport was closed)\n"),
                      observed));
          test->shutdown ();
          orb->destroy ();
          return 1;
        }

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("[PASS] server cache size after oneway wait = %d; ")
                  ACE_TEXT ("transport remained open\n"),
                  observed));

      test->shutdown ();
      orb->destroy ();
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("client exception"));
      return 1;
    }

  return 0;
}
