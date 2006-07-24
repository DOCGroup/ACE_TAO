/**
 * @file Client_Peer.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#include "Client_Peer.h"
#include "Clock_Ticks.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

ACE_RCSID(Nested_Upcall_Crash, Client_Peer, "$Id$")

class Crasher : public ACE_Event_Handler
{
public:
  Crasher (void);

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
                    Test::Payload const &
                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC((CORBA::SystemException))
{
  // ACE_DEBUG ((LM_DEBUG, "Received call, depth = %d\n", max_depth));
  if (max_depth > 0)
  {
    Test::Peer_var me =
      this->_this(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    Test::Payload return_data;

    callback->callme(me.in(), max_depth - 1, return_data
                     ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }
}

void
Client_Peer::crash(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC((CORBA::SystemException))
{
  Crasher * crasher = new Crasher;

  ACE_Time_Value clk_tck (0, Clock_Ticks::get_usecs_per_tick ());
  ACE_Reactor * reactor = this->orb_->orb_core()->reactor();
  reactor->schedule_timer(crasher, 0, clk_tck);
}

void
Client_Peer::noop(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC((CORBA::SystemException))
{
}

Crasher::Crasher(void)
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
