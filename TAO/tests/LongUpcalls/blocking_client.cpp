#include "ace/Get_Opt.h"
#include "Controller.h"

namespace
{
  ACE_TCHAR const *ior = ACE_TEXT ("file://test.ior");
}

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
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      Controller controller_impl;

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&controller_impl);

      CORBA::Object_var const object_act = root_poa->id_to_reference (id.in ());

      Test::Controller_var const controller =
        Test::Controller::_narrow (object_act.in ());

      object = orb->string_to_object (ior);

      Test::Manager_var const manager =
        Test::Manager::_narrow (object.in ());

      if (CORBA::is_nil (manager.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Manager is nil\n"),
                          1);


      poa_manager->activate ();

      CORBA::Short const worker_count = 10;
      manager->start_workers (worker_count,
                              CORBA::Long (1000),
                              controller.in ());

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
