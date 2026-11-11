#include "Messenger_i.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Get_Opt.h"
#include <fstream>
#include <iostream>

struct Options
{
  ACE_TCHAR const *output_file {ACE_TEXT ("MessengerServer.ready")};
};

int parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.output_file = get_opts.opt_arg ();
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -o <outputfile>\n"),
                           argv[0]),
                          -1);
      }

  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      Options options {};
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var poa_object = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa = PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var poa_manager = poa->the_POAManager ();
      poa_manager->activate ();

      CORBA::Object_var naming_object = orb->resolve_initial_references ("NameService");
      CosNaming::NamingContext_var root =
        CosNaming::NamingContext::_narrow (naming_object.in ());
      if (CORBA::is_nil (root.in ()))
        {
          std::cerr << "Nil Naming Context reference" << std::endl;
          return 1;
        }

      CosNaming::Name name {};
      name.length (1);
      name[0].id = CORBA::string_dup ("example");
      try
        {
          CORBA::Object_var context_object = root->resolve (name);
        }
      catch (CosNaming::NamingContext::NotFound const &)
        {
          CosNaming::NamingContext_var context = root->bind_new_context (name);
        }

      name.length (2);
      name[1].id = CORBA::string_dup ("Messenger");
      PortableServer::Servant_var<Messenger_i> servant = new Messenger_i;
      PortableServer::ObjectId_var object_id = poa->activate_object (servant.in ());
      CORBA::Object_var object = poa->id_to_reference (object_id.in ());
      Messenger_var messenger = Messenger::_narrow (object.in ());
      root->rebind (name, messenger.in ());
      std::cout << "Messenger object bound in Naming Service" << std::endl;

      {
        std::ofstream output_file (ACE_TEXT_ALWAYS_CHAR (options.output_file));
        if (!output_file)
          {
            std::cerr << "Could not create readiness file" << std::endl;
            return 1;
          }
        output_file << "Ready" << std::endl;
      }

      orb->run ();
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "server: Caught a CORBA::Exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}
