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

  // Make the connections ..
  this->validate_connection ();

  ACE_DECLARE_NEW_CORBA_ENV;

  // Now get the big replies..
  ACE_TRY
    {
      for (int i = 0; i != this->event_count_; ++i)
        {
          this->reply_gen_->get_big_reply (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
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

void
Client_Task::validate_connection (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Validating connection ..\n"));

  ACE_DECLARE_NEW_CORBA_ENV;

  // Try to setup a connection to the remote server, ignoring all the
  // exceptions  that are expected (see bug 189 on why it is so). We
  // could use a a validate_connection for it . But we wantthis test
  // to work with Minimum CORBA builds too..
  for (int i = 0; i != 100; ++i)
    {
      ACE_TRY
        {

          this->reply_gen_->ping (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
        ACE_CATCHANY {}
        ACE_ENDTRY;
    }
}
