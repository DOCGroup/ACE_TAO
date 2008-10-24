// $Id$

// This test tries to install a set of components in 2 different containers, which
// is hosted in the same NodeApplication.

#include "DAnCE/Deployment/Deployment_NodeApplicationC.h"
#include "Client_init.h"
#include "NodeAppTest_RoundTripC.h"
#include "ace/Get_Opt.h"
#include <stdlib.h>
#include "assert.h"
#include "ace/Vector_T.h"

const char *ior = "file://test.ior";
int comp_number = 4;
int counter = 0;

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
        comp_number = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        DANCE_ERROR_RETURN ((LM_ERROR,
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
  ACE_Vector<NodeAppTest::NodeAppTest_RoundTrip_var> comp_list;
  
  try
    {
      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::ULong comp_num (comp_number);
      DANCE_DEBUG((LM_DEBUG, "[%M] CompNum: %d\n",comp_num));

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CIAO::Client_init (orb.in ());

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Deployment::NodeApplication_var node_app =
        Deployment::NodeApplication::_narrow (tmp.in ());

      if (CORBA::is_nil (node_app.in ()))
        {
          DANCE_ERROR_RETURN ((LM_DEBUG,
                             "Nil nodeapplication reference <%s>\n", ior),
                            1);
        }

      // This is what we will send out of the ContainerImplmentationInfo for the
      // first container

      Deployment::ContainerImplementationInfo container_info_1;
      CORBA::ULong length_1 = comp_num/2;
      container_info_1.impl_infos.length (CORBA::ULong (length_1));

      CORBA::ULong i;
      char buf[256];
      for (i = 0; i < length_1; ++i)
        {
          Deployment::ComponentImplementationInfo info;

          sprintf(buf, "NodeAppTest_RoundTrip:%i", counter);
          ACE_CString tmp = buf;
          counter = counter + 1;

          // Add the names and entry points of each of the DLLs
          info.component_instance_name = CORBA::string_dup (tmp.c_str ());
          info.executor_dll = CORBA::string_dup ("NodeAppTest_RoundTrip_exec");
          info.executor_entrypt =
            CORBA::string_dup ("createRoundTripHome_Impl");
          info.servant_dll =
            CORBA::string_dup ("NodeAppTest_RoundTrip_svnt");
          info.servant_entrypt =
            CORBA::string_dup (
                "create_NodeAppTest_NodeAppTest_RoundTripHome_Servant"
              );

          //Now add the info into the infos
          container_info_1.impl_infos[i] = info;
        }

      // For debug purpose.
      for (i = 0; i < comp_num/2; ++i)
        {
          sprintf(buf, "NodeAppTest_RoundTrip:%i", i);
          ACE_CString tmp = buf;
          
          // Add the names and entry points of each of the DLLs
          DANCE_DEBUG((LM_DEBUG, "[%M] The info I will send out: \n\t%s\n\t%s\n\t%s\n\t%s\n\t%s\n",
            container_info_1.impl_infos[i].component_instance_name.in (),
            container_info_1.impl_infos[i].executor_dll.in (),
            container_info_1.impl_infos[i].executor_entrypt.in (),
            container_info_1.impl_infos[i].servant_dll.in (),
            container_info_1.impl_infos[i].servant_entrypt.in () ));
        }

      // This is what we will send out of the ContainerImplmentationInfo for the
      // first container

      Deployment::ContainerImplementationInfo container_info_2;
      CORBA::ULong length_2 = comp_num - comp_num/2;
      container_info_2.impl_infos.length (CORBA::ULong (length_2));

      for (i = 0; i < length_2; ++i)
        {
          Deployment::ComponentImplementationInfo info;

          sprintf(buf, "NodeAppTest_RoundTrip:%i", counter);
          ACE_CString tmp = buf;
          counter = counter + 1;

          // Add the names and entry points of each of the DLLs
          info.component_instance_name =
            CORBA::string_dup (tmp.c_str ());
          info.executor_dll = CORBA::string_dup ("NodeAppTest_RoundTrip_exec");
          info.executor_entrypt =
            CORBA::string_dup ("createRoundTripHome_Impl");
          info.servant_dll =
            CORBA::string_dup ("NodeAppTest_RoundTrip_svnt");
          info.servant_entrypt =
            CORBA::string_dup (
                "create_NodeAppTest_NodeAppTest_RoundTripHome_Servant"
              );

          //Now add the info into the infos
          container_info_2.impl_infos[i] = info;
        }

      // For debug purpose.
      for (i = 0; i < length_2; ++i)
        {
          sprintf(buf, "NodeAppTest_RoundTrip:%i", i);
          ACE_CString tmp = "NodeAppTest_RoundTrip:";
          
          // Add the names and entry points of each of the DLLs
          DANCE_DEBUG((LM_DEBUG, "[%M] The info I will send out: \n\t%s\n\t%s\n\t%s\n\t%s\n\t%s\n",
            container_info_2.impl_infos[i].component_instance_name.in (),
            container_info_2.impl_infos[i].executor_dll.in (),
            container_info_2.impl_infos[i].executor_entrypt.in (),
            container_info_2.impl_infos[i].servant_dll.in (),
            container_info_2.impl_infos[i].servant_entrypt.in () ));
        }

      // container_info.container_config is not set for now

      DANCE_DEBUG((LM_DEBUG, "[%M] Try installing Homes and Components\n"));

      // Create a NodeImplementationInfo sequence
      Deployment::NodeImplementationInfo node_info;
      node_info.impl_infos.length (2);
      node_info.impl_infos[0] = container_info_1;
      node_info.impl_infos[1] = container_info_2;

      // Install test component and its home on NodeApplication
      Deployment::ComponentInfos_var comp_info =
        node_app->install (node_info);

      // store the component refs
      for (i = 0; i < comp_num; ++i)
        {
          comp_list.push_back (NodeAppTest::NodeAppTest_RoundTrip::_narrow
                               (comp_info[i].component_ref.in ()));

          if (CORBA::is_nil (comp_list[i].in ()))
            {
              DANCE_ERROR_RETURN ((LM_DEBUG,
                                 "Nil RoundTripHome reference: %s \n",
                                 comp_info[i].component_instance_name.in ()),
                                1);
            }
        }

      DANCE_DEBUG((LM_DEBUG, "[%M] Installation finished successfully.\n"));

      // Before we can start we have to start.
      node_app->start ();

      // Invoke Operation on the components
      DANCE_DEBUG((LM_DEBUG, "[%M] Try cube_long operation on the Interface \n"));

      for (i = 0; i < comp_num; ++i)
        {

          CORBA::Long input = i;

          CORBA::Long output =
            (comp_list[i])->cube_long (input);

          if (input*input*input == output)
            {
              DANCE_DEBUG((LM_DEBUG, "[%M] Return values matched!! on Component: %d \n",
                          i));
            }
          else
            {
              DANCE_DEBUG((LM_DEBUG, "[%M] Return values did not match: on Component: %d \n",
                          i));
              ACE_OS::exit (1);
            }
        }

      DANCE_DEBUG((LM_DEBUG, "[%M] Try removing everything\n"));
      node_app->remove ();

      DANCE_DEBUG((LM_DEBUG, "[%M] Homes and components removed successfully\n"));

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
