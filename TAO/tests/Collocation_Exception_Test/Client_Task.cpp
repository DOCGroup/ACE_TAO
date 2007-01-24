//
// $Id$
//

#include "Client_Task.h"

ACE_RCSID(Collocation_Oneway_Tests, Client_Task, "$Id$")

Client_Task::Client_Task (const char *ior,
			  CORBA::ORB_ptr corb,
			  ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
    , input_ (ior)
    , corb_ (CORBA::ORB::_duplicate (corb))
{
}

void
Client_Task::test_system_exception (
  Test::Hello_ptr hello_ptr ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      hello_ptr->system_exception_test ();
    }
  ACE_CATCH (CORBA::INTERNAL, ex)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Caught internal exception as expected\n"));
      // ignore
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
         "Unexpected exception caught in test_system_exception:");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
}

void
Client_Task::test_user_exception_expected (
  Test::Hello_ptr hello_ptr ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      hello_ptr->user_exception_expected ();
    }
  ACE_CATCH (::Test::Hello::A, ex)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Caught user A exception as expected\n"));
      // ignore
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
         "Unexpected exception caught in test_user_exception_expected:");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
}

void
Client_Task::test_user_exception_not_expected (
  Test::Hello_ptr hello_ptr ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      hello_ptr->user_exception_not_expected ();
    }
  ACE_CATCH (CORBA::UNKNOWN, ex)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Caught unknown exception as expected\n"));
      // ignore
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
         "Unexpected exception caught in user_exception_not_expected:");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
}

int
Client_Task::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      CORBA::Object_var tmp =
        this->corb_->string_to_object (input_
				       ACE_ENV_ARG_PARAMETER);

      Test::Hello_var hello =
        Test::Hello::_narrow(tmp.in () ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             input_),
                             1);
        }

      CORBA::String_var the_string =
        hello->get_string ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%s>\n",
		  the_string.in ()));

      this->test_system_exception (hello.in () ACE_ENV_ARG_PARAMETER);

      this->test_user_exception_expected (hello.in () ACE_ENV_ARG_PARAMETER);

      this->test_user_exception_not_expected (hello.in () ACE_ENV_ARG_PARAMETER);

      hello->shutdown ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
         "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;

}
