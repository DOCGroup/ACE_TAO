//
// $Id$
//

#include "Client_Task.h"
#include "tao/ORB_Core.h"
#include "TestC.h"

ACE_RCSID(Muxing, Client_Task, "$Id$")

Client_Task::Client_Task (const char *ior,
                          CORBA::ORB_ptr corb,
                          ACE_Thread_Manager *thr_mgr,
                          CORBA::Boolean result)
  : ACE_Task_Base (thr_mgr)
    , input_ (ior)
    , corb_ (CORBA::ORB::_duplicate (corb))
    , result_ (result)
{
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

      if (!this->result_)
        {
          // We would expect the call to fail, so we have an error now
          ACE_ERROR ((LM_ERROR, "(%P|%t) - ERROR: get_string should have failed\n"));
        }

      hello->shutdown ();
    }
  ACE_CATCH (CORBA::TRANSIENT, ex)
    {
      if (!this->result_)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - caught expected exception\n"));
          // When collocation has been disabled we expect a transient
          // with minor code 2
          return 0;
        }

      ACE_PRINT_EXCEPTION (ex, "Caught exception:");
      return 1;
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
