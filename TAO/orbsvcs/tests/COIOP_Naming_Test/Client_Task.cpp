#include "Client_Task.h"
#include "tao/ORB_Core.h"
#include "TestC.h"

Client_Task::Client_Task (CosNaming::NamingContext_ptr root_context,
                          ACE_Thread_Manager *thr_mgr,
                          bool expected_success)
  : ACE_Task_Base (thr_mgr)
  , root_context_ (CosNaming::NamingContext::_duplicate (root_context))
  , expected_success_ (expected_success)
{
}

int
Client_Task::svc ()
{
  try
    {
      CosNaming::Name name {};
      name.length (1);
      name[0].id = CORBA::string_dup ("Hello");

      CORBA::Object_var const tmp = root_context_->resolve (name);

      Test::Hello_var const hello = Test::Hello::_narrow (tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Nil Test::Hello reference\n")),
                            1);
        }

      CORBA::String_var const the_string =
        hello->get_string ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) - string returned <%C>\n"),
                  the_string.in ()));

      if (!this->expected_success_)
        {
          // We would expect the call to fail, so we have an error now
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) - ERROR: get_string should have failed\n")));
        }

      hello->shutdown ();
    }
  catch (CORBA::TRANSIENT const & ex)
    {
      if (!this->expected_success_)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) - caught expected exception\n")));
          // When collocation has been disabled we expect a transient
          // with minor code 2
          return 0;
        }

      ex._tao_print_exception ("Caught exception:");
      return 1;
    }
  catch (CORBA::Exception const & ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
