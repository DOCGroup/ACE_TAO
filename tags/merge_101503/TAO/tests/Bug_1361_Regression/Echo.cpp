/**
 * @file Echo.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#include "Echo.h"

ACE_RCSID(Bug_1270_Regression, Echo, "$Id$")

Echo::Echo(CORBA::ORB_ptr orb,
           int abort_counter)
  : orb_(CORBA::ORB::_duplicate(orb))
  , abort_counter_(abort_counter)
{
}

void
Echo::echo_payload(Test::Payload const &
                   TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC((CORBA::SystemException))
{
  this->abort_counter_--;

  if (this->abort_counter_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Echo::echo_payload, aborting\n"));
      // Kill the app
      raise(9);
    }
}

void
Echo::echo_payload_out (
                        Test::Payload_out data
                        ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  int j = ACE_OS::rand() % 40000;
  data = new Test::Payload(j); 
  data->length(j);
  ACE_OS::memset(data->get_buffer(), data->length(), 0);

  this->abort_counter_--;

  if (this->abort_counter_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Echo::echo_payload, aborting\n"));
      // Kill the app
      raise(9);
    }

}
