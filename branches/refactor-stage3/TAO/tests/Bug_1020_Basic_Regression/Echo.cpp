#include "Echo.h"

ACE_RCSID(Bug_1269_Regression, Echo, "$Id$")

Echo::Echo(CORBA::ORB_ptr orb,
           int abort_counter)
  : orb_(CORBA::ORB::_duplicate(orb))
    , abort_counter_ (abort_counter),
    mutex_ ()
{
}

void
Echo::echo_payload (Test::Payload &
                    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_MT (ACE_GUARD (ACE_SYNCH_MUTEX,
                     ace_mon,
                     this->mutex_));

  this->abort_counter_--;

  if (this->abort_counter_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Echo::echo_payload, sleeping\n"));

      // Sleep for 10 seconds, forcing a flow control of some kind.
      ACE_OS::sleep (5);

      // Run the ORB for a while, to generate a short-lived release of
      // the flow control.
      /*ACE_Time_Value tv (0, 10000);
      this->orb_->run (tv);*/

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Echo::echo_payload, aborting\n"));

      // Kill the app
      ACE_OS::abort();
    }
}
