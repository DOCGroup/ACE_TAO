// $Id$

#include "Client_Task.h"
#include "testC.h"
#include "interceptors.h"

Client_Task::Client_Task (const ACE_TCHAR *ior,
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
  try
    {
      CORBA::Object_var object =
        corb_->string_to_object (input_);

      Test_Interceptors::Visual_var server =
        Test_Interceptors::Visual::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             this->input_),
                            1);
        }

      run_test (server.in ());

      server->shutdown ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in client task:");
      return 1;
    }

  return 0;

}


void
Client_Task::run_test (Test_Interceptors::Visual_ptr server)
{
  server->normal (10);

  CORBA::Long one = 1, two = 1;
  (void) server->calculate (one,
                            two);

  try
    {
      (void) server->user ();
    }
  catch (const Test_Interceptors::Silly&)
    {
      ACE_DEBUG ((LM_DEBUG, "Caught Silly\n"));
    }

  try
    {
      server->system ();
    }
  catch (const CORBA::INV_OBJREF&)
    {
      ACE_DEBUG ((LM_DEBUG, "Caught CORBA::INV_OBJREF\n"));
    }
}
