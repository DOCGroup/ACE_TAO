// Regression-test client for the TAO idle-transport-timeout feature.
//
// Test scenarios (executed in order):
//
//  TC-1  Basic idle close
//        Make one ping; verify server-side cache_size == 1; sleep past
//        the timeout; verify cache_size == 0 (transport closed by timer).
//
//  TC-2  Reconnect after idle close
//        After TC-1, ping again; verify cache_size == 1 again (new
//        transport created transparently).  Confirms TRANSIENT is not
//        raised by the reconnect path.
//
//  TC-3  Timer cancellation on reuse
//        Ping rapidly N times in a tight loop; the transport is
//        continuously reacquired so the idle timer is cancelled and
//        rescheduled each cycle.  After the loop, sleep just under the
//        timeout and verify cache_size == 1 (connection still alive).
//        Then sleep past the timeout and verify cache_size == 0.
//
//  TC-4  Disabled timeout (opt-out)
//        A second ORB is initialised with -ORBTransportIdleTimeout 0.
//        After sleeping well past the default 60 s timeout (we use a
//        short test timeout of 1 s so the second ORB uses 0 = disabled),
//        cache_size still reflects the connection is open.
//        NOTE: This scenario requires a separate server run with
//        -ORBTransportIdleTimeout 0 in its svc.conf; run_test.pl
//        handles this.  Within this binary TC-4 is a command-line flag.
//
// Usage:
//   client -k <IOR> -t <timeout_sec> [-n <loop_count>] [-d]
//
//   -t <sec>   The idle timeout configured on the *server* (default: 3).
//              The client sleeps for (timeout + 2) seconds to give the
//              reactor sufficient time to fire the timer.
//   -n <N>     Number of rapid-fire pings in TC-3 (default: 10).
//   -d         Run TC-4 (disabled-timeout) scenario instead of TC-1..3.

#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_sys_time.h"
#include "tao/ORB_Core.h"
#include "tao/Transport_Cache_Manager_T.h"
#include "tao/Thread_Lane_Resources.h"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

/// Sleep for @a seconds, spinning on reactor events so that the server's
/// reactor thread (which fires the idle timer) is not blocked.
/// We cannot use ACE_OS::sleep() alone because in a single-process test
/// harness the reactor runs in the same thread.  For a two-process test
/// the sleep is fine; for safety we drain reactor events anyway.
void
sleep_with_reactor (CORBA::ORB_ptr orb, int seconds)
{
  ACE_Time_Value deadline =
      ACE_OS::gettimeofday () + ACE_Time_Value (seconds);

  while (ACE_OS::gettimeofday () < deadline)
    {
      ACE_Time_Value tv (0, 50000); // 50 ms slices
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
                  ACE_TEXT ("(%P|%t)  [PASS] %C : cache_size = %B (expected %B)\n"),
                  label, got, expected));
      return true;
    }
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("(%P|%t)  [FAIL] %C : cache_size = %B (expected %B)\n"),
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
static const char *ior2         = nullptr;
static int         timeout_sec  = 3;    // must match server svc.conf value
static int         loop_count   = 10;
static bool        disabled_tc  = false;

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:l:t:n:d"));
  int c;
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':  ior         = get_opts.opt_arg ();           break;
      case 'l':  ior2        = get_opts.opt_arg ();           break;
      case 't':  timeout_sec = ACE_OS::atoi (get_opts.opt_arg ()); break;
      case 'n':  loop_count  = ACE_OS::atoi (get_opts.opt_arg ()); break;
      case 'd':  disabled_tc = true;                           break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
            ACE_TEXT ("Usage: client -k <ior> [-l <ior>] [-t <sec>] [-n <N>] [-d]\n")),
            -1);
      }
  if (ior == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
        ACE_TEXT ("client: -k <IOR> is required\n")), -1);
  return 0;
}

// ---------------------------------------------------------------------------
// TC-1 : Basic idle close
// ---------------------------------------------------------------------------
// Steps:
//   1. Ping once  -> transport created, server cache_size must be 1.
//   2. Sleep (timeout + 2s) -> idle timer fires on the server.
//   3. cache_size must be 0 -> transport closed by timer.
// ---------------------------------------------------------------------------
static bool
tc1_basic_idle_close (CORBA::ORB_ptr orb, Test::Echo_ptr echo, Test::Echo_ptr echo2)
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("\n=== TC-1: Basic idle close ===\n")));
  bool ok = true;

  // --- Step 1: establish a transport ---
  ok &= echo->ping (0, 1, echo2, 0, 2, 1);

  ok &= check ("TC-1 after ping (expect 1)", cache_size(orb), 1);

  // --- Step 2: idle sleep ---
  int const sleep_sec = timeout_sec + 2;
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("  sleeping %d s for idle timer to fire...\n"),
              sleep_sec));
  sleep_with_reactor (orb, sleep_sec);

  // --- Step 3: cache must be empty now ---
  ok &= check ("TC-1 after idle timeout (expect 0)", cache_size(orb), 0);

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("\n=== TC-1: Call again ping ===\n")));

  // Make another call, but let wait sleep_sec after the ping to the second server,
  // which means the call takes longer as the idle time, but it keeps it in the cache
  ok &= echo->ping (0, 1, echo2, sleep_sec, 1, 1);

  ok &= check ("TC-1 after ping longer as timeout (expect 1)", cache_size(orb), 1);

  sleep_with_reactor (orb, sleep_sec);

    // --- Step 4: cache must be empty now ---
  ok &= check ("TC-1 after second idle timeout (expect 0)", cache_size(orb), 0);

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
tc2_reconnect (CORBA::ORB_ptr orb, Test::Echo_ptr echo, Test::Echo_ptr echo2)
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("\n=== TC-2: Reconnect after idle close ===\n")));
  bool ok = true;

  // A new ping must succeed without TRANSIENT even though TC-1 caused the
  // server to close the connection.  TAO's reconnect logic handles this.
  ok &= echo->ping (0, 1, echo2, 0, 2, 1);

  ok &= check ("TC-2 after reconnect ping (expect 1)", cache_size(orb), 1);

  return ok;
}

