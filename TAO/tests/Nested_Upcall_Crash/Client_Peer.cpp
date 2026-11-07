/**
 * @file Client_Peer.cpp
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#include "Client_Peer.h"
#include "Clock_Ticks.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

class Crasher : public ACE_Event_Handler
{
public:
  Crasher () = default;
  Crasher (Crasher const &) = delete;
  Crasher (Crasher &&) = delete;
  Crasher &operator= (Crasher const &) = delete;
  Crasher &operator= (Crasher &&) = delete;

  int handle_timeout (ACE_Time_Value const &current_time,
                      void const *arg) override;
};

Client_Peer::Client_Peer (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Client_Peer::callme (Test::Peer_ptr callback,
                     CORBA::ULong max_depth,
                     Test::Payload const &)
{
  if (max_depth > 0)
    {
      Test::Peer_var const me = this->_this ();
      Test::Payload return_data;
      callback->callme (me.in (), max_depth - 1, return_data);
    }
}

void
Client_Peer::crash ()
{
  // The handler must stay alive until its timer fires and aborts the process.
  Crasher *const crasher = new Crasher;
  ACE_Time_Value const clk_tck (0, Clock_Ticks::get_usecs_per_tick ());
  ACE_Reactor *const reactor = this->orb_->orb_core ()->reactor ();
  reactor->schedule_timer (crasher, 0, clk_tck);
}

void
Client_Peer::noop ()
{
}

int
Crasher::handle_timeout (ACE_Time_Value const &,
                         void const *)
{
  ACE_OS::abort ();
  return 0;
}
