// $Id$

#include "tao/IORManipulation/IORManip_Loader.h"
#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "testC.h"
#include "tao/TimeBaseC.h"
#include "tao/Messaging/Messaging.h"

ACE_RCSID(MProfile_Connection_Timeout,
          client,
          "$Id$")

const char *ior = "file://test.ior";
int do_shutdown = 0;

const char *name = "file://amba.ior";
TimeBase::TimeT timeout_period = 1000000;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "xk:m:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        do_shutdown = 1;
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'm':
        name = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

CORBA::ULongLong
run_test (Simple_Server_ptr server
          ACE_ENV_ARG_DECL);

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      // Primary server
      CORBA::Object_var object_primary =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //Secondary server
      CORBA::Object_var object_secondary =
        orb->string_to_object (name ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get an object reference for the ORBs IORManipultion object!
      CORBA::Object_ptr IORM =
        orb->resolve_initial_references (TAO_OBJID_IORMANIPULATION,
                                         0
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TAO_IOP::TAO_IOR_Manipulation_ptr iorm =
        TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TAO_IOP::TAO_IOR_Manipulation::IORList iors (2);
      iors.length(2);
      iors [0] = object_primary;
      iors [1] = object_secondary;

      CORBA::Object_var merged = iorm->merge_iors (iors ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->resolve_initial_references ("PolicyCurrent" ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      CORBA::Any timeout_as_any;
      timeout_as_any <<= timeout_period;

      CORBA::PolicyList policy_list (1);
      policy_list.length (1);

      policy_list[0] =
        orb->create_policy (TAO::CONNECTION_TIMEOUT_POLICY_TYPE,
                            timeout_as_any
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      policy_current->set_policy_overrides (policy_list,
                                            CORBA::ADD_OVERRIDE
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      for (CORBA::ULong l = 0;
           l != policy_list.length ();
           ++l)
        {
          policy_list[l]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // Combined IOR stuff
      Simple_Server_var server =
        Simple_Server::_narrow (merged.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      CORBA::ULongLong freq =
        run_test (server.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (freq != 919263)
        ACE_ERROR ((LM_ERROR,
                    ACE_LIB_TEXT ("(%P|%t) ERROR in the test \n")));


      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT ("(%P|%t) Shutting server down \n")));

      server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Caught an exception \n");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::ULongLong
run_test (Simple_Server_ptr server
          ACE_ENV_ARG_DECL)
{
  return server->remote_call (ACE_ENV_SINGLE_ARG_PARAMETER);
}
