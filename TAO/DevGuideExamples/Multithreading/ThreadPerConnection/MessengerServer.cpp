#include "Messenger_i.h"
#include "ace/Get_Opt.h"
#include <iostream>
#include <fstream>

ACE_TCHAR const *ior_output_file = ACE_TEXT ("test.ior");

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

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

    //Get reference to the RootPOA.
    CORBA::Object_var const obj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var const poa = PortableServer::POA::_narrow (obj.in ());

    // Activate the POAManager.
    PortableServer::POAManager_var const mgr = poa->the_POAManager ();
    mgr->activate();

    if (parse_args (argc, argv) != 0)
      return 1;

    // Create a servant.
    PortableServer::Servant_var<Messenger_i> const messenger_servant = new Messenger_i;

    // Register the servant with the RootPOA, obtain its object
    // reference, stringify it, and write it to a file.
    PortableServer::ObjectId_var const oid =
      poa->activate_object (messenger_servant.in ());
    CORBA::Object_var messenger_obj = poa->id_to_reference (oid.in ());
    CORBA::String_var const str = orb->object_to_string (messenger_obj.in ());

    // Output the IOR to the <ior_output_file>
    FILE *const output_file = ACE_OS::fopen (ior_output_file, "w");
    if (output_file == nullptr)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Cannot open output file for writing IOR: %s\n",
                         ior_output_file),
                         1);
    ACE_OS::fprintf (output_file, "%s", str.in ());
    ACE_OS::fclose (output_file);
    std::cout << "IOR written to file " << ACE_TEXT_ALWAYS_CHAR (ior_output_file) << std::endl;

    // Accept requests from clients.
    orb->run();
    orb->destroy();
  }
  catch (CORBA::Exception const &ex) {
    std::cerr << "CORBA exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}
