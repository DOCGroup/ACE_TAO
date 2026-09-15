#include "Transport_Idle_Timeout_Long_RequestC.h"

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "tao/Thread_Lane_Resources.h"
#include <chrono>

static size_t
cache_size (CORBA::ORB_ptr orb)
{
  return orb->orb_core ()->lane_resources ().transport_cache ().current_size ();
}

static void
run_for (CORBA::ORB_ptr orb, int seconds)
{
  const auto deadline = std::chrono::steady_clock::now () + std::chrono::seconds (seconds);
  while (std::chrono::steady_clock::now () < deadline)
    {
      ACE_Time_Value slice (0, 50000);
      orb->perform_work (slice);
    }
}

static bool
expect_cache (CORBA::ORB_ptr orb, size_t expected)
{
  const size_t actual = cache_size (orb);
  if (actual != expected)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) cache size %B, expected %B\n"),
                  actual, expected));
      return false;
    }
  return true;
}

// Observe closure without making a CORBA call that could reconnect.
static bool
wait_for_idle_close (CORBA::ORB_ptr orb, int seconds)
{
  const auto deadline = std::chrono::steady_clock::now () + std::chrono::seconds (seconds);
  while (cache_size (orb) != 0 && std::chrono::steady_clock::now () < deadline)
    {
      ACE_Time_Value slice (0, 50000);
      orb->perform_work (slice);
    }
  return expect_cache (orb, 0);
}


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

      // Y=4, X=1. Start a two-second call near the old idle deadline.
      test->ping ();
      run_for (orb.in (), 3);
      if (!expect_cache (orb.in (), 1))
        {
          return 1;
        }
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) client: long_request\n")));
      test->long_request ();

      // Completion must refresh activity: three seconds is less than Y,
      // but more than the idle time remaining from receipt of the request.
      run_for (orb.in (), 3);
      if (!expect_cache (orb.in (), 1)
          || !wait_for_idle_close (orb.in (), 4 + 1 + 1))
        {
          return 1;
        }
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) client: reconnect after idle scan\n")));
      test->ping ();
      if (!expect_cache (orb.in (), 1))
        {
          return 1;
        }

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
