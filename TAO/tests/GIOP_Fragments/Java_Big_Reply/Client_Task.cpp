//
// $Id$
//

#include "Client_Task.h"

ACE_RCSID(Muxing, Client_Task, "$Id$")

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

  ACE_DECLARE_NEW_CORBA_ENV;

  // Now get the big replies..
  ACE_TRY
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
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception Caught \n");
      return -1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client task finished\n"));
  return 0;
}
