//
// $Id$
//

#include "Client_Task.h"
#include "testC.h"
#include "interceptors.h"

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
  ACE_TRY_NEW_ENV
    {

      CORBA::Object_var object =
        corb_->string_to_object (input_ ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test_Interceptors::Visual_var server =
        Test_Interceptors::Visual::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             this->input_),
                            1);
        }

      run_test (server.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught in client task:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;

}


void
Client_Task::run_test (Test_Interceptors::Visual_ptr server
          ACE_ENV_ARG_DECL)
{
  server->normal (10 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Long one = 1, two = 1;
  (void) server->calculate (one,
                            two
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_TRY
    {
      (void) server->user (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (Test_Interceptors::Silly, userex)
    {
      ACE_DEBUG ((LM_DEBUG, "As expected, Caught Silly\n"));
    }
  ACE_ENDTRY;
  ACE_CHECK;

  ACE_TRY_EX (SYS)
    {
      server->system (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (SYS);
    }
  ACE_CATCH (CORBA::INV_OBJREF, sysex)
    {
      ACE_DEBUG ((LM_DEBUG, "As expected, Caught CORBA::INV_OBJREF\n"));
    }
  ACE_ENDTRY;
  ACE_CHECK;
}
