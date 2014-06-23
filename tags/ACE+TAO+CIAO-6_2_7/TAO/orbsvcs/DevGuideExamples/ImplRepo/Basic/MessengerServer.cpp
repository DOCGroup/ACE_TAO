// $Id$

#include "Messenger_i.h"
#include <iostream>
#include <fstream>
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("messenger.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
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
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}


PortableServer::POA_ptr
createPersistentPOA(PortableServer::POA_ptr root_poa,
                    const char* poa_name)
{
  CORBA::PolicyList policies;
  policies.length(2);

  policies[0] = root_poa->create_lifespan_policy(PortableServer::PERSISTENT);
  policies[1] = root_poa->create_id_assignment_policy(PortableServer::USER_ID);

  PortableServer::POAManager_var mgr = root_poa->the_POAManager();
  PortableServer::POA_var poa =
    root_poa->create_POA(poa_name, mgr.in(), policies);

  policies[0]->destroy();
  policies[1]->destroy();

  return poa._retn();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    if (parse_args (argc, argv) != 0)
      return 1;

    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj.in());

    PortableServer::POAManager_var mgr = root_poa->the_POAManager();

    const char* poa_name = "MessengerService";

    PortableServer::POA_var poa = createPersistentPOA(root_poa.in(), poa_name);

    PortableServer::Servant_var<Messenger_i> servant = new Messenger_i;

    PortableServer::ObjectId_var object_id =
      PortableServer::string_to_ObjectId("object");

    poa->activate_object_with_id(object_id.in(), servant.in());

    obj = poa->id_to_reference(object_id.in());
    CORBA::String_var ior = orb->object_to_string(obj.in());

    // If the ior_output_file exists, output the ior to it
    if (ior_output_file != 0)
    {
      FILE *output_file= ACE_OS::fopen (ACE_TEXT_ALWAYS_CHAR(ior_output_file), "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);
    }

    mgr->activate();

    std::cout << "Messenger server ready." << std::endl;

    orb->run();

    std::cout << "Messenger server shutting down." << std::endl;

    root_poa->destroy(1,1);
    orb->destroy();

    return 0;
  } catch(const CORBA::Exception& ex) {
    std::cerr << "Server main() Caught Exception" << ex << std::endl;
  }
  return 1;
}
