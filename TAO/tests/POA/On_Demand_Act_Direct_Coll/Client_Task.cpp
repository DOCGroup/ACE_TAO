// $Id$

#include "Client_Task.h"
#include "testC.h"

Client_Task::Client_Task (const ACE_TCHAR *ior,
                          CORBA::ORB_ptr corb,
                          ACE_Thread_Manager *thr_mgr,
                          CORBA::Boolean exception)
  : ACE_Task_Base (thr_mgr)
    , input_ (ior)
    , corb_ (CORBA::ORB::_duplicate (corb))
    , except_ (exception)
{
}

int
Client_Task::svc (void)
{
  int status = 0;
  try
    {
      // Get an object reference from the argument string.
      CORBA::Object_var tmp =
        this->corb_->string_to_object (input_);

      // Try to narrow the object reference to a test reference.
      test_var test = test::_narrow (tmp.in ());
      if (CORBA::is_nil (test.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                               "Nil Test::Hello reference <%s>\n",
                               input_),
                               1);
         }

      CORBA::String_var ior =
        this->corb_->object_to_string (test.in ());

      ACE_DEBUG ((LM_DEBUG,
                   "\nConnecting to: %C\n\n",
                   ior.in ()));
      try
        {
           CORBA::String_var the_string = test->get_string ();
          if (ACE_OS::strcmp (the_string.in (), "Hello there!") != 0)
             {
               ACE_ERROR ((LM_ERROR, "(%P|%t) - Error, incorrect string returned <%C>\n",
               the_string.in ()));
             }
           else
             {
               ACE_DEBUG ((LM_DEBUG, "(%P|%t) - OK , correct string returned <%C>\n",
                 the_string.in ()));
               }
          test->shutdown ();
        }
      catch (CORBA::INTERNAL)
         {
           if (this->except_)
             {
               ACE_DEBUG ((LM_DEBUG,
                   "OK: Client_Task Expected exception with"
                   "direct collocation received\n"));
                  status = 0;
             }
           else
             {
               ACE_ERROR ((LM_ERROR,
                    "ERROR: Client_Task Unxpected exception with"
                    "thru_poa collocation received\n"));
                   status = 1;
             }
         }
      corb_->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      status = 1;
    }

  return status;

}
