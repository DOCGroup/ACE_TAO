#include "Client_Task.h"
#include "testC.h"
#include <cstring>

Client_Task::Client_Task (ACE_TCHAR const *ior,
                          CORBA::ORB_ptr corb,
                          ACE_Thread_Manager *thr_mgr,
                          CORBA::Boolean except)
  : ACE_Task_Base (thr_mgr)
  , input_ (ior)
  , corb_ (CORBA::ORB::_duplicate (corb))
  , except_ (except)
{
}

int
Client_Task::svc ()
{
  int status {};
  try
    {
      // Get an object reference from the argument string.
      CORBA::Object_var const tmp =
        this->corb_->string_to_object (input_);

      // Try to narrow the object reference to a test reference.
      test_var const test = test::_narrow (tmp.in ());
      if (CORBA::is_nil (test.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Nil Test::Hello reference <%s>\n",
                           this->input_),
                          1);

      CORBA::String_var const ior =
        this->corb_->object_to_string (test.in ());

      ACE_DEBUG ((LM_DEBUG, "\nConnecting to: %C\n\n", ior.in ()));
      try
        {
          CORBA::String_var const the_string = test->get_string ();
          if (std::strcmp (the_string.in (), "Hello there!") != 0)
            ACE_ERROR ((LM_ERROR,
                        "(%P|%t) - Error, incorrect string returned <%C>\n",
                        the_string.in ()));
          else
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) - OK, correct string returned <%C>\n",
                        the_string.in ()));
        }
      catch (CORBA::INTERNAL const &)
        {
          if (this->except_)
            ACE_DEBUG ((LM_DEBUG,
                        "OK: Client_Task expected exception with "
                        "direct collocation received\n"));
          else
            {
              ACE_ERROR ((LM_ERROR,
                          "ERROR: Client_Task unexpected exception with "
                          "thru_poa collocation received\n"));
              status = 1;
            }
          this->corb_->destroy ();
          return status;
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      status = 1;
    }
  this->corb_->destroy ();
  return status;
}
