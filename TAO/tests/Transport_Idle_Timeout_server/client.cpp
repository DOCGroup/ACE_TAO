#include "Transport_Idle_TimeoutC.h"

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/Time_Value.h"
#include "tao/ORB_Core.h"
#include "tao/Thread_Lane_Resources.h"
#include <chrono>
#include <string>

static size_t
cache_size (CORBA::ORB_ptr orb)
{
  return orb->orb_core ()->lane_resources ().transport_cache ().current_size ();
}

static void
run_for (CORBA::ORB_ptr orb, int seconds)
{
  std::chrono::steady_clock::time_point const deadline =
    std::chrono::steady_clock::now () + std::chrono::seconds (seconds);
  while (std::chrono::steady_clock::now () < deadline)
    {
      ACE_Time_Value slice (0, 50000);
      orb->perform_work (slice);
    }
}

static bool
expect_cache (CORBA::ORB_ptr orb, size_t expected)
{
  size_t const actual = cache_size (orb);
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
  std::chrono::steady_clock::time_point const deadline =
    std::chrono::steady_clock::now () + std::chrono::seconds (seconds);
  while (cache_size (orb) != 0 && std::chrono::steady_clock::now () < deadline)
    {
      ACE_Time_Value slice (0, 50000);
      orb->perform_work (slice);
    }
  return expect_cache (orb, 0);
}

struct Client_Options
{
  std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://server.ior")};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int option {};

  while ((option = get_opts ()) != -1)
    {
      switch (option)
        {
        case 'k':
          options.ior = get_opts.opt_arg ();
          break;
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage: %s -k <ior>\n"),
                             argv [0]),
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
      Client_Options options;
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object = orb->string_to_object (options.ior.c_str ());

      Transport_Idle_Timeout_Test::Test_var const test =
        Transport_Idle_Timeout_Test::Test::_narrow (object.in ());

      if (CORBA::is_nil (test.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "nil Test reference\n"), 1);

      // Y=2, X=2. Reuse for five seconds, beyond the first Y+X window.
      // Process closure notifications and check before each new invocation
      // so transparent reconnect cannot conceal an early server-side close.
      for (int i = 0; i != 6; ++i)
        {
          if (i != 0)
            {
              run_for (orb.in (), 1);
              if (!expect_cache (orb.in (), 1))
                {
                  return 1;
                }
            }
          test->ping ();
        }

      // Then stop all requests and let the server scanner close the connection.
      if (!wait_for_idle_close (orb.in (), 2 + 2 + 1))
        {
          return 1;
        }
      test->ping ();
      if (!expect_cache (orb.in (), 1))
        {
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client: shutdown\n"));
      test->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const & ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
