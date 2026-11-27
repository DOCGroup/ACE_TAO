#include "tao/Transport_Timer.h"
#include "tao/Transport.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  Transport_Timer::Transport_Timer (TAO_Transport *transport)
    : transport_ (transport)
  {
  }

  int
  Transport_Timer::handle_timeout (ACE_Time_Value const &current_time, void const *act)
  {
    return this->transport_->handle_timeout (current_time, act);
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
