#include "orbsvcs/CosNamingC.h"
#include "Messenger_i.h"
#include "ace/Get_Opt.h"

#include <cstdio>
#include <iostream>
#include <memory>

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior_output_file)
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
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      ACE_TCHAR const *ior_output_file { ACE_TEXT ("Messenger.ior") };
      if (parse_args (argc, argv, ior_output_file) != 0)
        return 1;

      CORBA::Object_var const naming_obj = orb->resolve_initial_references ("NameService");
      CosNaming::NamingContext_var const naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ());
      CORBA::Object_var const poa_object = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var const manager = poa->the_POAManager ();
      manager->activate ();

      PortableServer::Servant_var<Messenger_i> const servant = new Messenger_i (orb.in ());
      PortableServer::ObjectId_var const object_id = poa->activate_object (servant.in ());
      CORBA::Object_var const messenger_object = poa->id_to_reference (object_id.in ());

      CosNaming::Name name {};
      name.length (1);
      name[0].id = CORBA::string_dup ("MessengerService");
      naming_context->rebind (name, messenger_object.in ());

      CORBA::String_var const ior = orb->object_to_string (messenger_object.in ());
      {
        std::unique_ptr<FILE, int (*) (FILE *)> ior_file (
          ACE_OS::fopen (ior_output_file, ACE_TEXT ("w")),
          ACE_OS::fclose);
        if (!ior_file)
          {
            std::cerr << "Could not create IOR output file" << std::endl;
            return 1;
          }
        ACE_OS::fprintf (ior_file.get (), "%s\n", ior.in ());
      }
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("IOR written to file %s\n"),
                  ior_output_file));
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
