//
// $Id$
//

#include "Client_Task.h"

ACE_RCSID(Stack_Recursion,
          Client_Task,
          "$Id$")

Client_Task::Client_Task (Test::Sender_ptr sender,
                          CORBA::Long event_count,
                          CORBA::ULong event_size,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
  , sender_ (Test::Sender::_duplicate (sender))
  , event_count_ (event_count)
  , event_size_ (event_size)
{
}

int
Client_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting client task\n"));

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->validate_connections (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      for (int i = 0; i != this->event_count_; ++i)
        {
          if (i%100 == 0)
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) In iteration [%d] ....\n",
                        i));
          Test::Payload_var pl = new Test::Payload;
          Test::Payload_out payload (pl.out ());
          this->sender_->get_data (this->event_size_,
                                   payload
                                   ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client task finished\n"));
  return 0;
}


void
Client_Task::validate_connections (ACE_ENV_SINGLE_ARG_DECL)
{
  for (int i = 0 ; i != 100; i++)
    {
      ACE_TRY
        {
          this->sender_->ping (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

        }
      ACE_CATCHANY {} ACE_ENDTRY;
    }
}
