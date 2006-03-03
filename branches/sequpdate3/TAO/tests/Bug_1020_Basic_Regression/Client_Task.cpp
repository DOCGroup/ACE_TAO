//
// $Id$
//

#include "Client_Task.h"

ACE_RCSID(Muxing, Client_Task, "$Id$")

Client_Task::Client_Task (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Client_Task::svc (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
