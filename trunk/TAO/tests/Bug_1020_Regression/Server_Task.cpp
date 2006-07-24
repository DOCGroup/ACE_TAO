/**
 * @file Server_Task.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#include "Server_Task.h"

ACE_RCSID(Bug_1230_Regression, Server_Task, "$Id$")

Server_Task::Server_Task(Test::Echo_ptr echo)
  : echo_(Test::Echo::_duplicate(echo))
{
}

int
Server_Task::svc (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  Test::Payload pload(16); pload.length(16);
  int stop_loop = 0;
  for(int i = 0; i != 1000000 && !stop_loop; ++i)
    {
      ACE_TRY
        {
          Test::Payload_var result =
            this->echo_->echo_payload (pload
			               ACE_ENV_ARG_PARAMETER);
	  ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          stop_loop = 1;
        }
      ACE_ENDTRY;
    }
  return 0;
}
