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

  // Make the connections ..
  this->validate_connection ();

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting big reply\n"));


  // Now get the big replies..
  try
    {
      for (int i = 0; i != this->event_count_; ++i)
        {
          Test::Octet_Seq_var dummy =
          this->reply_gen_->get_big_reply ();
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

void
Client_Task::validate_connection (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Validating connection ..\n"));


  // Try to setup a connection to the remote server, ignoring all the
  // exceptions that are expected (see bug 189 on why it is so). We
  // could use a a validate_connection for it . But we want this test
  // to work with Minimum CORBA builds too..
  for (int i = 0; i != 100; ++i)
    {
      try
        {
          this->reply_gen_->ping ();
        }
      catch (const CORBA::Exception&)
        {
        }
    }
}
