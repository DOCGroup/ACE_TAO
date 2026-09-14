// Periodic idle scan regression: close, reconnect, activity refresh and opt-out.
// -t is idle age Y; -x is scan interval X. Both must match svc.conf.
// Client cache checks run the local reactor to observe peer closure.

#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Log_Msg.h"
#include <chrono>
#include "tao/ORB_Core.h"
#include "tao/Transport_Cache_Manager_T.h"
#include "tao/Thread_Lane_Resources.h"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

/// Keep this ORB's reactor running while waiting on a monotonic deadline.
void
sleep_with_reactor (CORBA::ORB_ptr orb, int seconds)
{
  const auto deadline = std::chrono::steady_clock::now ()
    + std::chrono::seconds (seconds);
  while (std::chrono::steady_clock::now () < deadline)
    {
      ACE_Time_Value tv (0, 50000);
      orb->perform_work (tv);
    }
}

/// Verify an expected value and print PASS/FAIL.  Returns false on failure.
bool
check (const char *label, size_t got, size_t expected)
{
  if (got == expected)
    {
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("(%P|%t)   [PASS] %C : cache_size = %B (expected %B)\n"),
                  label, got, expected));
      return true;
    }
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("(%P|%t)   [FAIL] %C : cache_size = %B (expected %B)\n"),
              label, got, expected));
  return false;
}

/// Retrieve the current size of the cache in the client
size_t
cache_size(CORBA::ORB_ptr orb)
{
  TAO_ORB_Core *core = orb->orb_core ();
  return core->lane_resources ().transport_cache ().current_size ();
}

// ---------------------------------------------------------------------------
// Argument parsing
// ---------------------------------------------------------------------------

static const char *ior          = nullptr;
static int         timeout_sec  = 3;    // must match server svc.conf value
static int         scan_interval_sec = 2;
static int         loop_count   = 10;
static bool        disabled_tc  = false;

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:t:x:n:d"));
  int c;
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':  ior         = get_opts.opt_arg ();           break;
      case 't':  timeout_sec = ACE_OS::atoi (get_opts.opt_arg ()); break;
      case 'x':  scan_interval_sec = ACE_OS::atoi (get_opts.opt_arg ()); break;
      case 'n':  loop_count  = ACE_OS::atoi (get_opts.opt_arg ()); break;
      case 'd':  disabled_tc = true;                           break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
            ACE_TEXT ("(%P|%t) Usage: client -k <ior> [-t <sec>] [-x <sec>] [-n <N>] [-d]\n")),
            -1);
      }
  if (timeout_sec < 2 || scan_interval_sec < 1 || loop_count < 1)
    ACE_ERROR_RETURN ((LM_ERROR,
        ACE_TEXT ("(%P|%t) require -t >= 2, -x >= 1 and -n >= 1\n")), -1);
  if (ior == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
        ACE_TEXT ("(%P|%t) client: -k <IOR> is required\n")), -1);
  return 0;
}

// ---------------------------------------------------------------------------
// TC-1 : Basic idle close
// ---------------------------------------------------------------------------
// Steps:
//   1. Ping once  -> transport created, server cache_size must be 1.
//   2. Wait Y + X + margin while processing local reactor events.
//   3. cache_size must be 0 -> transport closed by an idle scan.
// ---------------------------------------------------------------------------
static bool
tc1_basic_idle_close (CORBA::ORB_ptr orb, Test::Echo_ptr echo)
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) \n=== TC-1: Basic idle close ===\n")));
  bool ok = true;

  // --- Step 1: establish a transport ---
  ok &= echo->ping (0, 1, Test::Echo::_nil (), 0, 0, 0);

  ok &= check ("TC-1 after ping (expect 1)", cache_size(orb), 1);

  // --- Step 2: idle sleep ---
  int const sleep_sec = timeout_sec + scan_interval_sec + 1;
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("(%P|%t)   sleeping %d s for idle scan to close the transport...\n"),
              sleep_sec));
  sleep_with_reactor (orb, sleep_sec);

  // --- Step 3: cache must be empty now ---
  ok &= check ("TC-1 after idle timeout (expect 0)", cache_size(orb), 0);

  return ok;
}

