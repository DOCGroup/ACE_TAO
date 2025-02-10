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
  Crasher ();

  virtual int handle_timeout (ACE_Time_Value const & current_time,
                              void const * arg);
};

Client_Peer::Client_Peer (CORBA::ORB_ptr orb)
  : orb_(CORBA::ORB::_duplicate(orb))
{
}

void
Client_Peer::callme(Test::Peer_ptr callback,
                    CORBA::ULong max_depth,
                    Test::Payload const &)
{
  // ACE_DEBUG ((LM_DEBUG, "Received call, depth = %d\n", max_depth));
  if (max_depth > 0)
  {
    Test::Peer_var me = this->_this ();

    Test::Payload return_data;

    callback->callme(me.in(), max_depth - 1, return_data);
  }
}

void
Client_Peer::crash()
{
  Crasher * crasher = new Crasher;

  ACE_Time_Value clk_tck (0, Clock_Ticks::get_usecs_per_tick ());
  ACE_Reactor * reactor = this->orb_->orb_core()->reactor();
  reactor->schedule_timer(crasher, 0, clk_tck);
}

void
Client_Peer::noop()
{
}

Crasher::Crasher()
{
}

int
Crasher::handle_timeout (ACE_Time_Value const & ,
                       void const *)
{
  // ACE_DEBUG ((LM_DEBUG, "(%P|%t) Performing intentional crash
  // %a\n"));
  ACE_OS::abort();
  return 0;
}
