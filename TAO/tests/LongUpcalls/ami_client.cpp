// $Id$

#include "ace/Get_Opt.h"
#include "Controller.h"
#include "Manager_Handler.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
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
                           "-k <ior>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      Controller controller_impl;

      PortableServer::ObjectId_var id =
        root_poa->activate_object (&controller_impl);

      CORBA::Object_var object_act = root_poa->id_to_reference (id.in ());

      Test::Controller_var controller =
        Test::Controller::_narrow (object_act.in ());

      object = orb->string_to_object (ior);

      Test::Manager_var manager =
        Test::Manager::_narrow (object.in ());

      if (CORBA::is_nil (manager.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Manager is nil\n"),
                          1);

      Manager_Handler handler_impl (manager.in (),
                                    controller.in ());

      id = root_poa->activate_object (&handler_impl);

      object_act = root_poa->id_to_reference (id.in ());

      Test::AMI_ManagerHandler_var handler =
        Test::AMI_ManagerHandler::_narrow (object_act.in ());

      poa_manager->activate ();

      const CORBA::Short worker_count = 10;
      manager->sendc_start_workers (handler.in (),
                                    worker_count,
                                    CORBA::Long (1000),
                                    controller.in ());

      ACE_Time_Value tv (30, 0);
      orb->run (tv);

      controller_impl.dump_results ();

      manager->shutdown ();

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
