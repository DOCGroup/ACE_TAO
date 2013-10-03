//
// $Id$
//

#include "Client_Task.h"

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

  try
    {
      this->validate_connections ();

      for (int i = 0; i != this->event_count_; ++i)
        {
#if 0
          if (i%100 == 0)
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) In iteration [%d] ....\n",
                        i));
#endif /*if 0*/
          Test::Payload_var pl;
          this->sender_->get_data (this->event_size_, pl.out ());
        }
    }
  catch (const CORBA::Exception&)
    {
      return -1;
    }
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client task finished\n"));
  return 0;
}


void
Client_Task::validate_connections (void)
{
  for (int i = 0 ; i != 100; i++)
    {
      try
        {
          this->sender_->ping ();

        }
      catch (const CORBA::Exception&){}
    }
}
