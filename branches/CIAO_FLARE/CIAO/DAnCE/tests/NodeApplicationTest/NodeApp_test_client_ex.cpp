// $Id$

#include "DAnCE/Deployment/Deployment_NodeApplicationC.h"
#include "ciao/Client_init.h"
#include "NodeAppTest_RoundTripC.h"
#include "ace/Get_Opt.h"
#include "assert.h"

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
        DANCE_ERROR_RETURN ((LM_ERROR,
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
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CIAO::Client_init (orb.in ());

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Deployment::NodeApplication_var node_app =
        Deployment::NodeApplication::_narrow(tmp.in ());

      if (CORBA::is_nil (node_app.in ()))
        {
          DANCE_ERROR_RETURN ((LM_DEBUG,
                             "Nil NodeApplication reference <%s>\n",
                             ior),
                            1);
        }


      DANCE_DEBUG((LM_DEBUG, "[%M] Try installing Home and Component\n"));

      Deployment::ComponentImplementationInfo info;

      // Add the names and entry points of each of the DLLs
      info.component_instance_name =
        CORBA::string_dup ("NodeAppTest_RoundTrip");
      info.executor_dll =
        CORBA::string_dup ("NodeAppTest_RoundTrip_exec");
      info.executor_entrypt =
        CORBA::string_dup ("createRoundTripHome_Impl");
      info.servant_dll =
        CORBA::string_dup ("NodeAppTest_RoundTrip_svnt");
      info.servant_entrypt =
        CORBA::string_dup (
                           "create_NodeAppTest_NodeAppTest_RoundTripHome_Servant"
                           );

      // Create a ContainerImplementationInfo sequence
      Deployment::ContainerImplementationInfo container_info;
      container_info.impl_infos.length (1);
      container_info.impl_infos[0] = info;

      // Create a NodeImplementationInfo sequence
      Deployment::NodeImplementationInfo node_info;
      node_info.impl_infos.length (1);
      node_info.impl_infos[0] = container_info;

      // Install test component and its home on NodeApplication
      Deployment::ComponentInfos_var comp_info =
        node_app->install (node_info);

      assert (comp_info->length () == 1); //return 1 component objeref

      CORBA::ULong i = 0;
      Components::CCMObject_var objref = (comp_info[i]).component_ref;

      NodeAppTest::NodeAppTest_RoundTrip_var roundtrip_var =
        NodeAppTest::NodeAppTest_RoundTrip::_narrow (objref.in ());

      if (CORBA::is_nil (roundtrip_var.in ()))
        {
          DANCE_ERROR_RETURN ((LM_DEBUG,
                             "Nil RoundTrip reference\n"),
                            1);
        }

      // initialize the component

      //get the provided facets info.
      Components::FacetDescriptions_var facets_info =
        roundtrip_var->get_all_facets () ;

      if ( facets_info->length () != 2 )
        {
          DANCE_DEBUG((LM_DEBUG, "[%M] Didn't get 2 facet back! but only %d\n",
                     facets_info->length ()));
          return 1;
        }

      // Invoke Operation on the Interface
      DANCE_DEBUG((LM_DEBUG, "[%M] Try cube_long operation on the Interface \n"));

      for (i = 0; i < 2; ++i )
        {
          NodeAppTest::LatencyTest_var latency_var
            = NodeAppTest::LatencyTest::_narrow ( (facets_info[i]->facet_ref ()));
          DANCE_DEBUG((LM_DEBUG, "[%M] Calling on facet %s\n", (facets_info[i]->name ())));


          if ( CORBA::is_nil (latency_var.in ()) )
            {
              DANCE_DEBUG((LM_DEBUG, "[%M] get nil latency ref for facet%d\n", i));
              return 1;
            }

          CORBA::Long input = 1L;
          CORBA::Long output =
            latency_var->cube_long (input);

          if (input == output)
            DANCE_DEBUG((LM_DEBUG, "[%M] Retrun values matched!!\n"));
          else
            {
              DANCE_DEBUG((LM_DEBUG, "[%M] Return values did not match: failure\n"));
              ACE_OS::exit (1);
            }
        }

      DANCE_DEBUG((LM_DEBUG, "[%M] Try removing test component and its home\n"));
      node_app->remove ();
      DANCE_DEBUG((LM_DEBUG, "[%M] Component and Home removed successfully\n"));

      orb->destroy ();
      DANCE_DEBUG((LM_DEBUG, "[%M] Test success!!\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
