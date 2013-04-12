/**
 * @file Server_Task.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#include "Server_Task.h"



Server_Task::Server_Task(Test::Echo_ptr echo)
  : echo_(Test::Echo::_duplicate(echo))
{
}

int
Server_Task::svc (void)
{

  Test::Payload pload(16); pload.length(16);
  int stop_loop = 0;
  for(int i = 0; i != 1000000 && !stop_loop; ++i)
    {
      try
        {
          Test::Payload_var result =
            this->echo_->echo_payload (pload);
        }
      catch (const CORBA::Exception&)
        {
          stop_loop = 1;
        }
    }
  return 0;
}
