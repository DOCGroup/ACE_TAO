#include <memory>

#include "ace/Get_Opt.h"
#include "tao/IORTable/IORTable.h"
#include "Messenger_i.h"

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior_output_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c {};

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

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR const *ior_output_file = ACE_TEXT ("server.ior");
  try
    {
      // Initialize the ORB.
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      // Get the root POA.
      CORBA::Object_var const poa_object = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager = poa->the_POAManager ();

      if (parse_args (argc, argv, ior_output_file) != 0)
        return 1;

      poa_manager->activate ();

      // Get a reference to the IOR Table
      CORBA::Object_var const tobj = orb->resolve_initial_references ("IORTable");
      IORTable::Table_var const table = IORTable::Table::_narrow (tobj.in ());

      // Get an object id for the name.
      PortableServer::ObjectId_var const oid =
        PortableServer::string_to_ObjectId ("Messenger");

      // The POA borrows this stack servant for the lifetime of the event loop.
      Messenger_i messenger (orb.in ());
      poa->activate_object_with_id (oid.in (), &messenger);
      CORBA::Object_var const messenger_obj = poa->id_to_reference (oid.in ());

      // Bind the IOR string to the name.
      CORBA::String_var const messenger_ior_string = orb->object_to_string (messenger_obj.in ());
      table->bind ("Messenger", messenger_ior_string.in ());

      // Output the IOR to the <ior_output_file>
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior_output_file, "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s\n",
                             ior_output_file),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", messenger_ior_string.in ());
      }

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