// ---------------------------------------------------------------------------
// TC-3 : Timer cancellation on continuous reuse
// ---------------------------------------------------------------------------
// Steps:
//   1. Send N pings in rapid succession.  Each one reacquires the transport
//      (BUSY), cancels the idle timer, then releases back to idle and
//      reschedules the timer.  No close should occur mid-loop.
//   2. Sleep (timeout - 1)s — still within the window, cache must be 1.
//   3. Sleep another 4s — now past the timeout, cache must be 0.
// ---------------------------------------------------------------------------
static bool
tc3_timer_cancel_on_reuse (CORBA::ORB_ptr orb, Test::Echo_ptr echo, Test::Echo_ptr echo2)
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("\n=== TC-3: Timer cancellation on reuse (%d pings) ===\n"),
              loop_count));
  bool ok = true;

  // Rapid-fire loop — transport reused each time
  for (int i = 0; i < loop_count; ++i)
    {
      ok &= echo->ping (0, 2, echo2, 0, 2, 1);
    }

  // Immediately after the loop the transport returned to idle and the
  // timer was (re)started.  Cache must show 1 entry.
  ok &= check ("TC-3 immediately after loop (expect 1)", cache_size(orb), 1);

  // Sleep to just before the expected timeout
  int const pre_sleep = (timeout_sec > 1) ? timeout_sec - 1 : 1;
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("  sleeping %d s (pre-timeout)...\n"), pre_sleep));
  sleep_with_reactor (orb, pre_sleep);

  ok &= check ("TC-3 before timeout (expect 1)", cache_size(orb), 1);

  // Sleep past the remainder of the timeout
  int constexpr post_sleep = 4;
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("  sleeping %d s (post-timeout)...\n"), post_sleep));
  sleep_with_reactor (orb, post_sleep);

  ok &= check ("TC-3 after timeout (expect 0)", cache_size(orb), 0);

  return ok;
}

// ---------------------------------------------------------------------------
// TC-4 : Disabled timeout (opt-out)
// ---------------------------------------------------------------------------
// The server is started with -ORBTransportIdleTimeout 0 for this scenario.
// After sleeping well past the default timeout, the transport must still
// be present (i.e. not closed).
// ---------------------------------------------------------------------------
static bool
tc4_disabled_timeout (CORBA::ORB_ptr orb, Test::Echo_ptr echo, Test::Echo_ptr echo2)
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("\n=== TC-4: Disabled timeout ===\n")));
  bool ok = true;

  ok &= echo->ping (0, 2, echo2, 0, 1, 1);

  ok &= check ("TC-4 after ping (expect 1)", cache_size(orb), 1);

  // With timeout disabled the connection must never be closed.
  // We use a short wall-clock sleep equal to (timeout_sec + 2) — when
  // run_test.pl invokes TC-4 the server timeout is 0, so the timer never
  // fires regardless.
  const int sleep_sec = timeout_sec + 2;
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("  sleeping %d s (timeout should NOT fire)...\n"),
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
      CORBA::Object_var obj2;
      if (ior2)
        {
          ACE_DEBUG ((LM_INFO, ACE_TEXT ("Client received echo2\n")));
          obj2 = orb->string_to_object (ior2);
        }
      Test::Echo_var echo = Test::Echo::_narrow (obj.in ());
      Test::Echo_var echo2 = Test::Echo::_narrow (obj2.in ());

      if (CORBA::is_nil (echo.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Narrow to Test::Echo failed\n")), 1);

      bool all_pass = true;

      if (disabled_tc)
        {
          // TC-4 only — server was started without idle timeout
          all_pass &= tc4_disabled_timeout (orb.in (), echo.in (), echo2.in ());
        }
      else
        {
          // TC-1, TC-2, TC-3 in sequence
          all_pass &= tc1_basic_idle_close (orb.in (), echo.in (), echo2.in ());
          all_pass &= tc2_reconnect        (orb.in (), echo.in (), echo2.in ());
          all_pass &= tc3_timer_cancel_on_reuse (orb.in (), echo.in (), echo2.in ());
        }

      // Shut down the server
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("Shutting down echo\n")));
      echo->shutdown ();
      if (!CORBA::is_nil (echo2.in ()))
        {
          ACE_DEBUG ((LM_INFO, ACE_TEXT ("Shutting down echo2\n")));
          echo2->shutdown ();
        }

      orb->destroy ();

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("\n=== Overall result: %C ===\n"),
                  all_pass ? "PASS" : "FAIL"));
      return all_pass ? 0 : 1;
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("client exception"));
      return 1;
    }
}
