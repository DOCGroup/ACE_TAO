/**
 * @file Echo.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#include "Echo.h"

ACE_RCSID(Notify_Crash, Echo, "$Id$")

Echo::Echo(CORBA::ORB_ptr orb)
  : orb_(CORBA::ORB::_duplicate(orb))
{
}

Test::Payload *
Echo::echo_payload(Test::Payload const &
                   TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC((CORBA::SystemException))
{
  ACE_Time_Value tick(0, 10000);
  orb_->run(tick);

  Test::Payload_var pload(new Test::Payload);
  pload->length(1024);
  return pload._retn();
}
