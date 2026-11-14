#include "Hello.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include <memory>

int
parse_args (int argc,
            ACE_TCHAR *argv[],
            ACE_TCHAR const *&server_ior,
            ACE_TCHAR const *&ior_output_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        server_ior = get_opts.opt_arg ();
        break;
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-o <iorfile> "
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
  ACE_TCHAR const *server_ior = ACE_TEXT ("file://server.ior");
  ACE_TCHAR const *ior_output_file = ACE_TEXT ("client.ior");

  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, server_ior, ior_output_file) != 0)
        return 1;

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

      Hello *hello_impl {};
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in ()),
                      1);
      // Transfer servant ownership to the reference-counted wrapper.
      PortableServer::ServantBase_var owner_transfer (hello_impl);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (hello_impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      Test::Hello_var const hello =
        Test::Hello::_narrow (object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (hello.in ());

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

      CORBA::Object_var const tmp =
        orb->string_to_object (server_ior);

      Test::Hello_var const server =
        Test::Hello::_narrow (tmp.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior.in ()),
                            1);
        }

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) - Sending client obj ref to the server and requesting a callback.\n"));

      server->request_callback (hello.in ());

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Test (client) Succeeded !!!\n"));

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ACE_DEBUG ((LM_ERROR, "(%P|%t) - Test (client) Failed !!!\n"));
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
