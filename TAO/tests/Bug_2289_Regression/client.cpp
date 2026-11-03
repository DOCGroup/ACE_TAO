#include "TestS.h"
#include "MyInterfaceImpl.h"
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
  ACE_TCHAR const *server_ior = ACE_TEXT ("");
  ACE_TCHAR const *ior_output_file = ACE_TEXT ("client.ior");
  int result {};

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

      MyInterfaceImpl *test_impl {};
      ACE_NEW_RETURN (test_impl,
                      MyInterfaceImpl (orb.in ()),
                      1);

      // Transfer servant ownership to the reference-counted wrapper.
      PortableServer::ServantBase_var owner_transfer (test_impl);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (test_impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      MyInterface_var const test_ref =
        MyInterface::_narrow (object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (test_ref.in ());

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

      MyInterface_var const server =
        MyInterface::_narrow (tmp.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil server reference <%s>\n",
                             ior.in ()),
                            1);
        }

      CORBA::Boolean const temp = server->myMethod (MyInterfaceImpl::my_string);

      if (temp)
        {
          ACE_DEBUG ((LM_DEBUG, "Test succeeded\n"));
        }
      else
        {
          ACE_DEBUG ((LM_ERROR, "Test failed\n"));
          result = 1;
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return result;
}
