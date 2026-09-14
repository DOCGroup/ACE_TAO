#include "tao/Transport_Idle_Timer.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "ace/Reactor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  Transport_Idle_Timer::Transport_Idle_Timer (TAO_Thread_Lane_Resources *resources)
    : resources_ (resources)
  {
  }

  int
  Transport_Idle_Timer::handle_timeout (const ACE_Time_Value &, const void*)
  {
    try
      {
        this->resources_->scan_idle_transports ();
      }
    catch (...)
      {
        TAOLIB_ERROR ((LM_ERROR,
          ACE_TEXT ("TAO (%P|%t) - Transport idle scan failed; retrying next interval\n")));
      }
    return 0;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
