/**
 * @file Echo.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#include "Echo.h"



Echo::Echo(CORBA::ORB_ptr orb)
  : orb_(CORBA::ORB::_duplicate(orb))
{
}

Test::Payload *
Echo::echo_payload(Test::Payload const &)
{
  ACE_Time_Value tick(0, 10000);
  orb_->run(tick);

  Test::Payload_var pload(new Test::Payload);
  pload->length(1024);
  return pload._retn();
}
