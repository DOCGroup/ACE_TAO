#include "Messenger_i.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Get_Opt.h"

#include <cstdio>
#include <iostream>
#include <memory>

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
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      Options options {};
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const poa_object = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var const poa_manager = poa->the_POAManager ();
      poa_manager->activate ();

      CORBA::Object_var const naming_object = orb->resolve_initial_references ("NameService");
      CosNaming::NamingContext_var const root =
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
          CORBA::Object_var const context_object = root->resolve (name);
        }
      catch (CosNaming::NamingContext::NotFound const &)
        {
          CosNaming::NamingContext_var const context = root->bind_new_context (name);
        }

      name.length (2);
      name[1].id = CORBA::string_dup ("Messenger");
      PortableServer::Servant_var<Messenger_i> const servant = new Messenger_i;
      PortableServer::ObjectId_var const object_id = poa->activate_object (servant.in ());
      CORBA::Object_var const object = poa->id_to_reference (object_id.in ());
      Messenger_var const messenger = Messenger::_narrow (object.in ());
      root->rebind (name, messenger.in ());
      std::cout << "Messenger object bound in Naming Service" << std::endl;

      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.output_file, ACE_TEXT ("w")),
          ACE_OS::fclose);
        if (!output_file)
          {
            std::cerr << "Could not create readiness file" << std::endl;
            return 1;
          }
        ACE_OS::fprintf (output_file.get (), "Ready\n");
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
