#include "orbsvcs/CosNamingC.h"
#include "Messenger_i.h"
#include "ace/Get_Opt.h"
#include <fstream>
#include <iostream>

ACE_TCHAR const *ior_output_file = ACE_TEXT ("Messenger.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        break;
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var naming_obj = orb->resolve_initial_references ("NameService");
      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ());
      CORBA::Object_var poa_object = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa = PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var manager = poa->the_POAManager ();
      manager->activate ();

      PortableServer::Servant_var<Messenger_i> servant = new Messenger_i (orb.in ());
      PortableServer::ObjectId_var object_id = poa->activate_object (servant.in ());
      CORBA::Object_var messenger_object = poa->id_to_reference (object_id.in ());

      CosNaming::Name name;
      name.length (1);
      name[0].id = CORBA::string_dup ("MessengerService");
      naming_context->rebind (name, messenger_object.in ());

      CORBA::String_var ior = orb->object_to_string (messenger_object.in ());
      std::ofstream ior_file (ACE_TEXT_ALWAYS_CHAR (ior_output_file));
      ior_file << ior.in () << std::endl;
      std::cout << "IOR written to file "
                << ACE_TEXT_ALWAYS_CHAR (ior_output_file) << std::endl;
      orb->run ();
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << ex << std::endl;
      return 1;
    }

  return 0;
}
