// $Id$

#include "Hello.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/SString.h"
#include "tao/IORTable/IORTable.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("test.ior");
const ACE_TCHAR *ior_output_file2 = ACE_TEXT ("forward_forever.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:p:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 'p':
        ior_output_file2 = get_opts.opt_arg ();
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

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - ORB_init\n"));
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - Obtain RootPOA\n"));
      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - parse args\n"));
      if (parse_args (argc, argv) != 0)
        return 1;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - init hello\n"));
      Hello *hello_impl = 0;
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer(hello_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (hello_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Test::Hello_var hello = Test::Hello::_narrow (object.in ());

      CORBA::String_var ior = orb->object_to_string (hello.in ());

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - Create the forwarding loop\n"));
      CORBA::Object_var iorTableObj =
            orb->resolve_initial_references ("IORTable");

      IORTable::Table_var iorTable
          = IORTable::Table::_narrow (iorTableObj.in ());

      // We are using this test with ObjRefStyle == URL ... or at
      // least we better be. The IOR is therefore a corbaloc
      ACE_CString full_corbaloc (ior.in (), 0, 1);

      // Create a corbaloc for an IOR table binding that points to
      // itself. Acessing this will  make the server reply with LOCATION_FORWARD
      // indefinitely.

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - Corbaloc is\n  \"%C\"\n", full_corbaloc.c_str()));
      // Get the endpoint info only...
      CORBA::ULong first_slash = full_corbaloc.find ("/", 0);
      ACE_CString forward_forever = full_corbaloc.substring (0,
                                                             first_slash);

      // .. add the string we are going to bind against and then bind
      forward_forever += "/hello";
      iorTable->bind("hello", forward_forever.c_str ());

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - Forward forever is\n  \"%C\"\n", forward_forever.c_str()));
      // Output the IORs
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s\n",
                           ior_output_file),
                           1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      output_file = ACE_OS::fopen (ior_output_file2, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s\n",
                           ior_output_file2),
                           1);
      ACE_OS::fprintf (output_file, "%s", forward_forever.c_str ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - orb->run()\n"));
      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
