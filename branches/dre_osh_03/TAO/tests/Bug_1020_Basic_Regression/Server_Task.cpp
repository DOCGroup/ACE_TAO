#include "Server_Task.h"


ACE_RCSID (Bug_1020_Basic_Regression,
           Server_Task,
           "$Id$")


Server_Task::Server_Task (Test::Echo_ptr echo,
                          size_t sz)
  : echo_ (Test::Echo::_duplicate (echo)),
    sz_ (sz)

{
}

int
Server_Task::svc (void)
{
  Test::Payload payload (32768);
  payload.length (32768);

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Send messages till the remote guy crashes
      for (;;)
        {
          this->echo_->echo_payload (payload
                                     ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
