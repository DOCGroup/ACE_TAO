//
// $Id$
//
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "Test_i.h"

#include "tao/IORTable/IORTable.h"

const ACE_TCHAR *object_key = 0;
const ACE_TCHAR *ior_file = ACE_TEXT ("server.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:k"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_file = get_opts.opt_arg ();
        break;
      case 'k':
        object_key = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "SERVER (%P): usage:  %s "
                           "-k <object key> "
                           "-o <ior>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "SERVER (%P): Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      Test_i *server_impl = 0;
      ACE_NEW_RETURN (server_impl,
                      Test_i (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer(server_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (server_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      CORBA::Object_var server = CORBA::Object::_narrow (object.in ());

      CORBA::String_var ior =
        orb->object_to_string (server.in ());

      CORBA::Object_var table_object =
        orb->resolve_initial_references ("IORTable");

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in ());

      adapter->bind("Name-with-hyphens", ior.in());

      FILE *output_file= ACE_OS::fopen (ior_file, "w");
      if (output_file == 0)
         ACE_ERROR_RETURN ((LM_ERROR,
                               "SERVER (%P): Cannot open output file "
                               "for writing IOR: %s",
                               ior_file),
                              1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      ACE_DEBUG ((LM_DEBUG,
                      "SERVER (%P): Activated as file://%s\n",
                      ior_file));

      poa_manager->activate();

      orb->run ();

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("SERVER (%P): Caught exception:");
      return 1;
    }

  return 0;
}
