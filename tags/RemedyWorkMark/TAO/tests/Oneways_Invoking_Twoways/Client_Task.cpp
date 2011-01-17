//
// $Id$
//

#include "Client_Task.h"
#include "ace/OS_NS_unistd.h"

Client_Task::Client_Task (Test::Sender_ptr reply_gen,
                          Test::Receiver_ptr us,
                          ACE_Thread_Manager *thr_mgr,
                           Receiver_i * receiver_impl)
  : ACE_Task_Base (thr_mgr)
  , sender_(Test::Sender::_duplicate (reply_gen))
  , us_ (Test::Receiver::_duplicate (us))
  , receiver_impl_ (receiver_impl)

{
}

int
Client_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting client task\n"));

  try
    {
      for (int i = 0; i != 1; ++i)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) sending oneways...\n"));

          this->sender_->send_ready_message (this->us_.in ());
        }

      // sleeps are evil but 1 sec or so is an improvement on the minute plus
      // this poorly implemented test used to take
      while (this->receiver_impl_->no_calls_ < 10)
        ACE_OS::sleep (1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught Exception");
      return -1;
    }
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client task finished\n"));
  return 0;
}
