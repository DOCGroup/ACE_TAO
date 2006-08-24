//
// $Id$
//

#include "Client_Task.h"
#include "tao/ORB_Core.h"
#include "TestC.h"

ACE_RCSID(Muxing, Client_Task, "$Id$")

Client_Task::Client_Task (const char *ior,
			  CORBA::ORB_ptr corb,
			  ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
    , input_ (ior)
    , corb_ (CORBA::ORB::_duplicate (corb))

{
}

int
Client_Task::svc (void)
{
  CORBA::Boolean collocation =
    this->corb_->orb_core()->optimize_collocation_objects ();

  ACE_TRY_NEW_ENV
    {
      CORBA::Object_var tmp =
        this->corb_->string_to_object (input_
				       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Hello_var hello =
        Test::Hello::_narrow(tmp.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             input_),
                             1);
        }

      CORBA::String_var the_string =
        hello->get_string (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%s>\n",
		  the_string.in ()));

      hello->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::TRANSIENT, ex)
    {
      if (!collocation && (ex.minor() & 0xFFFU) == 2)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - caught expected exception\n"));
          // When collocation has been disabled we expect a trancient
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
