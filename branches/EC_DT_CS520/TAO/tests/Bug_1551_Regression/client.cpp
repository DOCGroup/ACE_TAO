//
// $Id$
//

#include "TestS.h"
#include "Client_Task.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Hello, client, "$Id$")

const char *ior = "file://test.ior";

int nthreads = 1;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "n:k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'n':
        nthreads = ACE_OS::atoi(get_opts.opt_arg());
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

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Hello_var hello =
        Test::Hello::_narrow(tmp.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;


      CORBA::Object_var object =
        orb->resolve_initial_references ("PolicyCurrent"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TimeBase::TimeT timeout_period = 20 * 1000;

      CORBA::Any timeout_as_any;
      timeout_as_any <<= timeout_period;

      CORBA::PolicyList policy_list (1);
      policy_list.length (1);
      policy_list[0] =
        orb->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                                   timeout_as_any
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policy_current->set_policy_overrides (policy_list,
                                            CORBA::ADD_OVERRIDE
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      // Let the client perform the test in a separate thread


      Client_Task client (hello.in (),
                          orb.in (),
                          ACE_Thread_Manager::instance ());

      if (client.activate (THR_NEW_LWP | THR_JOINABLE, nthreads) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                              "ERROR - Cannot activate client threads\n"),
            1);
      }


      ACE_Time_Value thread_deadline = ACE_OS::gettimeofday();
      thread_deadline += 30;

      if(client.thr_mgr ()->wait (&thread_deadline) == -1)
      {
        ACE_ERROR_RETURN((LM_ERROR,
                             "ERROR - Timeout waiting for client threads\n"),
            1);
      }

      root_poa->destroy (1,  // ethernalize objects
                         0  // wait for completion
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