// ---------------------------------------------------------------------------
// TC-2 : Reconnect after idle close
// ---------------------------------------------------------------------------
// Steps:
//   1. Ping (reconnects transparently after TC-1 closed the transport).
//   2. cache_size must be 1 again.
// ---------------------------------------------------------------------------
static bool
tc2_reconnect (CORBA::ORB_ptr orb, Test::Echo_ptr echo)
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) \n=== TC-2: Reconnect after idle close ===\n")));
  bool ok = true;

  // A new ping must succeed without TRANSIENT even though TC-1 caused the
  // server to close the connection.  TAO's reconnect logic handles this.
  ok &= echo->ping (0, 1, Test::Echo::_nil (), 0, 0, 0);

  ok &= check ("TC-2 after reconnect ping (expect 1)", cache_size(orb), 1);

  return ok;
}

// ---------------------------------------------------------------------------
// TC-3 : Activity refresh on reuse
// ---------------------------------------------------------------------------
// Send pings over multiple scan intervals, then check retention before Y
// and eventual closure after Y + X plus margin from the last activity.
// ---------------------------------------------------------------------------
static bool
tc3_activity_on_reuse (CORBA::ORB_ptr orb, Test::Echo_ptr echo)
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("(%P|%t) \n=== TC-3: Activity refresh on reuse (%d pings) ===\n"),
              loop_count));
  bool ok = true;

  // Spaced reuse crosses scan intervals; check before the next invocation.
  for (int i = 0; i < loop_count; ++i)
    {
      ok &= echo->ping (0, 1, Test::Echo::_nil (), 0, 0, 0);
      if (i + 1 < loop_count)
        {
          sleep_with_reactor (orb, 1);
          ok &= check ("TC-3 between pings (expect 1)", cache_size (orb), 1);
        }
    }

  // The last ping refreshed activity; the transport must still be cached.
  ok &= check ("TC-3 immediately after loop (expect 1)", cache_size(orb), 1);

  // Sleep to just before the expected timeout
  int const pre_sleep = timeout_sec - 1;
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("(%P|%t)   sleeping %d s (pre-timeout)...\n"), pre_sleep));
  sleep_with_reactor (orb, pre_sleep);

  ok &= check ("TC-3 before timeout (expect 1)", cache_size(orb), 1);

  // Sleep past the remainder of the timeout
  int const post_sleep = timeout_sec - pre_sleep + scan_interval_sec + 1;
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("(%P|%t)   sleeping %d s (post-timeout)...\n"), post_sleep));
  sleep_with_reactor (orb, post_sleep);

  ok &= check ("TC-3 after timeout (expect 0)", cache_size(orb), 0);

  return ok;
}

// ---------------------------------------------------------------------------
// TC-4 : Disabled timeout (opt-out)
// ---------------------------------------------------------------------------
// The server is started with -ORBTransportIdleTimeout 0 for this scenario.
// After waiting beyond the enabled expiry window, the connection must remain.
// ---------------------------------------------------------------------------
static bool
tc4_disabled_timeout (CORBA::ORB_ptr orb, Test::Echo_ptr echo)
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) \n=== TC-4: Disabled timeout ===\n")));
  bool ok = true;

  ok &= echo->ping (0, 1, Test::Echo::_nil (), 0, 0, 0);

  ok &= check ("TC-4 after ping (expect 1)", cache_size(orb), 1);

  // Wait longer than the enabled Y + X window; both ORBs disable expiry.
  const int sleep_sec = timeout_sec + scan_interval_sec + 1;
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("(%P|%t)   sleeping %d s (timeout should NOT fire)...\n"),
              sleep_sec));
  sleep_with_reactor (orb, sleep_sec);

  ok &= check ("TC-4 after sleep with timeout=0 (expect 1)", cache_size(orb), 1);

  return ok;
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var obj = orb->string_to_object (ior);
      Test::Echo_var echo = Test::Echo::_narrow (obj.in ());

      if (CORBA::is_nil (echo.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Narrow to Test::Echo failed\n")), 1);

      bool all_pass = true;

      if (disabled_tc)
        {
          // TC-4 only — server was started without idle timeout
          all_pass &= tc4_disabled_timeout (orb.in (), echo.in ());
        }
      else
        {
          // TC-1, TC-2, TC-3 in sequence
          all_pass &= tc1_basic_idle_close (orb.in (), echo.in ());
          all_pass &= tc2_reconnect        (orb.in (), echo.in ());
          all_pass &= tc3_activity_on_reuse (orb.in (), echo.in ());
        }

      // Shut down the server
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) Shutting down echo\n")));
      echo->shutdown ();

      orb->destroy ();

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("(%P|%t) \n=== Overall result: %C ===\n"),
                  all_pass ? "PASS" : "FAIL"));
      return all_pass ? 0 : 1;
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("client exception"));
      return 1;
    }
}
