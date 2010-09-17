// $Id$

#include "ace/Get_Opt.h"
#include "tao/IORTable/IORTable.h"
#include "Messenger_i.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT("server.ior");

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
  // Indicates sucessful parsing of the command line
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      //init the orb
      CORBA::ORB_var orb=CORBA::ORB_init(argc, argv);

      //get the root poa
      CORBA::Object_var poa_object = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa = PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager = poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      poa_manager->activate ();

      // Get a reference to the IOR Table
      CORBA::Object_var tobj = orb->resolve_initial_references("IORTable");
      IORTable::Table_var table = IORTable::Table::_narrow(tobj.in());

      //get an object id for the name
      PortableServer::ObjectId_var oid =
      PortableServer::string_to_ObjectId ("Messenger");

      //activate the object for the given id
      Messenger_i messenger (orb.in ());
      poa->activate_object_with_id(oid.in (), &messenger);
      CORBA::Object_var messenger_obj = poa->id_to_reference (oid.in ());

      //bind the ior string to the name
      CORBA::String_var messenger_ior_string = orb->object_to_string(messenger_obj.in());
      table->bind("Messenger", messenger_ior_string.in());

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s\n",
                           ior_output_file),
                          1);
      ACE_OS::fprintf (output_file, "%s", messenger_ior_string.in ());
      ACE_OS::fclose (output_file);

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
