#include "Messenger_i.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/Get_Opt.h"

#include <fstream>
#include <iostream>

int
parse_args (int const argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior_output_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s [-o <ior>]\n"),
                           argv[0]),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      ACE_Argv_Type_Converter conv (argc, argv);
      CORBA::ORB_var const orb = CORBA::ORB_init (conv.get_argc (), conv.get_TCHAR_argv ());
      ACE_TCHAR const *ior_output_file = ACE_TEXT ("");
      if (parse_args (argc, argv, ior_output_file) != 0)
        return 1;

      CORBA::Object_var const naming_obj = orb->resolve_initial_references ("NameService");
      CosNaming::NamingContext_var const naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ());
      CORBA::Object_var const root_poa_obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (root_poa_obj.in ());
      PortableServer::POAManager_var const manager = poa->the_POAManager ();
      manager->activate ();

      PortableServer::Servant_var<Messenger_i> const messenger_servant = new Messenger_i (orb.in ());
      PortableServer::ObjectId_var const oid = poa->activate_object (messenger_servant.in ());
      CORBA::Object_var const messenger_obj = poa->id_to_reference (oid.in ());

      if (ior_output_file[0] != ACE_TCHAR {})
        {
          CORBA::String_var const ior = orb->object_to_string (messenger_obj.in ());
          std::ofstream ior_file (ACE_TEXT_ALWAYS_CHAR (ior_output_file));
          if (!ior_file)
            {
              std::cerr << "Unable to open IOR output file "
                        << ACE_TEXT_ALWAYS_CHAR (ior_output_file) << std::endl;
              return 1;
            }
          ior_file << ior.in () << std::endl;
          ior_file.close ();
        }

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup ("MessengerService");
      naming_context->rebind (name, messenger_obj.in ());

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
