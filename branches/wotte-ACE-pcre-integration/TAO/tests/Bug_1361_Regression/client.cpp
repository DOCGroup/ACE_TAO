// $Id$

#include "Echo.h"
#include "ORB_Task.h"
#include "tao/Messaging/Messaging.h"
#include "tao/Utils/Servant_Var.h"
#include "tao/ORB_Core.h"
#include "ace/Get_Opt.h"
#include "ace/Reactor.h"

ACE_RCSID(Bug_1270_Regression, client, "$Id$")

const char *ior = "file://test.ior";
int serverthreads = 4;

int
parse_args (int argc, char *argv[]);

class Client_Timer : public ACE_Event_Handler
{
public:
  /// Constructor
  Client_Timer (ACE_Reactor * reactor)
    : ACE_Event_Handler (reactor)
  {
  }

  void activate (void)
  {
    ACE_Time_Value tv (150, 0);
    this->reactor()->schedule_timer (this, 0, tv, tv);
  }

  /// Thread entry point
  int handle_timeout (ACE_Time_Value const & , void const *)
  {
    // kill the application
    raise (9);
    this->reactor ()->cancel_timer (this);
    return 0;
  }
  int handle_close (ACE_HANDLE, ACE_Reactor_Mask)
  {
    delete this;
    return 0;
  }

};

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, ""
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->resolve_initial_references ("PolicyCurrent" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (policy_current.in ()))
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Nil policy current\n"));
          return 1;
        }
      CORBA::Any scope_as_any;
      scope_as_any <<= Messaging::SYNC_WITH_TRANSPORT;

      CORBA::PolicyList policies(1); policies.length (1);
      policies[0] =
        orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                            scope_as_any
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policy_current->set_policy_overrides (policies, CORBA::ADD_OVERRIDE
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policies[0]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      TAO::Utils::Servant_Var<Echo> impl;
      {
        Echo * tmp;
        // ACE_NEW_RETURN is the worst possible way to handle
        // exceptions (think: what if the constructor allocates memory
        // and fails?), but I'm not in the mood to fight for a more
        // reasonable way to handle allocation errors in ACE.
        ACE_NEW_RETURN (tmp,
                        Echo(orb.in(), 1000 / serverthreads),
                        1);
        impl = tmp;
      }

      Test::Echo_var echo =
        impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var tmp =
        orb->string_to_object(ior
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Echo_Caller_var server =
        Test::Echo_Caller::_narrow(tmp.in ()
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Echo_Caller reference <%s>\n",
                             ior),
                            1);
        }

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ORB_Task worker (orb.in());
      worker.activate (THR_NEW_LWP | THR_JOINABLE,
                       serverthreads);

      ACE_TRY_EX (BL)
        {
          for(int i = serverthreads; i; --i)
            {
              server->start_task(echo.in()
                                 ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK_EX (BL);
            }
        }
      ACE_CATCHALL
        {
        }
      ACE_ENDTRY;

      Client_Timer * task = new Client_Timer (orb->orb_core()->reactor());
      task->activate ();

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      worker.wait ();

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) client - event loop finished\n"));

      // Actually the code here should never be reached.
      root_poa->destroy (1, 1
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
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

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:t:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 't':
        serverthreads = atoi(get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-t threads "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}
