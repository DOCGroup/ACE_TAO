#include "OnewayIdle_i.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Transport_Cache_Manager_T.h"

namespace
{
  size_t
  cache_size (CORBA::ORB_ptr orb)
  {
    TAO_ORB_Core *core = orb->orb_core ();
    return core->lane_resources ().transport_cache ().current_size ();
  }
}

OnewayIdle_i::OnewayIdle_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
OnewayIdle_i::ping ()
{
  size_t const size = cache_size (this->orb_.in ());

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("(%P|%t) oneway ping: transport cache size = %B\n"),
              size));

  if (size != 1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) ERROR: expected transport cache size 1, got %B\n"),
                  size));
      this->test_failed_ = true;
    }
}

bool
OnewayIdle_i::test_failed () const
{
  return this->test_failed_;
}
