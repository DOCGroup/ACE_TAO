//
// $Id$
//

#include "Client_Task.h"

Client_Task::Client_Task (Test::Big_Reply_ptr reply_gen,
                          int event_count,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
  , reply_gen_ (Test::Big_Reply::_duplicate (reply_gen))
  , event_count_ (event_count)

{
}

int
Client_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting client task\n"));


  // Now get the big replies..
  try
    {
      for (int i = 0; i != this->event_count_; ++i)
        {
          Test::Octet_Seq_var seq =
              this->reply_gen_->get_big_reply ();

          CORBA::ULong length = seq->length ();
          for(CORBA::ULong i = 0; i < length; ++i)
            {
              if (seq[i] != 'A')
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "Invalid data '%c' at %d\n", seq[i], i),
                                    -1);
                }
            }

//          ACE_Time_Value tv(0, 10000000);
//          ACE_OS::sleep(tv);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception Caught\n");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client task finished\n"));
  return 0;
}
