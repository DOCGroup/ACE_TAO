// $Id$

#include "ciao/Deployment_CoreC.h"
#include "ciao/Client_init.h"
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
                           "-k <NodeApplication ior> \n"
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
                             "Nil NodeApplication reference <%s>\n",
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
      info.servant_entrypt = "createNodeAppTest_RoundTripHome_Servant";

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
      ACE_DEBUG ((LM_DEBUG, "Try obtaining RoundTrip component ref from Home\n"));
      NodeAppTest::NodeAppTest_RoundTrip_var roundtrip_comp_var =
	NodeAppTest::NodeAppTest_RoundTrip::_narrow (
						     home_var->create
						     (ACE_ENV_SINGLE_ARG_PARAMETER)
						     );
      ACE_TRY_CHECK;
      if (CORBA::is_nil (roundtrip_comp_var.in ()))
       {
         ACE_ERROR_RETURN ((LM_DEBUG,
                            "Nil RoundTrip component reference\n"),
                            1);
       }

      ACE_TRY_CHECK;
      // initialize the component
      roundtrip_comp_var->ciao_postactivate ();
      ACE_CHECK_RETURN (1);


      //get the provided facets info.
      Components::FacetDescriptions_var facets_info =
	roundtrip_comp_var->get_all_facets (ACE_ENV_SINGLE_ARG_PARAMETER) ;
      ACE_TRY_CHECK;

      if ( facets_info->length () != 2 )
      {
	ACE_DEBUG((LM_DEBUG, "Didn't get 2 facet back! but only %d\n",
		   facets_info->length ()));
	return 1;
      }

      // Invoke Operation on the Interface
      ACE_DEBUG ((LM_DEBUG, "Try cube_long operation on the Interface \n"));

      for (CORBA::ULong i = 0; i < 2; ++i )
      {
	NodeAppTest::LatencyTest_var latency_var
	  = NodeAppTest::LatencyTest::_narrow ( (facets_info[i]->facet_ref ()));
	ACE_DEBUG((LM_DEBUG, "Calling on facet %s\n", (facets_info[i]->name ())));

	ACE_TRY_CHECK;

	if ( CORBA::is_nil (latency_var.in ()) )
	{
	  ACE_DEBUG((LM_DEBUG, "get nil latency ref for facet%d\n", i));
	  return 1;
	}

	CORBA::Long input = 1L;
	CORBA::Long output =
	  latency_var->cube_long (input ACE_ENV_ARG_PARAMETER);
	if (input == output)
	  ACE_DEBUG ((LM_DEBUG, "Retrun values matched!!\n"));
	else
	  {
	    ACE_DEBUG ((LM_DEBUG, "Return values did not match: failure\n"));
	    exit (1);
	  }
      }
      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "Test success!!\n"));
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
