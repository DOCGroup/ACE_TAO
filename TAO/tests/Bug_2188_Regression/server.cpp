#include "broken_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
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
  ACE_TCHAR const *ior_output_file = ACE_TEXT ("broken.ior");

  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv, ior_output_file) != 0)
        return 1;

      ArrayTest_i *target_servant {};
      ACE_NEW_RETURN (target_servant, ArrayTest_i, 1);

      // Transfer servant ownership to the reference-counted wrapper.
      PortableServer::ServantBase_var owner_transfer(target_servant);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (target_servant);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      ArrayTest_var const atobj =
        ArrayTest::_narrow (object.in ());

      ServerAdmin_i *servant {};
      ACE_NEW_RETURN (servant, ServerAdmin_i (atobj.in ()), 1);

      // Release the old servant reference and transfer ownership of this servant.
      owner_transfer = servant;

      id = root_poa->activate_object (servant);
      object = root_poa->id_to_reference (id.in ());
      ServerAdmin_var const saobj = ServerAdmin::_narrow (object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (saobj.in ());

      // Output the IOR to the <ior_output_file>
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior_output_file, "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s",
                             ior_output_file),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
