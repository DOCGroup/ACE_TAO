//
// $Id$
//

#include "Client_Task.h"
#include "tao/Stub.h"

Client_Task::Client_Task (Test::Hello_ptr receiver,
                          CORBA::ORB_ptr o,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
    , receiver_ (Test::Hello::_duplicate (receiver))
    , handler_ (o)
    , handler_var_ ()
    , orb_ (CORBA::ORB::_duplicate (o))
{
  this->handler_var_ = this->handler_._this ();
}

int
Client_Task::svc (void)
{
  // ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting client task\n"));

  try
    {
      for (CORBA::Long j = 0;
           j != 1000;
           ++j)
        {
          // Send 25 messages in every loop..
          for (CORBA::Short i = 0; i != 25; ++i)
            {
              this->receiver_->sendc_next_prime (this->handler_var_.in ());
            }

          CORBA::Short repl = 0;
          while (repl != 25)
            {
              CORBA::Boolean pending =
                this->orb_->work_pending ();

              if (pending)
                {
                  this->orb_->perform_work ();

                  ++repl;
                }
            }
        }
    }
  catch (const CORBA::Exception&)
    {
      return -1;
    }
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client task finished\n"));
  return 0;
}
