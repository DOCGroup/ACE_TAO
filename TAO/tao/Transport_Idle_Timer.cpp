#include "tao/Transport_Idle_Timer.h"
#include "tao/debug.h"
#include "ace/Reactor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  Transport_Idle_Timer::Transport_Idle_Timer (Transport_Idle_Timer_Handler *handler)
    : handler_ (handler)
  {
  }

  int
  Transport_Idle_Timer::handle_timeout (ACE_Time_Value const &, void const *)
  {
    try
      {
        this->handler_->scan_idle_transports ();
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
