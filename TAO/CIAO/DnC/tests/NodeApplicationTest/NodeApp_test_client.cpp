// $Id$

#include "Deployment_CoreC.h"
#include "Client_init.h"
#include "NodeAppTest_RoundTripC.h"
#include "ace/Get_Opt.h"
#include <sstream>
#include <vector>
#include <stdlib.h>

const char *ior = "file://test.ior";
int comp_number = 4;

int parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:n:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'n':
        comp_number = atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s \n"
                           "-k <NodeApplication ior> \n"
                           "-n <The number of component instances> \n"
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
  std::vector<NodeAppTest::NodeAppTest_RoundTrip_var> comp_list;
  //std::vector<NodeAppTest::NodeAppTest_RoundTrip_var>::const_iterator iter;

  ACE_TRY_NEW_ENV
    {
      CORBA::ULong comp_num (comp_number);
      ACE_DEBUG ((LM_DEBUG, "CompNum: %d\n",comp_num));

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
                             "Nil ServerActivator reference <%s>\n", ior),
                            1);
        }

      // This is what we will send out, containing all the instance info the
      // container needs
      Deployment::ImplementationInfos infos;
      infos.length (CORBA::ULong (comp_num));

      // This is what we will get back, a sequence of compoent object refs.
      Deployment::ComponentInfos_var comp_info;

      for (CORBA::ULong i = 0; i < comp_num; ++i)
      {
	Deployment::ImplementationInfo info;

	std::stringstream tmp;
        tmp << "NodeAppTest_RoundTrip:" << i;

	// Add the names and entry points of each of the DLLs
	info.component_instance_name = CORBA::string_dup (tmp.str ().c_str ());
	info.executor_dll = CORBA::string_dup ("NodeAppTest_RoundTrip_exec");
	info.executor_entrypt = CORBA::string_dup ("createRoundTripHome_Impl");
	info.servant_dll = CORBA::string_dup ("NodeAppTest_RoundTrip_svnt");
	info.servant_entrypt = CORBA::string_dup ("createNodeAppTest_RoundTripHome_Servant");

	//Now add the info into the infos
	infos[i] = info;
      }

      // For debug purpose.
      for (CORBA::ULong i = 0; i < comp_num; ++i)
      {
	Deployment::ImplementationInfo info;

	std::stringstream tmp;
        tmp << "NodeAppTest_RoundTrip:" << i;

	// Add the names and entry points of each of the DLLs
	ACE_DEBUG ((LM_DEBUG, "The info I will send out: \n\t%s\n\t%s\n\t%s\n\t%s\n\t%s\n",
		    infos[i].component_instance_name.in (),
		    infos[i].executor_dll.in (),
		    infos[i].executor_entrypt.in (),
		    infos[i].servant_dll.in (),
		    infos[i].servant_entrypt.in () ));
      }

      ACE_DEBUG ((LM_DEBUG, "Try installing Homes and Components\n"));

      // Install the NodeApplication Test components
      comp_info = comserv->install (infos);
      ACE_TRY_CHECK;

      // store the component refs
      for (CORBA::ULong i = 0; i < comp_num; ++i)
      {
	comp_list.push_back (NodeAppTest::NodeAppTest_RoundTrip::_narrow
			     (comp_info[i].component_ref));
	ACE_TRY_CHECK;

	if (CORBA::is_nil (comp_list[i].in ()))
	{
	  ACE_ERROR_RETURN ((LM_DEBUG,
			     "Nil RoundTripHome reference: %s \n",
			     comp_info[i].component_instance_name.in ()),
			    1);
	}
      }

      ACE_DEBUG ((LM_DEBUG, "Installation finished successfully.\n"));

      // Before we can start we have to start.
      comserv->start ();
      ACE_CHECK_RETURN (1);

      // Invoke Operation on the components
      ACE_DEBUG ((LM_DEBUG, "Try cube_long operation on the Interface \n"));

      for (CORBA::ULong i = 0; i < comp_num; ++i)
      {

	CORBA::Long input = i;

	CORBA::Long output =
	  (comp_list[i])->cube_long (input ACE_ENV_ARG_PARAMETER);

	if (input*input*input == output)
	  ACE_DEBUG ((LM_DEBUG, "Retrun values matched!! on Component: %d \n", i));

	else
	{
	  ACE_DEBUG ((LM_DEBUG, "Return values did not match: on Component: %d \n", i));
	  exit (1);
	}
      }

      ACE_DEBUG ((LM_DEBUG, "Try removing everything\n"));
      comserv->remove ();
      ACE_CHECK_RETURN (1);

      ACE_DEBUG ((LM_DEBUG, "Homes removed successfully\n"));

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
