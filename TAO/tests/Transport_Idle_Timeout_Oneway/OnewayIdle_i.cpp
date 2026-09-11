#include "OnewayIdle_i.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_sys_time.h"
#include "tao/ORB_Core.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Transport_Cache_Manager_T.h"

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

  CORBA::Long
  cache_size (CORBA::ORB_ptr orb)
  {
    TAO_ORB_Core *core = orb->orb_core ();
    return static_cast<CORBA::Long> (
      core->lane_resources ().transport_cache ().current_size ());
  }
}

OnewayIdle_i::OnewayIdle_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
OnewayIdle_i::ping (CORBA::Long wait_seconds)
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("(%P|%t) oneway ping: waiting %d seconds\n"),
              wait_seconds));

  wait_with_reactor (this->orb_.in (), wait_seconds);

  this->observed_cache_size_ = cache_size (this->orb_.in ());

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("(%P|%t) oneway ping: cache size after wait = %d\n"),
              this->observed_cache_size_));
}

CORBA::Long
OnewayIdle_i::observed_cache_size ()
{
  return this->observed_cache_size_;
}

void
OnewayIdle_i::shutdown ()
{
  this->orb_->shutdown (false);
}
