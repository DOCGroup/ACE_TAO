//
// $Id$
//

#include "Client_Task.h"

Client_Task::Client_Task (const ACE_TCHAR *ior,
                          CORBA::ORB_ptr corb,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
    , input_ (ior)
    , corb_ (CORBA::ORB::_duplicate (corb))
{
}

void
Client_Task::test_system_exception (
  Test::Hello_ptr hello_ptr)
{
  try
    {
      hello_ptr->system_exception_test ();
    }
  catch (const CORBA::INTERNAL& )
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Caught internal exception as expected\n"));
      // ignore
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Unexpected exception caught in test_system_exception:");
      throw;
    }
}

void
Client_Task::test_user_exception_expected (
  Test::Hello_ptr hello_ptr)
{
  try
    {
      hello_ptr->user_exception_expected ();
    }
  catch (const ::Test::Hello::A& )
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Caught user A exception as expected\n"));
      // ignore
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Unexpected exception caught in test_user_exception_expected:");
      throw;
    }
}

void
Client_Task::test_user_exception_not_expected (
  Test::Hello_ptr hello_ptr)
{
  try
    {
      hello_ptr->user_exception_not_expected ();
    }
  catch (const CORBA::UNKNOWN& ex)
    {
      if ((ex.minor() & 0xFFFU) == 1)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Caught unknown exception as expected\n"));
        }
      else
        {
          ex._tao_print_exception (
            "Unexpected exception caught in user_exception_not_expected:");
          throw;
        }
      // ignore
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Unexpected exception caught in user_exception_not_expected:");
      throw;
    }
}

int
Client_Task::svc (void)
{
  try
    {
      CORBA::Object_var tmp = this->corb_->string_to_object (input_);

      Test::Hello_var hello = Test::Hello::_narrow(tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             input_),
                             1);
        }

      CORBA::String_var the_string = hello->get_string ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                  the_string.in ()));

      this->test_system_exception (hello.in ());

      this->test_user_exception_expected (hello.in ());

      this->test_user_exception_not_expected (hello.in ());

      hello->shutdown ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;

}
