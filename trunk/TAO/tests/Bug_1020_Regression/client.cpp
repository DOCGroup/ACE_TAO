// $Id$

#include "Echo.h"
#include "ORB_Task.h"
#include "tao/Messaging/Messaging.h"
#include "tao/Utils/Servant_Var.h"
#include "tao/ORB_Core.h"
#include "ace/Get_Opt.h"
#include "ace/Reactor.h"

ACE_RCSID(Bug_1230_Regression, client, "$Id$")

const char *ior = "file://test.ior";

int
parse_args (int argc, char *argv[]);

class Crasher : public ACE_Event_Handler
{
public:
  Crasher();

  virtual int handle_timeout (ACE_Time_Value const & current_time,
                              void const * arg);
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
        orb->resolve_initial_references("RootPOA"
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
                        Echo(orb.in()),
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

      Test::Server_var server =
        Test::Server::_narrow(tmp.in ()
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (echo.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Server reference <%s>\n",
                             ior),
                            1);
        }

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Crasher crasher;

      // Crash after 15 seconds
      ACE_Time_Value interval(15, 0);
      ACE_Reactor * reactor = orb->orb_core()->reactor();
      reactor->schedule_timer(&crasher, 0, interval, interval);

      ORB_Task task(orb.in());
      task.activate(THR_NEW_LWP | THR_JOINABLE, 4, 1);

      Test::Payload payload(16); payload.length(16);
      for(int i = 0; i != 4; ++i)
        {
          server->start_task(echo.in()
                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      task.wait();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - event loop finished\n"));

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
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

Crasher::Crasher()
{
}

int
Crasher::handle_timeout (ACE_Time_Value const & ,
                         void const *)
{
  ACE_OS::abort();

  return 0;
}
