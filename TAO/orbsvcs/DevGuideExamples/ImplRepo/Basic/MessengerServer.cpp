#include "Messenger_i.h"
#include <iostream>
#include <memory>
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior_output_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
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
                           ACE_TEXT ("usage:  %s -o <iorfile>\n"),
                           argv[0]),
                          -1);
      }

  return 0;
}

PortableServer::POA_var
create_persistent_poa (PortableServer::POA_ptr root_poa,
                       char const *poa_name)
{
  CORBA::PolicyList policies;
  policies.length (2);

  policies[0] = root_poa->create_lifespan_policy (PortableServer::PERSISTENT);
  policies[1] = root_poa->create_id_assignment_policy (PortableServer::USER_ID);

  PortableServer::POAManager_var const manager = root_poa->the_POAManager ();
  PortableServer::POA_var poa = root_poa->create_POA (poa_name, manager.in (), policies);

  policies[0]->destroy ();
  policies[1]->destroy ();

  return poa;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    ACE_TCHAR const *ior_output_file = ACE_TEXT ("messenger.ior");
    CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

    if (parse_args (argc, argv, ior_output_file) != 0)
      {
        return 1;
      }

    CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var const root_poa = PortableServer::POA::_narrow (obj.in ());

    PortableServer::POAManager_var const manager = root_poa->the_POAManager ();

    constexpr char poa_name[] = "MessengerService";

    PortableServer::POA_var const poa = create_persistent_poa (root_poa.in (), poa_name);

    PortableServer::Servant_var<Messenger_i> const servant = new Messenger_i;

    PortableServer::ObjectId_var const object_id =
      PortableServer::string_to_ObjectId ("object");

    poa->activate_object_with_id (object_id.in (), servant.in ());

    obj = poa->id_to_reference (object_id.in ());
    CORBA::String_var const ior = orb->object_to_string (obj.in ());

    {
      std::unique_ptr<FILE, int (*) (FILE *)> output_file (
        ACE_OS::fopen (ACE_TEXT_ALWAYS_CHAR (ior_output_file), "w"), ACE_OS::fclose);
      if (!output_file)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                           ior_output_file),
                          1);
      ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
    }

    manager->activate ();

    std::cout << "Messenger server ready." << std::endl;

    orb->run ();

    std::cout << "Messenger server shutting down." << std::endl;

    root_poa->destroy (true, true);
    orb->destroy ();

    return 0;
  }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Server main() Caught Exception" << ex << std::endl;
    }
  return 1;
}
