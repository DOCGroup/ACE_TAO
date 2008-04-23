/**
 * @file Client_Task.cpp
 * @author Will Otte <wotte@dre.vanderbilt.edu
 *
 * $Id$
 *
 * Implements the Client_Task class defined in Client_Task.h.
 */

#include "Client_Task.h"
#include "testC.h"
#include "Client_ORBInitializer.h"
#include "tid_to_int.h"


Client_Task::Client_Task (const char *input,
                          CORBA::ORB_ptr corb,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr),
    input_ (input),
    corb_ (CORBA::ORB::_duplicate (corb))
{}


int
Client_Task::svc (void)
{
  try
    {
      CORBA::Object_var object =
        this->corb_->string_to_object (this->input_);

      Bug1495_Regression::Bug1495_var server =
        Bug1495_Regression::Bug1495::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "Object reference <%s> is nil\n",
                      input_));

          return 1;
        }

      // Try multiple calls to see if we keep being forwarded
      for (int i = 0; i < 5; ++i)
        {
          using Bug1495_Regression::ThreadId;

          // call the thread_id function on the test object
          ThreadId remote_thread_id;

          server->get_thread_id (remote_thread_id);

          ACE_DEBUG ((LM_INFO,
                      "Remote thread ID was"
                      ACE_INT64_FORMAT_SPECIFIER
                      "\n",
                      remote_thread_id));

          ThreadId const mythread_id =
            ACE_thread_t_to_integer<ThreadId> (ACE_Thread::self ());

          if (mythread_id != remote_thread_id)
            {
              ACE_ERROR ((LM_ERROR,
                          "ERROR:  Failed Bug_1495_Regression test. "
                          "Expected thread id was"
                          ACE_INT64_FORMAT_SPECIFIER
                          ", received"
                          ACE_INT64_FORMAT_SPECIFIER
                          ".\n",
                          mythread_id,
                          remote_thread_id));
            }
          else
            {
              ACE_DEBUG ((LM_INFO,
                          "Passed Bug_1495_Regression test.\n"
                          "Local thread id was "
                          ACE_INT64_FORMAT_SPECIFIER
                          ", received "
                          ACE_INT64_FORMAT_SPECIFIER
                          ".\n",
                          mythread_id,
                          remote_thread_id));
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception in client task:");
      return 1;
    }

  return 0;
}
