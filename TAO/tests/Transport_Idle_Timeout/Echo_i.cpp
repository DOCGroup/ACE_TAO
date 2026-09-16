#include "Echo_i.h"
#include "tao/debug.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include "ace/High_Res_Timer.h"
#include "tao/ORB_Core.h"
#include "tao/Transport_Cache_Manager_T.h"
#include "tao/Thread_Lane_Resources.h"

/// Run the servant ORB reactor while delaying synchronous completion.
void
sleep_with_reactor (CORBA::ORB_ptr orb, int seconds)
{
  ACE_Time_Value const deadline = ACE_High_Res_Timer::gettimeofday_hr ()
    + ACE_Time_Value (seconds);

  while (ACE_High_Res_Timer::gettimeofday_hr () < deadline)
    {
      ACE_Time_Value tv (0, 50000); // 50 ms slices
      orb->perform_work (tv);
    }
}

size_t
cache_size(CORBA::ORB_ptr orb)
{
  TAO_ORB_Core *core = orb->orb_core ();
  return core->lane_resources ().transport_cache ().current_size ();
}

bool
check (const char *label, size_t got, size_t expected)
{
  if (got == expected)
    {
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("(%P|%t)  [PASS] %C: cache_size = %B (expected %B)\n"),
                  label, got, expected));
      return true;
    }
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("(%P|%t)  [FAIL] %C: cache_size = %B (expected %B)\n"),
              label, got, expected));
  return false;
}

Echo_i::Echo_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

bool
Echo_i::ping (::CORBA::Long sleep_time, ::CORBA::Long cache_size_expected, ::Test::Echo_ptr server, ::CORBA::Long sleep_time_server, ::CORBA::Long cache_size_expected_in_server1, ::CORBA::Long cache_size_expected_in_server2)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Echo_i::ping, sleep time (%d), cache size expected (%d)\n"), sleep_time, cache_size_expected));

  bool ok = true;
  sleep_with_reactor (this->orb_, sleep_time);

  ok &= check ("ping", cache_size(this->orb_), cache_size_expected);

  if (!CORBA::is_nil(server))
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) Echo_i::ping, pinging second server with sleep %d\n"), sleep_time_server));
    ok &= server->ping (sleep_time_server, cache_size_expected_in_server2, Test::Echo::_nil(), 0, 1, 0);
    if (sleep_time_server > 0)
      {
        sleep_with_reactor (this->orb_, sleep_time_server);
      }
    ok &= check ("ping_second", cache_size(this->orb_), cache_size_expected_in_server1);
  }
  return ok;
}

void
Echo_i::shutdown ()
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Echo_i::shutdown ()\n")));
  orb_->shutdown (false);
}
