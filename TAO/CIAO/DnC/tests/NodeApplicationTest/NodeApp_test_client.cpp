// $Id$

#include "DeploymentC.h"
#include "Client_init.h"
#include "NodeAppTest_RoundTripC.h"
#include "ace/Get_Opt.h"

const char *ior = "file://test.ior";

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
                           "usage:  %s \n"
                           "-k <ComponentServer ior> \n"
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

      CIAO::Client_init (orb.in ());

      CORBA::Object_var tmp =
        orb->string_to_object(ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Deployment::NodeApplication_var comserv =
        Deployment::NodeApplication::_narrow(tmp.in ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (comserv.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil ServerActivator reference <%s>\n",
                             ior),
                            1);
        }


      ACE_DEBUG ((LM_DEBUG, "Try installing Home\n"));

      Deployment::ImplementationInfo info;

      // Add the names and entry points of each of the DLLs
      info.component_instance_name = "NodeAppTest_RoundTrip";
      info.executor_dll = "NodeAppTest_RoundTrip_exec";
      info.executor_entrypt = "createRoundTripHome_Impl";
      info.servant_dll = "NodeAppTest_RoundTrip_svnt";
      info.servant_entrypt = "createRoundTripHome_Servant";

      // Install the NodeApplication Test component
      ::Components::CCMHome_var home = comserv->install_home (info);
      ACE_TRY_CHECK;

      // Narrow the Home to the appropriate component
      NodeAppTest::NodeAppTest_RoundTripHome_var home_var =
        NodeAppTest::NodeAppTest_RoundTripHome::_narrow (home.in ());

      ACE_TRY_CHECK;
      if (CORBA::is_nil (home_var.in ()))
       {
         ACE_ERROR_RETURN ((LM_DEBUG,
                            "Nil RoundTripHome reference\n"),
                            1);
       }

      // Get Component from Home
      ACE_DEBUG ((LM_DEBUG, "Try obtaining RoundTrip ref from Home\n"));
      NodeAppTest::NodeAppTest_RoundTrip_var roundtrip_var =
        home_var->create (ACE_ENV_SINGLE_ARG_PARAMETER);

      if (CORBA::is_nil (roundtrip_var.in ()))
       {
         ACE_ERROR_RETURN ((LM_DEBUG,
                            "Nil RoundTrip reference\n"),
                            1);
       }

      // Get the interface from the Component reference
      ACE_DEBUG ((LM_DEBUG, "Try obtaining RoundTrip ref from Home\n"));
      NodeAppTest::LatencyTest_var latency_test =
        roundtrip_var->provide_latency ();
      ACE_TRY_CHECK;

      if (CORBA::is_nil (latency_test.in ()))
       {
         ACE_ERROR_RETURN ((LM_DEBUG,
                            "Nil Interface reference\n"),
                            1);
       }


      // Invoke Operation on the Interface
      ACE_DEBUG ((LM_DEBUG, "Try cube_long operation on the Interface \n"));
      CORBA::Long input = 1L;
      CORBA::Long output =
        latency_test->cube_long (input ACE_ENV_ARG_PARAMETER);

     if (input == output)
       ACE_DEBUG ((LM_DEBUG, "Test succeeded\n"));
     else
       ACE_DEBUG ((LM_DEBUG, "Test failure\n"));

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
