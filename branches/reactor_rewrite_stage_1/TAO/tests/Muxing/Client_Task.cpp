//
// $Id$
//

#include "Client_Task.h"

ACE_RCSID(Muxing, Client_Task, "$Id$")

Client_Task::Client_Task (Test::Receiver_ptr receiver,
                          CORBA::Long event_count,
                          CORBA::ULong event_size,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
  , receiver_ (Test::Receiver::_duplicate (receiver))
  , event_count_ (event_count)
  , event_size_ (event_size)
{
}

int
Client_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting client task\n"));
  Test::Payload payload (this->event_size_);
  payload.length (this->event_size_);

  for (CORBA::ULong j = 0; j != payload.length (); ++j)
    payload[j] = (j % 256);

  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      for (int i = 0; i != this->event_count_; ++i)
        {
          this->receiver_->receive_data (payload TAO_ENV_ARG_PARAMETER);
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
