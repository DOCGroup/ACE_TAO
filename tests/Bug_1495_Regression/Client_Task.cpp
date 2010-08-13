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
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_stat.h"

Client_Task::Client_Task (const ACE_TCHAR *input,
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
      // The test was designed so that we have to wait here.
      ACE_stat st;
      int timeout = 10;
      while (--timeout)
        {
          if (ACE_OS::stat (this->input_, &st) == 0 &&
              st.st_size != 0)
            {
              break;
            }

          ACE_OS::sleep (1);
        }

      ACE_TString infile = ACE_TString (ACE_TEXT("file://")) + this->input_;

      CORBA::Object_var object =
        this->corb_->string_to_object (infile.c_str ());

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
                      ACE_TEXT ("Remote thread ID was ")
                      ACE_INT64_FORMAT_SPECIFIER
                      ACE_TEXT ("\n"),
                      remote_thread_id));

          ThreadId const mythread_id =
            ACE_thread_t_to_integer<ThreadId> (ACE_Thread::self ());

          if (mythread_id != remote_thread_id)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("ERROR:  Failed Bug_1495_Regression test. ")
                          ACE_TEXT ("Expected thread id was ")
                          ACE_INT64_FORMAT_SPECIFIER
                          ACE_TEXT (", received ")
                          ACE_INT64_FORMAT_SPECIFIER
                          ACE_TEXT (".\n"),
                          mythread_id,
                          remote_thread_id));
            }
          else
            {
              ACE_DEBUG ((LM_INFO,
                          ACE_TEXT ("Passed Bug_1495_Regression test.\n")
                          ACE_TEXT ("Local thread id was ")
                          ACE_INT64_FORMAT_SPECIFIER
                          ACE_TEXT (", received ")
                          ACE_INT64_FORMAT_SPECIFIER
                          ACE_TEXT (".\n"),
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
