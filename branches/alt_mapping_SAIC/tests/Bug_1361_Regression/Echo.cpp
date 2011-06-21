/**
 * @file Echo.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#include "Echo.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_signal.h"

Echo::Echo(CORBA::ORB_ptr orb,
           int abort_counter)
  : orb_(CORBA::ORB::_duplicate(orb))
  , abort_counter_(abort_counter)
{
}

void
Echo::echo_payload(Test::Payload const &)
{
  --this->abort_counter_;

  if (this->abort_counter_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Echo::echo_payload, aborting\n"));
      // Kill the app
      ACE::terminate_process (ACE_OS::getpid ());
    }
}

void
Echo::echo_payload_out (Test::Payload_out data)
{
  int j = ACE_OS::rand() % 40000;
  data = new Test::Payload(j);
  data->length(j);
  ACE_OS::memset(data->get_buffer(), 0, data->length());

  --this->abort_counter_;

  if (this->abort_counter_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Echo::echo_payload_out, aborting\n"));
      // Kill the app
      ACE::terminate_process (ACE_OS::getpid ());
    }
}
