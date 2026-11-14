#include "Messenger_i.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Get_Opt.h"

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

struct Options
{
  ACE_TCHAR const *ior_output_file {ACE_TEXT ("url.ior")};
  ACE_TCHAR const *hostname {ACE_TEXT ("localhost")};
  ACE_TCHAR const *port {ACE_TEXT ("2809")};
};

int parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:h:p:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;

      case 'h':
        options.hostname = get_opts.opt_arg ();
        break;

      case 'p':
        options.port = get_opts.opt_arg ();
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -o <iorfile> -h <host> -p <port>\n"),
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
      CosNaming::NamingContextExt_var const root =
        CosNaming::NamingContextExt::_narrow (naming_object.in ());
      if (CORBA::is_nil (root.in ()))
        {
          std::cerr << "Nil Naming Context reference" << std::endl;
          return 1;
        }

      CosNaming::Name name {};
      name.length (1);
      name[0].id = CORBA::string_dup ("root.esc-dot");
      name[0].kind = CORBA::string_dup ("kind1");
      try
        {
          CORBA::Object_var const context_object = root->resolve (name);
        }
      catch (CosNaming::NamingContext::NotFound const &)
        {
          CosNaming::NamingContext_var const context = root->bind_new_context (name);
        }

      name.length (2);
      name[1].id = CORBA::string_dup ("leaf/esc-slash");
      name[1].kind = CORBA::string_dup ("kind2");

      PortableServer::Servant_var<Messenger_i> const servant = new Messenger_i;
      PortableServer::ObjectId_var const object_id = poa->activate_object (servant.in ());
      CORBA::Object_var const messenger_object = poa->id_to_reference (object_id.in ());
      root->rebind (name, messenger_object.in ());

      CosNaming::Name_var simple_name = root->to_name ("Simple");
      try
        {
          CORBA::Object_var const context_object = root->resolve (simple_name.in ());
        }
      catch (CosNaming::NamingContext::NotFound const &)
        {
          CosNaming::NamingContext_var const context = root->bind_new_context (simple_name.in ());
        }
      simple_name = root->to_name ("Simple/Messenger");
      root->rebind (simple_name.in (), messenger_object.in ());

      CORBA::String_var const string_name = root->to_string (name);
      std::cout << "str_name:  " << string_name.in () << std::endl;
      CORBA::String_var const string_simple = root->to_string (simple_name.in ());
      std::cout << "simple: " << string_simple.in () << std::endl;

      CosNaming::Name_var const converted_name = root->to_name (string_name.in ());
      std::cout << "converted back to a CosNaming::Name: " << std::endl;
      std::cout << "   name[0] = " << converted_name.in ()[0].id.in () << " , "
                << converted_name.in ()[0].kind.in () << std::endl;
      std::cout << "   name[1] = " << converted_name.in ()[1].id.in () << " , "
                << converted_name.in ()[1].kind.in () << std::endl;

      try
        {
          CORBA::Object_var const object = root->resolve_str (string_name.in ());
        }
      catch (CosNaming::NamingContext::NotFound const &)
        {
          std::cerr << "Couldn't resolve the string name: " << string_name.in () << std::endl;
          return 1;
        }

      std::string base_address (":");
      base_address += ACE_TEXT_ALWAYS_CHAR (options.hostname);
      base_address += ':';
      base_address += ACE_TEXT_ALWAYS_CHAR (options.port);
      std::string address (base_address + "/key/str");

      std::cout << "call to_url(\"" << address << "\"" << std::endl;
      std::cout << "           ,\"" << string_simple.in () << "\")" << std::endl;
      CORBA::String_var const url_string = root->to_url (address.c_str (), string_simple.in ());
      std::cout << "to_url result: " << url_string.in () << std::endl;

      std::cout << "call to_url(\"" << base_address << "\",\""
                << string_simple.in () << "\")" << std::endl;
      CORBA::String_var const url = root->to_url (base_address.c_str (), string_simple.in ());
      std::cout << "to_url result:    " << url.in () << std::endl;

      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_output_file, ACE_TEXT ("w")),
          ACE_OS::fclose);
        if (!output_file)
          throw std::runtime_error ("Cannot open URL output file");
        ACE_OS::fprintf (output_file.get (), "%s\n", url.in ());
      }
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Naming Service URL written to file %s\n"),
                  options.ior_output_file));

      orb->run ();
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Caught a CORBA::Exception: " << ex << std::endl;
      return 1;
    }
  catch (std::runtime_error const &ex)
    {
      std::cerr << "Server: " << ex.what () << std::endl;
      return 1;
    }

  return 0;
}
