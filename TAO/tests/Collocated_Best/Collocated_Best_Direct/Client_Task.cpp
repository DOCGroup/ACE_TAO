
#include "Client_Task.h"
#include "TestC.h"

Client_Task::Client_Task (const ACE_TCHAR *ior,
                          CORBA::ORB_ptr corb,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
    , input_ (ior)
    , corb_ (CORBA::ORB::_duplicate (corb))

{
}

int
Client_Task::svc ()
{
  int status = 0;
  try
    {
      CORBA::Object_var tmp =
        this->corb_->string_to_object (input_);

      Test::Hello_var hello =
        Test::Hello::_narrow(tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             input_),
                             1);
        }

      CORBA::String_var the_string =
        hello->get_string ();

      if (ACE_OS::strcmp (the_string.in (), "Hello there!") != 0)
        {
          ACE_ERROR ((LM_ERROR, "(%P|%t) - Error, incorrect string returned <%C>\n",
                      the_string.in ()));
          status = 1;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                      the_string.in ()));
        }

      hello->shutdown ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      status = 1;
    }

  return status;
}
