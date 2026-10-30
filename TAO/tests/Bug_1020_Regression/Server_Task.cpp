/**
 * @file Server_Task.cpp
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#include "Server_Task.h"


Server_Task::Server_Task(Test::Echo_ptr echo)
  : echo_(Test::Echo::_duplicate(echo))
{
}

int
Server_Task::svc ()
{
  Test::Payload pload(16); pload.length(16);
  bool stop_loop = false;
  for (int i = 0; i != 1000000 && !stop_loop; ++i)
    {
      try
        {
          Test::Payload_var result =
            this->echo_->echo_payload (pload);
        }
      catch (CORBA::Exception const &)
        {
          stop_loop = true;
        }
    }
  return 0;
}
