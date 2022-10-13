#include "Server_Task.h"

Server_Task::Server_Task (Test::Echo_ptr echo)
  : echo_ (Test::Echo::_duplicate (echo))
{
}

int
Server_Task::svc ()
{
  Test::Payload payload (32768);
  payload.length (32768);

  try
    {
      // Send messages till the remote guy crashes
      for (;;)
        {
          this->echo_->echo_payload (payload);
        }
    }
  catch (const CORBA::Exception&)
    {
      return -1;
    }

  ACE_NOTREACHED (return 0);
}
