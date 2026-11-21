
#include "Client_Task.h"

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
Client_Task::svc ()
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting client task\n"));
  Test::Payload payload (this->event_size_);
  payload.length (this->event_size_);

  for (CORBA::ULong j = 0; j != payload.length (); ++j)
    payload[j] = static_cast<CORBA::Octet> (j % 256);

  try
    {
      for (CORBA::Long i {}; i != this->event_count_; ++i)
        {
          this->receiver_->receive_data (payload);
        }
    }
  catch (CORBA::Exception const &)
    {
      return -1;
    }
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client task finished\n"));
  return 0;
}
