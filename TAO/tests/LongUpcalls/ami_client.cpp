#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "Controller.h"
#include "Manager_Handler.h"

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://test.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
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
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Client_Options options;
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv, options) != 0)
        return 1;

      Controller controller_impl;

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&controller_impl);

      CORBA::Object_var const controller_object = root_poa->id_to_reference (id.in ());

      Test::Controller_var const controller =
        Test::Controller::_narrow (controller_object.in ());

      CORBA::Object_var const manager_object =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (options.ior.c_str ()));

      Test::Manager_var const manager =
        Test::Manager::_narrow (manager_object.in ());

      if (CORBA::is_nil (manager.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Manager is nil\n"),
                          1);

      Manager_Handler handler_impl (manager.in (), controller.in ());

      PortableServer::ObjectId_var const handler_id = root_poa->activate_object (&handler_impl);

      CORBA::Object_var const handler_object = root_poa->id_to_reference (handler_id.in ());

      Test::AMI_ManagerHandler_var const handler =
        Test::AMI_ManagerHandler::_narrow (handler_object.in ());

      poa_manager->activate ();

      CORBA::Short const worker_count = 10;
      manager->sendc_start_workers (handler.in (),
                                    worker_count,
                                    CORBA::Long (1000),
                                    controller.in ());

      ACE_Time_Value tv (30, 0);
      orb->run (tv);

      controller_impl.dump_results ();

      manager->shutdown ();

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
