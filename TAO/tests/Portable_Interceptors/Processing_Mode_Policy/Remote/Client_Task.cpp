#include "Client_Task.h"
#include "testC.h"
#include "client_interceptor.h"

Client_Task::Client_Task (ACE_TCHAR const *ior,
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
  try
    {
      CORBA::Object_var object =
        corb_->string_to_object (this->input_.c_str ());

      Test_Interceptors::Visual_var server =
        Test_Interceptors::Visual::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             this->input_.c_str ()),
                            1);
        }

      run_test (server.in ());

      server->shutdown ();
    }
  catch (CORBA::Exception const &ex)
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

  CORBA::Long const one (1);
  CORBA::Long const two (1);
  (void) server->calculate (one, two);

  try
    {
      (void) server->user ();
    }
  catch (Test_Interceptors::Silly const &)
    {
      ACE_DEBUG ((LM_DEBUG, "As expected, Caught Silly\n"));
    }

  try
    {
      server->system ();
    }
  catch (CORBA::INV_OBJREF const &)
    {
      ACE_DEBUG ((LM_DEBUG, "As expected, Caught CORBA::INV_OBJREF\n"));
    }
}
